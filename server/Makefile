CC := gcc
CFLAGS := -Wall -g
all := servermain
LIB := -lpthread -lmysqlclient
INCLUDE := /usr/include/mysql/
target: $(all)
	@echo "all is done"

servermain: servermain.o work_que.o factory.o func.o poll_n.o mvfile.o cmdAnylasis.o cmd.o lscmd.o mysql.o
	$(CC) -o $@ $^ -I $(INCLUDE) $(LIB)

clean:
	rm $(all) *.o

