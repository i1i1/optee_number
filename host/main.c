#include <err.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <tee_client_api.h>

#include <number_ta.h>


int main_args(void)
{
	TEEC_Result res;
	TEEC_Context ctx;
	TEEC_Session sess;
	TEEC_Operation op;
	TEEC_UUID uuid = TA_NUMBER_UUID;
	uint32_t err_origin;

        res = TEEC_InitializeContext(NULL, &ctx);
	if (res != TEEC_SUCCESS)
		errx(1, "TEEC_InitializeContext failed with code 0x%x", res);

	res = TEEC_OpenSession(&ctx, &sess, &uuid,
			       TEEC_LOGIN_USER, NULL, NULL, &err_origin);
	if (res != TEEC_SUCCESS)
		errx(1, "TEEC_Opensession failed with code 0x%x origin 0x%x",
			res, err_origin);

	memset(&op, 0, sizeof(op));

	op.paramTypes = TEEC_PARAM_TYPES(TEEC_VALUE_INOUT,
                                         TEEC_NONE, TEEC_NONE, TEEC_NONE);
	op.params[0].value.a = 23;
	op.params[0].value.b = 46;

        printf("Op params %d %d\n", op.params[0].value.a, op.params[0].value.b);

        res = TEEC_InvokeCommand(&sess, TA_NUMBER_CMD_SUM, &op,
				 &err_origin);
	if (res != TEEC_SUCCESS)
		errx(1, "TEEC_InvokeCommand failed with code 0x%x origin 0x%x",
			res, err_origin);
        printf("Op params %d %d\n", op.params[0].value.a, op.params[0].value.b);

	TEEC_CloseSession(&sess);
	TEEC_FinalizeContext(&ctx);

	return 0;
}

int main_shared(void)
{
	TEEC_Result res;
	TEEC_Context ctx;
	TEEC_Session sess;
	TEEC_Operation op;
	TEEC_UUID uuid = TA_NUMBER_UUID;
	uint32_t err_origin;
        uint32_t *p;

        res = TEEC_InitializeContext(NULL, &ctx);
	if (res != TEEC_SUCCESS)
		errx(1, "TEEC_InitializeContext failed with code 0x%x", res);

	res = TEEC_OpenSession(&ctx, &sess, &uuid,
			       TEEC_LOGIN_USER, NULL, NULL, &err_origin);
	if (res != TEEC_SUCCESS)
		errx(1, "TEEC_Opensession failed with code 0x%x origin 0x%x",
			res, err_origin);

        if (p = malloc(2 * sizeof(uint32_t)), !p)
		errx(1, "malloc failed");

	memset(&op, 0, sizeof(op));

	op.paramTypes = TEEC_PARAM_TYPES(TEEC_MEMREF_TEMP_INOUT,
                                         TEEC_NONE, TEEC_NONE, TEEC_NONE);
	op.params[0].tmpref.buffer = p;
	op.params[0].tmpref.size = 2 * sizeof(uint32_t);
        p[0] = 23;
        p[1] = 46;

        printf("Op params %d %d\n", p[0], p[1]);

        res = TEEC_InvokeCommand(&sess, TA_NUMBER_CMD_SUM, &op,
				 &err_origin);
	if (res != TEEC_SUCCESS)
		errx(1, "TEEC_InvokeCommand failed with code 0x%x origin 0x%x",
			res, err_origin);
        printf("Op params %d %d\n", p[0], p[1]);

        free(p);
	TEEC_CloseSession(&sess);
	TEEC_FinalizeContext(&ctx);

	return 0;
}

int main(void)
{
        return main_shared();
}

