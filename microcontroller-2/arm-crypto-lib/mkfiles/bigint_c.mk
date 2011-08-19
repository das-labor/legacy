# Makefile for BigInt
ALGO_NAME := BIGINT_C

# comment out the following line for removement of BigInt from the build process
AUX += $(ALGO_NAME)

$(ALGO_NAME)_DIR      := bigint/
$(ALGO_NAME)_INCDIR   := memxor/ noekeon/
$(ALGO_NAME)_OBJ      := bigint.o bigint_io.o  
$(ALGO_NAME)_TESTBIN := main-bigint-test.o $(CLI_STD)  \
                         performance_test.o noekeon.o noekeon_prng.o memxor.o
			 
$(ALGO_NAME)_PERFORMANCE_TEST := performance

