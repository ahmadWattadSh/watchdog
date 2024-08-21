/*Name: Ahmad Wattad
  Project: calculator using fsm machine
  Reviewer: ??
  Date: 03/10/2023
*/
#include <stddef.h> /*size_t*/
#include <assert.h> /*assert*/
#include <sys/types.h>
#include <stdio.h>
#include <fcntl.h>
#include <semaphore.h>
#include <string.h>

#include "keep_watching.h"

/******************************** ENUMS ***************************************/
enum
{ WD_PATH = 0, INTERVAL = 1, MAX_FAIL = 2, FIRST_USER_ARGV_IDX = 3};

enum{ SUCCEED = 0,FAIL};

/******************************** FUNCS ***************************************/

/*----------------------------------------------------------------------------*/

/***************************** FORWARDED FUNCS ********************************/
void Cpy(char *str1, char *str2);

/******************************** API FUNC ************************************/

int main(int argc, char *argv[])
{
  /*put*/
  pid_t pid = 0;
  int status = 0;
  sem_t *sem = NULL;

  sem = sem_open("pSem", O_CREAT);
  /*HanleErr*/
  pid = getppid();

  sem_post(sem);

  if (-1 == sem_close(sem))
  {
    perror("sem");
    return FAIL;
  }

  return KeepWatching(argv, pid);
}

