CC = gcc
CFLAGS = -Wall -pthread
SERVER_TARGET = server
CLIENT_TARGET = fget
SERVER_OBJS = server.o
CLIENT_OBJS = client.o client_utils.o

all: $(SERVER_TARGET) $(CLIENT_TARGET)

$(SERVER_TARGET): $(SERVER_OBJS)
	$(CC) $(CFLAGS) -o $(SERVER_TARGET) $(SERVER_OBJS)

$(CLIENT_TARGET): $(CLIENT_OBJS)
	$(CC) $(CFLAGS) -o $(CLIENT_TARGET) $(CLIENT_OBJS)

server.o: server.c
	$(CC) $(CFLAGS) -c server.c

client.o: client.c client_utils.h
	$(CC) $(CFLAGS) -c client.c

client_utils.o: client_utils.c client_utils.h
	$(CC) $(CFLAGS) -c client_utils.c

clean:
	rm -f $(SERVER_OBJS) $(CLIENT_OBJS) $(SERVER_TARGET) $(CLIENT_TARGET)
