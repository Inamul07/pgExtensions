EXTENSION = cache_ext
DATA = $(wildcard *--*.sql)
TESTS = $(wildcard test/sql/*.sql)

REGRESS_OPTS = --inputdir=test --load-extension=cache_ext --outpiutdir=test
REGRESS = $(patsubst test/sql/%.sql,%,$(TESTS))
OBJS = $(patsubst %.c,%.o,$(wildcard src/*.c))
MODULE_big = $(EXTENSION)

PG_CONFIG = ../../bin/pg_config
PGXS := $(shell $(PG_CONFIG) --pgxs)
PG_CPPFLAGS += -I../../include/
SHLIB_LINK += -lpq
include $(PGXS)
