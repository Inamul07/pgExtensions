#include "base36.h"

PG_MODULE_MAGIC;

PG_FUNCTION_INFO_V1(base36_in);
Datum base36_in(PG_FUNCTION_ARGS) {
	long result;
	char* chrPtr;
	char* str = PG_GETARG_CSTRING(0);
	result = strtol(str, &chrPtr, 36);

	if(chrPtr[0] != '\0' || strlen(str) == 0) {
		BASE36_SYNTAX_ERROR(str);
	}

	if(result < 0) {
		elog(ERROR, "Value %ld is negative. Give positive values", result);
	}

	PG_RETURN_DATUM(DirectFunctionCall1(int84, (int64) result));
}

PG_FUNCTION_INFO_V1(base36_out);
Datum base36_out(PG_FUNCTION_ARGS) {
	int32 arg = PG_GETARG_INT32(0);
	char base36[36] = "0123456789abcdefghijklmnopqrstuvwxyz";

	char buffer[7];
	unsigned int offset;

	if(arg < 0) {
		elog(ERROR, "Value %d is negative. Give positive values", arg);
	}

	offset = 7;
	buffer[--offset] = '\0';

	do {
		buffer[--offset] = base36[arg % 36];
	} while(arg /= 36);

	PG_RETURN_CSTRING(pstrdup(&buffer[offset]));
}
