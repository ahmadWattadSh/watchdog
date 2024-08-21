/*Name: Ahmad Wattad
  Project: calculator using fsm machine
  Reviewer: ??
  Date: 03/10/2023
*/
#include <stddef.h> /*size_t*/
#include <assert.h> /*assert*/
#include <stdatomic.h>
#include <signal.h>
#include <stdio.h> 
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

#include "sched.h"
#include "uid.h"
/******************************** ENUMS ***************************************/


enum {WD_PATH = 0, INTERVAL = 1, MAX_FAILURE = 2, FIRST_USER_ARGV_IDX = 3};

/******************************** FUNCS ***************************************/
atomic_size_t g_counter = 0; 

atomic_int g_end_program = 0;
  
/*----------------------------------------------------------------------------*/

typedef struct infos
{
	char **args;
	sched_t * sched;
	pid_t pid;
}ts2_info_ty;

/***************************** FORWARDED FUNCS ********************************/

static int CheckCounter(void *data);
static int SendSignal(void *data);


/******************************** API FUNC ************************************/

void SignalHandler1(int signum);
void SignalHandler2(int signum);

int KeepWatching(char *argv[], pid_t pid)
{
	struct sigaction sa1;
	struct sigaction sa2;
	sigset_t set;		
	sched_t *sched = NULL;
	int res = 0;
	uid_ty uid = {0};
	time_t interval = 0;
	ts2_info_ty infos = {0};
	size_t i = 0;
	
	/*set counter = 0*/
	__atomic_store_n(&g_counter, 0,__ATOMIC_SEQ_CST);

	/*register 2 signal handlers*/
	sa1.sa_handler = SignalHandler1;
	sa2.sa_handler = SignalHandler2;
	
	res = sigemptyset(&set);
	
	if (0 != res)
	{
		fprintf(stderr, "err");
		exit(1);
	
	}	

	/*register siguesr1, register sgiuser2*/	
	res = sigaction(SIGUSR1, &sa1, NULL);
	
	if (-1 == res)
	{
		fprintf(stderr, "err");
		return FAILURE;
	}	
		
	res = sigaction(SIGUSR2, &sa2, NULL);	
	
	if (-1 == res)
	{
		fprintf(stderr, "err");
		return FAILURE;
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
	
	res = pthread_sigmask(SIG_UNBLOCK, &set, NULL);
	
	if (0 != res)
	{
		fprintf(stderr, "err");
		exit(1);
	}	

	/*CreateSched*/
	sched = SchedCreate();
	if (NULL == sched)
	{	
		fprintf(stderr,"Failed to create sched\n");
		return FAILURE;
	}
	
	interval = time(0) + (size_t)atoi(argv[INTERVAL]);

	/*add task1 - check counter*/
	uid = AddTask(sched, SendSignal, (void *)(size_t)pid , interval, NULL, NULL);
	
	if(1 == UIDIsSame(uid, bad_uid))
	{
		fprintf(stderr, "cannot generate uid for task2");
		return FAILURE;	
	}

	
	infos.args = argv;
	infos.sched = sched;
	infos.pid = pid;

	/*add task2 - */
	uid = AddTask(sched, CheckCounter, (void *)&infos, interval, NULL, NULL);
	
	if(1 == UIDIsSame(uid, bad_uid))
	{
		fprintf(stderr, "cannot generate uid for task2");
		return FAILURE;	
	}
	
	/*run task*/
	res = SchedRun(sched);


	if (FAILURE == res)
	{
		fprintf(stderr, "err in sched");
		return FAILURE;
	}


	SchedDestroy(sched);

	for (i = INTERVAL; i < FIRST_USER_ARGV_IDX; ++i)
	{
		free(argv[i]);
	}

	free(argv);

}


static int SendSignal(void *data)
{	
	pid_t pid = (pid_t)(size_t)data;
	/*send sigusr 1*/
	kill(pid, SIGUSR1);

	__atomic_fetch_add(&g_counter, 1, __ATOMIC_SEQ_CST);
	
	return 1;
}

/*TODO*/
static int CheckCounter(void *data)
{
	ts2_info_ty *infos = (ts2_info_ty *)data;
   	void **task_inputs = NULL;
	pid_t resus_pid = 0;
	
    /*check counter*/
	if(g_end_program)
    {
        printf("the pid that will receeive the signal %d\n", infos->pid);
		/*I am the parent*/
        if(getppid() != infos->pid)
        {
            kill(infos->pid, SIGUSR2);
            waitpid(infos->pid, 0, 0);
        }
        /*sched stop*/
        SchedStop(infos->sched);
        return FALSE;
    }

    /*if the counter > max_failures_num  */
    if(g_counter >= atoi(infos->args[MAX_FAILURE]))
    {
        /* kill the user_process/wd_process */
        kill(infos->pid, SIGTERM);

		/*parent*/
		if (getppid() != infos->pid)
        {
			resus_pid = fork();
			
			if(-1 == resus_pid)
        	{
            	perror("fork failed, can't resusiate the program\n");
            	return FAILURE;
        	}
			
			if(0 == resus_pid)
			{
				execv(infos->args[WD_PATH],infos->args);
			}
			else
			{
          		infos->pid = resus_pid;
			}
		}
		else
		{
			execv(infos->args[FIRST_USER_ARGV_IDX],&infos->args[FIRST_USER_ARGV_IDX]);

		}
    
       	__atomic_store_n(&g_counter, 0,__ATOMIC_SEQ_CST); 
   }   

	return 1;
}

void SignalHandler1(int signum)
{
	assert(SIGUSR1 == signum);
	/*reset the counter*/
	__atomic_store_n(&g_counter, 0,__ATOMIC_SEQ_CST);

}

void SignalHandler2(int signum)
{
	assert(SIGUSR2 == signum);
	g_end_program = 1;
}



