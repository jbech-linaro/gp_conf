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
#include <string.h>

#include <tee_internal_api.h>
#include <tee_internal_api_extensions.h>

#include <gp_conf_ta.h>

/*
 * Called when the instance of the TA is created. This is the first call in
 * the TA.
 */
TEE_Result TA_CreateEntryPoint(void)
{
	DMSG("has been called");
	return TEE_SUCCESS;
}

/*
 * Called when the instance of the TA is destroyed if the TA has not
 * crashed or panicked. This is the last call in the TA.
 */
void TA_DestroyEntryPoint(void)
{
	DMSG("has been called");
}

/*
 * Called when a new session is opened to the TA. *sess_ctx can be updated
 * with a value to be able to identify this session in subsequent calls to the
 * TA. In this function you will normally do the global initialization for the
 * TA.
 */
TEE_Result TA_OpenSessionEntryPoint(uint32_t param_types,
		TEE_Param  params[4], void **sess_ctx)
{
	uint32_t exp_param_types = TEE_PARAM_TYPES(TEE_PARAM_TYPE_NONE,
						   TEE_PARAM_TYPE_NONE,
						   TEE_PARAM_TYPE_NONE,
						   TEE_PARAM_TYPE_NONE);
	if (param_types != exp_param_types)
		return TEE_ERROR_BAD_PARAMETERS;

	/* Unused parameters */
	(void)&params;
	(void)&sess_ctx;

	DMSG("has been called");

	return TEE_SUCCESS;
}

/*
 * Called when a session is closed, sess_ctx hold the value that was
 * assigned by TA_OpenSessionEntryPoint().
 */
void TA_CloseSessionEntryPoint(void *sess_ctx)
{
	(void)&sess_ctx; /* Unused parameter */
	DMSG("has been called");
}

static TEE_Result invoke_print(uint32_t param_types, TEE_Param params[4])
{
	uint32_t exp_param_types = TEE_PARAM_TYPES(TEE_PARAM_TYPE_MEMREF_INPUT,
						   TEE_PARAM_TYPE_NONE,
						   TEE_PARAM_TYPE_NONE,
						   TEE_PARAM_TYPE_NONE);
	if (param_types != exp_param_types)
		return TEE_ERROR_BAD_PARAMETERS;

	DMSG("message: %s", (char *)params[0].memref.buffer);

	return TEE_SUCCESS;
}

static TEE_Result invoke_generate_hash(uint32_t param_types,
				       TEE_Param params[4])
{
	TEE_Result res = TEE_ERROR_BAD_PARAMETERS;

	void *message = NULL;
	size_t message_len;

	void *digest = NULL;
	size_t digest_len;

	TEE_OperationHandle operation;

	uint32_t exp_param_types = TEE_PARAM_TYPES(TEE_PARAM_TYPE_MEMREF_INPUT,
						   TEE_PARAM_TYPE_MEMREF_OUTPUT,
						   TEE_PARAM_TYPE_NONE,
						   TEE_PARAM_TYPE_NONE);
	DMSG("has been called");
	if (param_types != exp_param_types)
		return TEE_ERROR_BAD_PARAMETERS;

	message = params[0].memref.buffer;
	message_len = params[0].memref.size;

	digest = params[1].memref.buffer;
	digest_len = params[1].memref.size;

	res = TEE_AllocateOperation(&operation, TEE_ALG_SHA1, TEE_MODE_DIGEST, 0);

	if (res != TEE_SUCCESS) {
		DMSG("TEE_AllocateOperation failed! res: 0x%x", res);
		goto out;
	}

	res = TEE_DigestDoFinal(operation, message, message_len, digest, &digest_len);

	if (res != TEE_SUCCESS) {
		DMSG("TEE_DigestDoFinal failed! res: 0x%x", res);
		goto out;
	}
out:
	if (operation)
		TEE_FreeOperation(operation);

	return TEE_SUCCESS;
}

static TEE_Result invoke_get_random(uint32_t param_types, TEE_Param params[4])
{
	uint8_t *random_data;
	uint32_t data_length;

	uint32_t exp_param_types = TEE_PARAM_TYPES(TEE_PARAM_TYPE_MEMREF_INOUT,
						   TEE_PARAM_TYPE_NONE,
						   TEE_PARAM_TYPE_NONE,
						   TEE_PARAM_TYPE_NONE);

	DMSG("has been called");
	if (param_types != exp_param_types)
		return TEE_ERROR_BAD_PARAMETERS;

	random_data = params[0].memref.buffer;
	data_length = params[0].memref.size;

	TEE_GenerateRandom(random_data, data_length);

	return TEE_SUCCESS;
}

/*
 * Called when a TA is invoked. sess_ctx hold that value that was
 * assigned by TA_OpenSessionEntryPoint(). The rest of the paramters
 * comes from normal world.
 */
TEE_Result TA_InvokeCommandEntryPoint(void *sess_ctx, uint32_t cmd_id,
				      uint32_t param_types, TEE_Param params[4])
{
	(void)&sess_ctx; /* Unused parameter */

	DMSG("has been called: cmd_id: %d", cmd_id);

	switch (cmd_id) {
	case GP_CONF_TA_PRINT:
		return invoke_print(param_types, params);

	case GP_CONF_TA_HASH:
		return invoke_generate_hash(param_types, params);

	case GP_CONF_TA_RANDOM:
		return invoke_get_random(param_types, params);

	default:
		return TEE_ERROR_BAD_PARAMETERS;
	}
}
