CC = gcc
CFLAGS := -Wall -g

all = client

target: $(all)
	@echo "all is done"

client: client.o poll_n.o sendFd.o func.o mvfile.o handleRet.o
	$(CC) -o $@ $^

clean:
	rm $(all) *.o
