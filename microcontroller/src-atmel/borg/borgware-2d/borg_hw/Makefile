TARGET = libborg_hw.a
TOPDIR = ..

SRC_SIM := 

include $(TOPDIR)/defaults.mk


ifeq ($(BORG_HW),HW_BORG_16)
  SRC = borg_hw_borg16.c
endif

ifeq ($(BORG_HW),HW_BORG_ANDRE)
  SRC = borg_hw_andreborg.c
endif

ifeq ($(BORG_HW),HW_BORG_LS)
  SRC = borg_hw_borg_ls.c
endif

ifeq ($(BORG_HW),HW_BORG_MH)
  SRC = borg_hw_borg_mh.c
endif

ifeq ($(BORG_HW),HW_BORG_MINI)
  SRC = borg_hw_borg_mini.c
endif

ifeq ($(SRC),'')
  $(error no valid hardware driver selected )
endif

include $(TOPDIR)/rules.mk
