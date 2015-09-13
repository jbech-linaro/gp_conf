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

/*
 * This function should send the string "abc" to secure side and secure side
 * will hash (SHA1) the string and send it back to the client in normal world
 * user space.
 *
 * Correct sha1("abc") = a9993e364706816aba3e25717850c26c9cd0d89d
 */
void get_hash(void)
{
#ifdef REMOVE_AND_FILL_IN_MISSING
	const char *message = "abc";

	/* 
	 * Spec:
	 * TEEC_Result TEEC_InitializeContext(const char* name,
	 *                        	      TEEC_Context* context)
	 */
	res = TEEC_InitializeContext();

	/*
	 * Spec:
	 * TEEC_Result TEEC_OpenSession(TEEC_Context* context,
	 * 				TEEC_Session* session,
	 * 				const TEEC_UUID* destination,
	 * 				uint32_t connectionMethod,
	 * 				const void* connectionData,
	 * 				TEEC_Operation* operation,
	 * 				uint32_t* returnOrigin)
	 */
	res = TEEC_OpenSession();

	/*
	 * Spec:
	 * TEEC_Result TEEC_InvokeCommand(TEEC_Session* session,
	 * 				  uint32_t commandID,
	 * 				  TEEC_Operation* operation,
	 * 				  uint32_t* returnOrigin)
	 */
	res = TEEC_InvokeCommand();

	/*
	 * Spec:
	 * void TEEC_CloseSession(TEEC_Session* session)
	 */
	TEEC_CloseSession();

	/*
	 * Spec:
	 * void TEEC_FinalizeContext(TEEC_Context* context)
	 */
	TEEC_FinalizeContext();
#endif
}

/*
 * This function should send a buffer of a certain length to secure side, where
 * secure side is supposed to generate a random number, which then are sent back
 * to the client in normal world / user space.
 */
void get_random(void)
{
}

int main(int argc, char *argv[])
{
	trigger_ta_print();
	get_hash();
	get_random();

	return 0;
}
