# Makefile for bcal-basic (library)
ALGO_NAME := BCAL_BASIC

$(ALGO_NAME)_DIR      := bcal/
$(ALGO_NAME)_INCDIR   := memxor/
$(ALGO_NAME)_OBJ      := bcal-basic.o keysize_descriptor.o
$(ALGO_NAME)_NESSIE_TEST      := test nessie
$(ALGO_NAME)_PERFORMANCE_TEST := performance

