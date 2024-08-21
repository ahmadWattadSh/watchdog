# vars
CC=gcc
# CFLAGS will be the options passed to the compiler. -pedantic-errors -Wall -Wextra -Werror -g
CFLAGS=-ansi -g
#path of header
PATH_HEADER= -I/home/asus/Desktop/Git/projects/watchdog/schedular/header
PATH_HEADER_SECOND = -I/home/asus/Desktop/Git/projects/watchdog/schedular/lib

all: libwd_lib.so

sched.o: ./src/sched.c  
	$(CC) $(CFLAGS) -c -fpic ./src/sched.c $(PATH_HEADER) $(PATH_HEADER_SECOND) -o sched.o

libwd_lib.so: ./dll.o ./sorted_list.o ./uid.o ./task.o ./pq.o
	$(CC) $(CFLAGS) -shared -o ./lib/libwd_lib.so dll.o sorted_list.o uid.o task.o pq.o sched.o keep_watching.o

#	$(CC) $(CFLAGS) sched.o sched_test.o -L$(PATH_HEADER_SECOND) -o sched.out ./lib/liball_lib.a
#sched.out: sched.o sched_test.o
#	$(CC) $(CFLAGS) sched.o sched_test.o -L$(PATH_HEADER_SECOND) -o sched.out ./lib/liball_lib.a

cleanobj:
	rm *.o
clean:
	rm *.o *.out *.so
