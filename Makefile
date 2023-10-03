# contrib/my_first_extension/Makefile

MODULE_big = my_first_extension
OBJS = calculate_digit.o \
       generate_check_digit.o \
       validate_check_digit.o

EXTENSION = my_first_extension
DATA = my_first_extension--1.0.sql
PGFILEDESC = "my_first_extension - ID generation"

REGRESS = my_test

SHLIB_LINK += $(UUID_LIBS)

ifdef USE_PGXS
PG_CONFIG =/usr/local/pgsql/bin/pg_config
PGXS := $(shell $(PG_CONFIG) --pgxs)
include $(PGXS)
else
subdir = contrib/my_first_extension
top_builddir = ../..
include $(top_builddir)/src/Makefile.global
include $(top_srcdir)/contrib/contrib-global.mk
endif
