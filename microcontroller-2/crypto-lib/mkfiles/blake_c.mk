# Makefile for Blake
ALGO_NAME := BLAKE_C

# comment out the following line for removement of Blake from the build process
HASHES += $(ALGO_NAME)


$(ALGO_NAME)_OBJ      := blake_small.o blake_large.o blake_common.o memxor.o
$(ALGO_NAME)_TEST_BIN := main-blake-test.o debug.o uart.o hexdigit_tab.o  \
                         dbz_strings.o nessie_common.o cli.o string-extras.o performance_test.o \
			 nessie_hash_test.o hfal-basic.o hfal_blake_small.o hfal_blake_large.o \
			 shavs.o hfal-nessie.o hfal-test.o hfal-performance.o
$(ALGO_NAME)_NESSIE_TEST      := test nessie
$(ALGO_NAME)_PERFORMANCE_TEST := performance

