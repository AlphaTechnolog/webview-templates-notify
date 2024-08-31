CC=gcc
CFLAGS=-Wall -Werror -march=native -mtune=native -std=c89
LDFLAGS=

SRC=$(wildcard src/**/*.c src/*.c)
OBJS=$(SRC:.c=.o)

OUTPUT=webview-example

$(OUTPUT): $(OBJS)
	$(CC) -o $@ $^ $(LDFLAGS)

%.o: %.c
	$(CC) -c -o $@ $< $(CFLAGS)