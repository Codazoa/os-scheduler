CC = gcc
CFLAGS = -Wall -g -Werror
OBJ = main.o cpu_thread.o io_thread.o file_parser.o double_linked_list.o process.o
HDRS = cpu_thread.h file_parser.h io_thread.h double_linked_list.h process.h options.h

all: scheduler

scheduler: $(OBJ)
	$(CC) $(CFLAGS) -o scheduler $(OBJ) -lpthread

main.o: main.c $(HDRS)
	$(CC) $(CFLAGS) -c main.c

cpu_thread.o: cpu_thread.c $(HDRS)
	$(CC) $(CFLAGS) -c cpu_thread.c

io_thread.o: io_thread.c $(HDRS)
	$(CC) $(CFLAGS) -c io_thread.c

file_parser.o: file_parser.c $(HDRS)
	$(CC) $(CFLAGS) -c file_parser.c

double_linked_list.o: double_linked_list.c $(HDRS)
	$(CC) $(CFLAGS) -c double_linked_list.c

process.o: process.c $(HDRS)
	$(CC) $(CFLAGS) -c process.c

clean:
	rm -f $(OBJ) scheduler