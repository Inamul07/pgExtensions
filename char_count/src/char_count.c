#include "postgres.h"
#include "fmgr.h"
#include "utils/builtins.h"

#include "u_dshash.h"
#include "u_pglist.h"
#include "u_ilist.h"
#include "u_dynahash.h"

#include "utils/rel.h"
#include "catalog/pg_type.h"
#include "catalog/pg_class.h"
#include "access/htup_details.h"

PG_MODULE_MAGIC;

PG_FUNCTION_INFO_V1(char_count);

Datum char_count(PG_FUNCTION_ARGS) {
	int charCount = 0;
	int i = 0;
	text* inputText = PG_GETARG_TEXT_PP(0);
	text* targetChar = PG_GETARG_TEXT_PP(1);

	int inputTextSize = VARSIZE(inputText)-VARHDRSZ;
	int targetCharSize = VARSIZE(targetChar)-VARHDRSZ;
	char* cp_inputText = (char*) palloc(inputTextSize + 1);
	char* cp_targetChar = (char*) palloc(targetCharSize + 1);

	if(targetCharSize > 1) {
		elog(ERROR, "arg2 must be of length 1");
	}

	memcpy(cp_inputText, VARDATA(inputText), inputTextSize);
	memcpy(cp_targetChar, VARDATA(targetChar), targetCharSize);

	while (i < inputTextSize) {
		if(cp_inputText[i] == cp_targetChar[0])
			charCount++;
		i++;
	}

	executeDynahash();

	pfree(cp_inputText);
	pfree(cp_targetChar);
	PG_RETURN_INT32(charCount);
}

PG_FUNCTION_INFO_V1(print_count);

Datum print_count(PG_FUNCTION_ARGS) {
	int32 count = PG_GETARG_INT32(0);
	PG_RETURN_INT32(count);
}