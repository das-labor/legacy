# Makefile for PRF Tlsv1.2
ALGO_NAME := PRF_TLS12

# comment out the following line for removement of PRF TLSv1.2 from the build process
AUX += $(ALGO_NAME)

$(ALGO_NAME)_DIR      := prf_tls12/
$(ALGO_NAME)_INCDIR   := hmac/ sha512/ sha256/ hfal/
$(ALGO_NAME)_OBJ      := prf_tls12.o
$(ALGO_NAME)_TESTBIN := main-prf_tls12-test.o $(CLI_STD) hfal_sha512.o hfal_sha256.o $(HFAL_STD) \
                        sha256.o sha512.o hfal-hmac.o
			 
$(ALGO_NAME)_PERFORMANCE_TEST := performance

