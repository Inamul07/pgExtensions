#ifndef BASE36_H
#define BASE36_H

#include "postgres.h"
#include "fmgr.h"
#include "utils/builtins.h"
#include "libpq/pqformat.h"
#include <limits.h>

extern const char base36[36];

#define BASE36_OUT_OF_RANGE_ERROR() elog(ERROR, "base36 out of range.");

#define BASE36_SYNTAX_ERROR(_str) elog(ERROR, "Invalid input syntax for base36: %s", str);

#endif