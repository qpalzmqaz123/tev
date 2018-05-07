FILES = $(shell find ./src -maxdepth 1 -name '*.c')
FILES += $(shell find ./src/port/unix -maxdepth 1 -name '*.c')
FILES += test.c

CFLAGS += -g -Wall
CFLAGS += -I./src/include -lpthread

all:
	gcc $(CFLAGS) $(FILES)
