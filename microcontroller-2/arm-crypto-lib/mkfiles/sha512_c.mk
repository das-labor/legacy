# Makefile for SHA-512
ALGO_NAME := SHA512_C

# comment out the following line for removement of SHA-512 from the build process
HASHES += $(ALGO_NAME)

$(ALGO_NAME)_DIR      := sha512/
$(ALGO_NAME)_INCDIR   := hfal/
$(ALGO_NAME)_OBJ      := sha512.o
$(ALGO_NAME)_TESTBIN := main-sha512-test.o $(CLI_STD) $(HFAL_STD) hfal_sha512.o 
$(ALGO_NAME)_NESSIE_TEST      := "nessie"
$(ALGO_NAME)_PERFORMANCE_TEST := "performance"

