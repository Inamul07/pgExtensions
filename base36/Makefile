EXTENSION = base36
DATA = $(wildcard *--*.sql)
TESTS = $(wildcard test/sql/*.sql)

REGRESS_OPTS = --inputdir=test --load-extension=base36 --outputdir=test
REGRESS = $(patsubst test/sql/%.sql,%,$(TESTS))
OBJS = $(patsubst %.c,%.o,$(wildcard src/*.c))
MODULE_big = $(EXTENSION)

PG_CONFIG = ../../bin/pg_config
PGXS := $(shell $(PG_CONFIG) --pgxs)
include $(PGXS)
