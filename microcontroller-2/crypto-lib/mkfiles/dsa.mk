# Makefile for DSA
ALGO_NAME := DSA

# comment out the following line for removement of DSA from the build process
SIGNATURE += $(ALGO_NAME)

$(ALGO_NAME)_DIR      := dsa/
$(ALGO_NAME)_OBJ      := bigint.o bigint_io.o bigint_add_u.o sha1-asm.o dsa_sign.o dsa_verify.o dsa_key_blob.o base64_enc.o
$(ALGO_NAME)_TEST_BIN := main-dsa-test.o $(CLI_STD) hfal_sha1.o $(HFAL_STD) \
                         noekeon_asm.o noekeon_prng.o memxor.o 
			 
$(ALGO_NAME)_PERFORMANCE_TEST := performance

