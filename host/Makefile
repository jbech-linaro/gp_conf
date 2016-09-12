CC      = $(CROSS_COMPILE)gcc
LD      = $(CROSS_COMPILE)ld
AR      = $(CROSS_COMPILE)ar
NM      = $(CROSS_COMPILE)nm
OBJCOPY = $(CROSS_COMPILE)objcopy
OBJDUMP = $(CROSS_COMPILE)objdump
READELF = $(CROSS_COMPILE)readelf

OBJS = gp_conf_client.o

CFLAGS += -Wall -I../ta/include -I$(TEEC_EXPORT)/include -I./include -DDEBUG
LDADD += -lteec -L$(TEEC_EXPORT)/lib

.PHONY: all
all: gp_conf_client

gp_conf_client: $(OBJS)
	$(CC) $(LDADD) -o $@ $^

.PHONY: clean
clean:
	rm -f $(OBJS) gp_conf_client
