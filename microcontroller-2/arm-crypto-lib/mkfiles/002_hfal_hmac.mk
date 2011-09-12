# Makefile for hfal-hmac (library)
ALGO_NAME := HFAL_HMAC

$(ALGO_NAME)_DIR      := hfal/
$(ALGO_NAME)_INCDIR   := memxor/
$(ALGO_NAME)_OBJ      := hfal-basic.o hfal-hmac.o
$(ALGO_NAME)_NESSIE_TEST      := test nessie
$(ALGO_NAME)_PERFORMANCE_TEST := performance

