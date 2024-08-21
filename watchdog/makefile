# vars
CC=gcc
# CFLAGS will be the options passed to the compiler. -pedantic-errors -Wall -Wextra -Werror -g
CFLAGS= -ansi -g
#path of header
PATH_HEADER= -I/home/asus/Desktop/Git/projects/watchdog/header

all:  wd_process.o wd_process.out program.out

wd_process.o:
	$(CC) -fpic $(CFLAGS) -c ./src/wd_process.c $(PATH_HEADER) -o ./watchdog2/wd_process.o
	
wd_process.out:
	$(CC) $(CFLAGS) ./watchdog2/wd_process.o -L/lib -o wd_process.out ./lib/libwd_lib.so -Wl,-rpath,/lib
	
watchdog.o:
	$(CC) -fpic $(CFLAGS) -c ./src/watchdog.c $(PATH_HEADER) -o watchdog.o
	
test_watchdog.o:
	$(CC) -fpic $(CFLAGS) -c ./test/test_watchdog.c $(PATH_HEADER) -o test_watchdog.o

program.out: ./watchdog.o ./test_watchdog.o
	$(CC) $(CFLAGS) watchdog.o test_watchdog.o -L/lib -o program.out ./lib/libwd_lib.so -Wl,-rpath,/lib
	
clean:
	rm *.o
