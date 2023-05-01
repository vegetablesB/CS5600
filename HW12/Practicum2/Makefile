CC = gcc
CFLAGS = -Wall -pthread
SERVER_TARGET = server
CLIENT_TARGET = fget
TEST_TARGET = test
TEST_THREAD = multi_client_test
SERVER_OBJS = server.o
CLIENT_OBJS = client.o client_utils.o
TEST_OBJS = test.o
TEST_THREAD_OBJS = multi_client_test.o client_utils.o

all: $(SERVER_TARGET) $(CLIENT_TARGET) $(TEST_TARGET) $(TEST_THREAD)

$(SERVER_TARGET): $(SERVER_OBJS)
	$(CC) $(CFLAGS) -o $(SERVER_TARGET) $(SERVER_OBJS)

$(CLIENT_TARGET): $(CLIENT_OBJS)
	$(CC) $(CFLAGS) -o $(CLIENT_TARGET) $(CLIENT_OBJS)

$(TEST_TARGET): $(TEST_OBJS)
	$(CC) $(CFLAGS) -o $(TEST_TARGET) $(TEST_OBJS)

$(TEST_THREAD): $(TEST_THREAD_OBJS)
	$(CC) $(CFLAGS) -o $(TEST_THREAD) $(TEST_THREAD_OBJS)

server.o: server.c
	$(CC) $(CFLAGS) -c server.c

client.o: client.c client_utils.h
	$(CC) $(CFLAGS) -c client.c

client_utils.o: client_utils.c client_utils.h
	$(CC) $(CFLAGS) -c client_utils.c

test.o: test.c
	$(CC) $(CFLAGS) -c test.c

multi_client_test.o: multi_client_test.c
	$(CC) $(CFLAGS) -c multi_client_test.c

clean:
	rm -f $(SERVER_OBJS) $(CLIENT_OBJS) $(SERVER_TARGET) $(CLIENT_TARGET) $(TEST_OBJS) $(TEST_TARGET) $(TEST_THREAD_OBJS) $(TEST_THREAD)

tar:
	tar -cvf Practicum2.tar Makefile server.c server.h client.c client_utils.c client_utils.h test.c multi_client_test.c README config.txt

