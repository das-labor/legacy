# Makefile for RC6
ALGO_NAME := RC6

# comment out the following line for removement of RC6 from the build process
BLOCK_CIPHERS += $(ALGO_NAME)


$(ALGO_NAME)_OBJ      := rc6.o
$(ALGO_NAME)_TEST_BIN := main-rc6-test.o $(CLI_STD) \
                         nessie_bc_test.o nessie_common.o performance_test.o
$(ALGO_NAME)_NESSIE_TEST      := test nessie
$(ALGO_NAME)_PERFORMANCE_TEST := performance

