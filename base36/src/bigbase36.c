#include "base36.h"

PG_FUNCTION_INFO_V1(bigbase36_in);
Datum bigbase36_in(PG_FUNCTION_ARGS) {
    long result;
    char *chrPtr;
    char *str = PG_GETARG_CSTRING(0);
    result = strtol(str, &chrPtr, 36);

    if(result == LONG_MIN || result == LONG_MAX) {
        BASE36_OUT_OF_RANGE_ERROR();
    }

    if(chrPtr[0] != '\0' || strlen(str) == 0) {
		BASE36_SYNTAX_ERROR(str);
	}

	if(result < 0) {
		elog(ERROR, "Value %ld is negative. Give positive values", result);
	}

    PG_RETURN_INT64((int64) result);
}

PG_FUNCTION_INFO_V1(bigbase36_out);
Datum bigbase36_out(PG_FUNCTION_ARGS) {
	int32 arg = PG_GETARG_INT32(0);
	char base36[36] = "0123456789abcdefghijklmnopqrstuvwxyz";

	char buffer[14];
	unsigned int offset;

	if(arg < 0) {
		elog(ERROR, "Value %d is negative. Give positive values", arg);
	}

	offset = 14;
	buffer[--offset] = '\0';

	do {
		buffer[--offset] = base36[arg % 36];
	} while(arg /= 36);

	PG_RETURN_CSTRING(pstrdup(&buffer[offset]));
}
