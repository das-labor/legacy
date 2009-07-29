# Makefile for noekeon
ALGO_NAME := NOEKEON

# comment out the following line for removement of noekeon from the build process
BLOCK_CIPHERS += $(ALGO_NAME)


$(ALGO_NAME)_OBJ      := noekeon_asm.o
$(ALGO_NAME)_TEST_BIN := main-noekeon-test.o $(CLI_STD)  \
                         nessie_bc_test.o nessie_common.o performance_test.o
$(ALGO_NAME)_NESSIE_TEST      := test nessie
$(ALGO_NAME)_PERFORMANCE_TEST := performance

