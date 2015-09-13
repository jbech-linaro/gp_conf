/*
 * Copyright (c) 2015, Linaro Limited
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 * 1. Redistributions of source code must retain the above copyright notice,
 * this list of conditions and the following disclaimer.
 *
 * 2. Redistributions in binary form must reproduce the above copyright notice,
 * this list of conditions and the following disclaimer in the documentation
 * and/or other materials provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE
 * LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 */
#include <stdio.h>
#include <string.h>
#include <err.h>

/* OP-TEE TEE client API */
#include <tee_client_api.h>

/* To get the UUID */
#include <gp_conf_ta.h>

#include <gp_conf_client.h>

static void dump_hash(uint8_t *hash, size_t len)
{
	size_t i;
	for (i = 0; i < len; i++)
		fprintf(stdout, "%02x", hash[i]);
	fprintf(stdout, "\n");
}

/*
 * This function will send a message from normal world user space down to a
 * Trusted Application in secure world, which will print the same on the secure
 * side.
 */
void trigger_ta_print(void)
{
	TEEC_Context ctx;
	TEEC_Operation op;
	TEEC_Result res;
	TEEC_Session session;
	TEEC_UUID uuid = GP_CONF_TA_UUID;
	uint32_t err_origin;

	const char message[] = "Message from user space normal world";

	/* Initialize a context connecting us to the TEE */
	res = TEEC_InitializeContext(NULL, &ctx);
	if (res != TEEC_SUCCESS)
		errx(1, "TEEC_InitializeContext failed with code 0x%x", res);

	/*
	 * Open the session, will trigger TA_OpenSessionEntryPoint on secure
	 * side.
	 */
	res = TEEC_OpenSession(&ctx, &session, &uuid, TEEC_LOGIN_PUBLIC, NULL,
			       NULL, &err_origin);
	if (res != TEEC_SUCCESS)
		errx(1, "TEEC_Opensession failed with code 0x%x origin 0x%x",
		     res, err_origin);

	memset(&op, 0, sizeof(op));
	op.paramTypes = TEEC_PARAM_TYPES(TEEC_MEMREF_TEMP_INPUT,
					 TEEC_NONE,
					 TEEC_NONE,
					 TEEC_NONE);

	op.params[0].tmpref.buffer = (void *)message;
	op.params[0].tmpref.size = strlen(message);

	res = TEEC_InvokeCommand(&session, GP_CONF_TA_PRINT, &op, &err_origin);
	if (res != TEEC_SUCCESS)
		errx(1, "TEEC_InvokeCommand failed with code 0x%x origin 0x%x",
		     res, err_origin);

	/* We're done with the TA, close the session ... */
	TEEC_CloseSession(&session);

	/* ... and destroy the context. */
	TEEC_FinalizeContext(&ctx);
}

void get_hash(void)
{
	TEEC_Context ctx;
	TEEC_Operation op;
	TEEC_Result res;
	TEEC_Session session;
	TEEC_UUID uuid = GP_CONF_TA_UUID;
	uint32_t err_origin;

	const char *message = "abc";
	uint8_t hash[20];
	memset(hash, 0, sizeof(hash));

	/* Initialize a context connecting us to the TEE */
	res = TEEC_InitializeContext(NULL, &ctx);
	if (res != TEEC_SUCCESS)
		errx(1, "TEEC_InitializeContext failed with code 0x%x", res);

	res = TEEC_OpenSession(&ctx, &session, &uuid, TEEC_LOGIN_PUBLIC, NULL,
			       NULL, &err_origin);
	if (res != TEEC_SUCCESS)
		errx(1, "TEEC_Opensession failed with code 0x%x origin 0x%x",
		     res, err_origin);

	memset(&op, 0, sizeof(op));
	op.paramTypes = TEEC_PARAM_TYPES(TEEC_MEMREF_TEMP_INPUT,
					 TEEC_MEMREF_TEMP_OUTPUT,
					 TEEC_NONE,
					 TEEC_NONE);

	op.params[0].tmpref.buffer = (void *)message;
	op.params[0].tmpref.size = strlen(message);

	op.params[1].tmpref.buffer = (void *)hash;
	op.params[1].tmpref.size = sizeof(hash);

	res = TEEC_InvokeCommand(&session, GP_CONF_TA_HASH, &op, &err_origin);
	if (res != TEEC_SUCCESS)
		errx(1, "TEEC_InvokeCommand failed with code 0x%x origin 0x%x",
		     res, err_origin);

	
	fprintf(stdout, "Hash of string \"%s\" from secure world\n", message);
	dump_hash(hash, sizeof(hash));

	/* We're done with the TA, close the session ... */
	TEEC_CloseSession(&session);

	/* ... and destroy the context. */
	TEEC_FinalizeContext(&ctx);
}

void get_random(void)
{
	TEEC_Context ctx;
	TEEC_Operation op;
	TEEC_Result res;
	TEEC_Session session;
	TEEC_UUID uuid = GP_CONF_TA_UUID;
	uint32_t err_origin;

	uint8_t random_data[20];

	/* Initialize a context connecting us to the TEE */
	res = TEEC_InitializeContext(NULL, &ctx);
	if (res != TEEC_SUCCESS)
		errx(1, "TEEC_InitializeContext failed with code 0x%x", res);

	res = TEEC_OpenSession(&ctx, &session, &uuid, TEEC_LOGIN_PUBLIC, NULL,
			       NULL, &err_origin);
	if (res != TEEC_SUCCESS)
		errx(1, "TEEC_Opensession failed with code 0x%x origin 0x%x",
		     res, err_origin);

	memset(&op, 0, sizeof(op));
	op.paramTypes = TEEC_PARAM_TYPES(TEEC_MEMREF_TEMP_INOUT,
					 TEEC_NONE,
					 TEEC_NONE,
					 TEEC_NONE);

	op.params[0].tmpref.buffer = (void *)random_data;
	op.params[0].tmpref.size = sizeof(random_data);

	res = TEEC_InvokeCommand(&session, GP_CONF_TA_RANDOM, &op, &err_origin);
	if (res != TEEC_SUCCESS)
		errx(1, "TEEC_InvokeCommand failed with code 0x%x origin 0x%x",
		     res, err_origin);

	fprintf(stdout, "Random data from secure world\n");
	dump_hash(random_data, sizeof(random_data));

	/* We're done with the TA, close the session ... */
	TEEC_CloseSession(&session);

	/* ... and destroy the context. */
	TEEC_FinalizeContext(&ctx);
}

int main(int argc, char *argv[])
{
	trigger_ta_print();
	get_hash();
	get_random();

	return 0;
}
