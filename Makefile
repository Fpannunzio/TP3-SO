
SERVER=ownServer
CLIENT=client

SOURCES_SERVER=server.c 
SOURCES_CLIENT=client.c

CC=gcc
CFLAGS= -Wall -pedantic -std=c99

all: $(SERVER) $(CLIENT)

$(SERVER): $(SOURCES_SERVER)
	$(CC) $(CFLAGS) $^ -o $@

$(CLIENT): $(SOURCES_CLIENT)
	$(CC) $(CFLAGS) $^ -o $@

clean:
	rm -f $(SERVER) $(CLIENT)

.PHONY: all, clean