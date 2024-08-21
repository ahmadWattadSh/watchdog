/*Name: Ahmad Wattad
  Project: 
  Reviewer: ??
  Date: 03/10/2023
*/
#include <stddef.h> /*size_t*/
#include <assert.h> /*assert*/
#include <signal.h>
#include <pthread.h>
#include <asm/signal.h>
#include <stdio.h> 
#include <semaphore.h> 
#include <fcntl.h>
#include <errno.h>
#include <stdlib.h>

#include "watchdog.h" 
#include "keep_watching.h" 


#define args_num 4
#define PATH_WD_PROCESS "wd_process.out"


enum {WD_PATH = 0, INTERVAL = 1, MAX_FAIL = 2, FIRST_USER_ARGV_IDX = 3};
enum {SECS_TO_WAIT = 20};
enum {INPUT_LENGHT = 20};
enum {SUCCEED = 0, FAIL};

typedef struct thread_inputs
{
	char **args;
	pid_t pid;
}thread_inputs_ty;

pthread_t wd_thread = 0;

/******************************** ENUMS ***************************************/

/******************************** FUNCS ***************************************/

/*----------------------------------------------------------------------------*/

/***************************** FORWARDED FUNCS ********************************/
static char * ConvertToStr(size_t input);
static void* WdThreadFunc(void* args);


/******************************** API FUNC ************************************/


int MakeMeImmortal(char* argv[], int argc, size_t interval, size_t max_FAIL_num )
{
	/*assert*/	
	pthread_t thread;
	sigset_t set;
	int res = 0;
	char **args = NULL;
	size_t i = 0;
	pid_t  pid = 0;
	pthread_t wd_thread = 0;
	sem_t *sem = NULL;
    struct timespec ts;
	thread_inputs_ty *th_inputs = NULL;
	size_t j = 0;

	th_inputs = (thread_inputs_ty *)malloc(sizeof(thread_inputs_ty));
	if (NULL == th_inputs)
	{	
		fprintf(stderr, "Cannot allocate\n");
		return FAIL;		
	}

	/*set mask to ignore sigusr1*/
	res = sigemptyset(&set);
	
	if (0 != res)
	{
		fprintf(stderr, "err");
		exit(1);
	
	}	
	
	res = sigaddset(&set, SIGUSR1);
	
	if (0 != res)
	{
		fprintf(stderr, "err");
		exit(1);
	
	}
	
	res = sigaddset(&set, SIGUSR2);

	if (0 != res)
	{
		fprintf(stderr, "err");
		exit(1);
	
	}
		
	res = pthread_sigmask(SIG_BLOCK, &set, NULL);
	
	if (0 != res)
	{
		fprintf(stderr, "err");
		exit(1);
	}
	

	/*prepare customized argv[]...*/
	args = (char **)malloc(argc * sizeof(char *) + 4 * sizeof(char *));	
	if (NULL == args)
	{	
		fprintf(stderr, "Cannot allocate\n");
		return FAIL;		
	}

    args[WD_PATH] = PATH_WD_PROCESS;

    args[INTERVAL] = ConvertToStr(interval);
	/*RETURN IF NULL*/
    args[MAX_FAIL] = ConvertToStr(max_FAIL_num);
	/*RETURN IF NULL*/

	for(j = 0, i = FIRST_USER_ARGV_IDX; i < argc +  FIRST_USER_ARGV_IDX; ++i, ++j)
	{
		args[i] = argv[j];
	}

	args[i] = NULL;


	sem = sem_open("pSem", O_CREAT, 0777, 0); 

	if (clock_gettime(CLOCK_REALTIME, &ts) == -1)
	{
		perror("get time FAIL");
		return FAIL;
	}	

	ts.tv_sec += SECS_TO_WAIT;
	

	/*pid = fork*/
	pid = fork();
	
	/*handle FAIL (-1)*/
	if (-1 == pid)
	{
		perror("failed to create the child process");
		exit(1);
	}
	
	/*if (pid == 0)*/
	if(0 == pid)
	{
		/*execv watchdog using argv, argc, interval, max_FAIL_num , mask*/
		res = execv(PATH_WD_PROCESS, args); /*parent*/
		
		/*handle FAIL (-1)*/		
		if ( -1 == res)
		{			
			/*perror*/
			perror("failed to create the child process");
			exit(1);
		}
	}
	else
	{
		while ((res = sem_timedwait(sem, &ts)) == -1 && errno == EINTR)
		{	
			continue;
		}
		if (res == -1)
		{
    		if (errno == ETIMEDOUT)
			{
    			perror("sem_timedwait");
			}
			else
			{
    	 		perror("sem_timedwait");
			}			
		}
		th_inputs->pid = pid;
	}

 	if (-1 == sem_close(sem))
	{
		perror("sem");
		return FAIL;
	}

	argv[FIRST_USER_ARGV_IDX] = PATH_WD_PROCESS;
	/*sem_wait(sem)*/
	/*Create Thread - (ThreadFunc )*/
	res = pthread_create(&wd_thread, NULL, WdThreadFunc, (void*)th_inputs);
	
	/*hanlde error*/
	if(res != 0)
	{
		perror("failed to create the child process");
		exit(1);
	}

	/*return SUCCEED*/
	return SUCCEED;
}


int DoNotResuscitate()
{
	pid_t thread_pid = 0;
	pid_t process_pid = 0;
	int res = 0;

	thread_pid = getpid();
	process_pid = (pid_t)(size_t)atoi(getenv("WD_PID"));;

	res = kill(thread_pid, SIGUSR2);
	
	if (-1 == res)
	{
		perror("kill didn't work");
		return FAIL;
	}

	/*res = kill(process_pid, SIGUSR2);

	if (-1 == res)
	{
		perror("kill didn't work");
		return FAIL;
	}*/

	res = sem_unlink("pSem");

	if (-1 == res  &&  errno != ENOENT)
	{
		perror("FAIL in unlinking");
		return FAIL;
	}

	res = pthread_join(wd_thread, NULL);
	
	if(res != 0)
	{
		perror("failed to create the child process");
		exit(1);
	}

	return SUCCEED;
}


static void* WdThreadFunc(void* args)
{
	thread_inputs_ty *thread_inputs = (thread_inputs_ty *)args;
	KeepWatching(thread_inputs->args, thread_inputs->pid);
	free(thread_inputs);
}



static char * ConvertToStr(size_t input)
{
	char *result = (char *)malloc(sizeof(char) * INPUT_LENGHT);
	if (NULL == result)
	{	
		fprintf(stderr, "Cannot allocate\n");
		return NULL;
	}
	sprintf(result, "%ld", input);
	return result;
}






