FILES = $(shell find ./src -name '*.c')
FILES += main.c

CFLAGS += -g -Wall
CFLAGS += -I./src/include

all:
	gcc $(CFLAGS) $(FILES)
