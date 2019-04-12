#include <tee_internal_api.h>
#include <tee_internal_api_extensions.h>

#include <number_ta.h>


TEE_Result TA_CreateEntryPoint(void)
{
	return TEE_SUCCESS;
}

void TA_DestroyEntryPoint(void)
{
}

TEE_Result TA_OpenSessionEntryPoint(uint32_t param_types,
		TEE_Param __maybe_unused params[4],
		void __maybe_unused **sess_ctx)
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

	return TEE_SUCCESS;
}

void TA_CloseSessionEntryPoint(void __maybe_unused *sess_ctx)
{
	(void)&sess_ctx; /* Unused parameter */
}

TEE_Result sum_value(uint32_t param_types, TEE_Param params[4]);

TEE_Result sum_value(uint32_t param_types, TEE_Param params[4])
{
        uint32_t memref, value;
        uint32_t *p;

        p = params[0].memref.buffer;
        memref = TEE_PARAM_TYPES(TEE_PARAM_TYPE_MEMREF_INOUT, TEE_PARAM_TYPE_NONE,
                                 TEE_PARAM_TYPE_NONE, TEE_PARAM_TYPE_NONE);
        value = TEE_PARAM_TYPES(TEE_PARAM_TYPE_VALUE_INOUT, TEE_PARAM_TYPE_NONE,
                                TEE_PARAM_TYPE_NONE, TEE_PARAM_TYPE_NONE);

        if (param_types == memref) {
                if (!p || params[0].memref.size != 2 * sizeof(uint32_t))
                        return TEE_ERROR_BAD_PARAMETERS;

                p[0] += p[1];
        } else if (param_types == value)
                params[0].value.a += params[0].value.b;
        else
                return TEE_ERROR_BAD_PARAMETERS;

	return TEE_SUCCESS;
}

TEE_Result TA_InvokeCommandEntryPoint(void __maybe_unused *sess_ctx,
			uint32_t cmd_id,
			uint32_t param_types, TEE_Param params[4])
{
	(void)&sess_ctx; /* Unused parameter */

        switch (cmd_id) {
	case TA_NUMBER_CMD_SUM:
		return sum_value(param_types, params);
	default:
		return TEE_ERROR_BAD_PARAMETERS;
	}
}
