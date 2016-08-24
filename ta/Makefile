CFG_TEE_TA_LOG_LEVEL ?= 5
CPPFLAGS += -DCFG_TEE_TA_LOG_LEVEL=$(CFG_TEE_TA_LOG_LEVEL)
BINARY=67707465-6563-6f6e-666c696e61726f15

include $(TA_DEV_KIT_DIR)/mk/ta_dev_kit.mk

#all: $O/$(BINARY).ta
#
#$O/$(BINARY).ta: $O/$(BINARY).bin
#	rm -f $@
#	cat faked_armv7_uta_signed_header.bin $< > $@
#
#clean: clean_ta_file
#
#.PHONY: clean_ta_file
#clean_ta_file:
#	rm -f $(BINARY).ta
