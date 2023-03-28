CC = gcc
CFLAGS = -Wall -g #-Werror
OBJ = main.o cpu_thread.o io_thread.o file_parser.o
HDRS = cpu_thread.h file_parser.h io_thread.h

all: scheduler

scheduler: $(OBJ)
	$(CC) $(CFLAGS) -o scheduler $(OBJ)

main.o: main.c $(HDRS)
	$(CC) $(CFLAGS) -c main.c

cpu_thread.o: cpu_thread.c $(HDRS)
	$(CC) $(CFLAGS) -c cpu_thread.c

io_thread.o: io_thread.c $(HDRS)
	$(CC) $(CFLAGS) -c io_thread.c

file_parser.o: file_parser.c $(HDRS)
	$(CC) $(CFLAGS) -c file_parser.c

clean:
	rm -f $(OBJ) scheduler