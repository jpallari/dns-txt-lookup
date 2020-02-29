CC=gcc
CFLAGS=-I. -Wall -Wextra -pedantic -Wshadow -O2
LDFLAGS=-lresolv
APP_NAME=dns-txt-lookup

$(APP_NAME): $(APP_NAME).c
	$(CC) -o $@ $^ $(CFLAGS) $(LDFLAGS)

.PHONY: clean
clean:
	rm -f $(APP_NAME)

