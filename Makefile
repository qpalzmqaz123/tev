FILES = $(shell find ./src -maxdepth 1 -name '*.c')
FILES += $(shell find ./src/port/unix -maxdepth 1 -name '*.c')
FILES += main.c

CFLAGS += -g -Wall
CFLAGS += -I./src/include

all:
	gcc $(CFLAGS) $(FILES)
