
SERVER=ownServer
CLIENT=client

SOURCES_SERVER=server.c 
SOURCES_CLIENT=client.c

CC=gcc
CFLAGS= -Wall -pedantic -std=c99

all: $(SERVER) $(CLIENT)

$(SERVER): $(SOURCES_SERVER)
	$(CC) $(CFLAGS) $^ -o $@
	touch run_me.o
	objcopy --add-section .RUN_ME=run_me.o $(SERVER) $(SERVER)
	rm run_me.o


$(CLIENT): $(SOURCES_CLIENT)
	$(CC) $(CFLAGS) $^ -o $@

clean:
	rm -f $(SERVER) $(CLIENT)

.PHONY: all, clean