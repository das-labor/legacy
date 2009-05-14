# Makefile for TWISTER
ALGO_NAME := TWISTER_C

# comment out the following line for removement of TWISTER from the build process
HASHES += $(ALGO_NAME)

$(ALGO_NAME)_OBJ      := twister.o twister-small.o twister-big.o memxor.o gf256mul.o
$(ALGO_NAME)_TEST_BIN := main-twister-test.o hfal_twister224.o hfal_twister256.o \
                         hfal_twister384.o hfal_twister512.o $(CLI_STD) $(HFAL_STD)
$(ALGO_NAME)_NESSIE_TEST      := "nessie"
$(ALGO_NAME)_PERFORMANCE_TEST := "performance"

