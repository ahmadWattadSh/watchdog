/******************************************************************************/
/*
  Name:Sahar Attia
  Project: Schedule project  
  Reviewer: Idan
  Date: 06/09/2023
*/
/******************************************************************************/
#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include<unistd.h>

#include "uid.h"
#include "task.h"
#include "pq.h"
#include "sched.h"

struct sched
{
    pq_t *pq;
    int is_running;
    int is_self_terminate;
    task_t *curr_task;    
};

/**********************************define**************************************/

#define ErrorMsg(expected, NULLult, str) \
		{\
		    if (expected != NULLult)\
		    {\
		    	printf("%s has failed\n", str);\
		    }\
		}
	
#define ARRAYSIZE (7)	

typedef struct scheduid
{
	uid_ty uid;
	sched_t *sched;	
}scheduid;

/**************************helper functions************************************/


static int Operation1(void *data);
static int Operation2(void* data);
static void Cleanup(void *data);
static int OperationTrue(void* data);
static int OperationFalse(void* data);
static int OperationStop(void* data);
static int OperationRemove(void* data);
static int OperationClear(void* data);
static void CreateUidSched(scheduid *sched_uid, uid_ty uid);
static void DestroyUidSched(void *sched_uid);

/******************************************************************************/

static void MainTestFunc(void);

static void TestSchedCreate(void);
static void TestAddTask(void);
static void TestRemoveTask(void);
static void TestSchedRun(void);
static void TestSchedStop(void);
static void TestSchedSize(void);
static void TestSchedIsEmpty(void);
static void TestSchedClear(void);

/******************************************************************************/

int main()
{
	MainTestFunc();
	return 0;
}

/**************************** test program ************************************/

static void MainTestFunc()
{
	TestSchedCreate();
	TestAddTask();
	TestRemoveTask();
	TestSchedRun();
	TestSchedStop();
	TestSchedSize();
	TestSchedIsEmpty();
	TestSchedClear();
}

/******************************************************************************/

static void TestSchedCreate()
{
	sched_t *sched = SchedCreate();
	
	ErrorMsg(0, SchedSize(sched), "TestSchedCreate()");
	
	SchedDestroy(sched);
	
	printf("TestSchedCreate() tested\n");
}

/******************************************************************************/

static void TestAddTask()
{
	int x1 = 11; 
	int x2 = 21; 

	sched_t *sched = SchedCreate();
	
	AddTask(sched, Operation1, &x1, time(0)+5, Cleanup, NULL);

	ErrorMsg(1, SchedSize(sched), "TestAddTask()");
	
	AddTask(sched, Operation2, &x2, time(0)+10, Cleanup, NULL);
	
	ErrorMsg(2, SchedSize(sched), "TestAddTask()");
	
/* for playing around!		
	SchedRun(sched);
*/		
	SchedDestroy(sched);

	printf("TestAddTask() tested\n");
}

/******************************************************************************/

static void TestRemoveTask()
{
	int x1 = 11; 
	int x2 = 21; 

	sched_t *sched = SchedCreate();
	uid_ty uid2 = {0};
	uid_ty uid1 = {0};

	uid1 = AddTask(sched, Operation1, &x1, time(0)+20, Cleanup, NULL);
	
	ErrorMsg(1, SchedSize(sched), "TestRemoveTask()");
	
	uid2 = AddTask(sched, Operation1, &x2, time(0)+40, Cleanup, NULL);
	
	ErrorMsg(2, SchedSize(sched), "TestRemoveTask()");
		
	RemoveTask(sched, uid2);
	
	ErrorMsg(1, SchedSize(sched), "TestRemoveTask()");
	
	RemoveTask(sched, uid1);
	
	ErrorMsg(0, SchedSize(sched), "TestRemoveTask()");
	
	
	SchedDestroy(sched);
	printf("TestRemoveTask() tested\n");
}

/******************************************************************************/

static void TestSchedRun(void)
{
	int x1 = 21; 
	int x2 = 31; 
	int x3 = 41;

	uid_ty uid = {0};

	sched_t *sched = SchedCreate();
	
	scheduid *sched_uid = (scheduid*)malloc(sizeof(scheduid));
	sched_uid->sched = sched;
	
	uid = AddTask(sched, OperationRemove, sched_uid, time(0)+10, DestroyUidSched, sched_uid);
	CreateUidSched(sched_uid, uid);
	
	AddTask(sched, OperationClear, sched, time(0)+20, Cleanup, NULL);
	AddTask(sched, OperationFalse, &x1, time(0)+5, Cleanup, NULL);
	AddTask(sched, OperationFalse, &x2, time(0)+15, Cleanup, NULL);
	AddTask(sched, OperationFalse, &x3, time(0)+25, Cleanup, NULL);

	
	ErrorMsg(SUCCESS, SchedRun(sched), "TestSchedRun()");
	SchedDestroy(sched);
	
	printf("TestSchedRun() tested\n");
}

/******************************************************************************/

static void TestSchedStop(void)
{
	int x1 = 11; 
	int x2 = 21; 
	int x3 = 41;
	

	sched_t *sched = SchedCreate();
	
	AddTask(sched, OperationTrue, &x1, time(0)+5, NULL, NULL);
	AddTask(sched, OperationFalse, &x2, time(0)+10, Cleanup, NULL);
	AddTask(sched, OperationFalse, &x3, time(0)+25, Cleanup, NULL);
	AddTask(sched, OperationStop, sched, time(0)+15, Cleanup, NULL);

	
	ErrorMsg(STOPPED, SchedRun(sched), "TestSchedStop()");
	
	SchedDestroy(sched);
	printf("TestSchedStop() tested\n");
}

/******************************************************************************/

static void TestSchedSize()
{
	int x1 = 11; 
	int x2 = 21; 

	sched_t *sched = SchedCreate();
	uid_ty uid2 = {0};
	uid_ty uid1 = {0};
	
	uid1 = AddTask(sched, Operation1, &x1, time(0)+20, Cleanup, NULL);
	
	ErrorMsg(1, SchedSize(sched), "TestSchedSize()");
	
	uid2 = AddTask(sched, Operation1, &x2, time(0)+40, Cleanup, NULL);
	
	ErrorMsg(2, SchedSize(sched), "TestSchedSize()");
	
	RemoveTask(sched, uid2);
	
	ErrorMsg(1, SchedSize(sched), "TestSchedSize()");
	
	
	SchedDestroy(sched);
	printf("TestSchedSize() tested\n");
}

/******************************************************************************/

static void TestSchedIsEmpty()
{
	int x1 = 11; 
	int x2 = 21; 

	sched_t *sched = SchedCreate();
	uid_ty uid2 = {0};
	uid_ty uid1 = {0};
	
	ErrorMsg(TRUE, SchedIsEmpty(sched), "TestSchedIsEmpty()");
	
	uid1 = AddTask(sched, Operation1, &x1, time(0)+20, Cleanup, NULL);
	
	ErrorMsg(FALSE, SchedIsEmpty(sched), "TestSchedIsEmpty()");
	
	uid2 = AddTask(sched, Operation1, &x2, time(0)+40, Cleanup, NULL);
	
	ErrorMsg(FALSE, SchedIsEmpty(sched), "TestSchedIsEmpty()");
	
	RemoveTask(sched, uid2);
	
	ErrorMsg(FALSE, SchedIsEmpty(sched), "TestSchedIsEmpty()");
	
	RemoveTask(sched, uid1);
	
	ErrorMsg(TRUE, SchedIsEmpty(sched), "TestSchedIsEmpty()");
	
	SchedDestroy(sched);
	printf("TestSchedIsEmpty() tested\n");
}

/******************************************************************************/

static void TestSchedClear()
{
	int x1 = 11; 
	int x2 = 21; 

	sched_t *sched = SchedCreate();
	
	AddTask(sched, Operation1, &x1, time(0)+20, Cleanup, NULL);
	AddTask(sched, Operation1, &x2, time(0)+40, Cleanup, NULL);
	AddTask(sched, Operation1, &x1, time(0)+20, Cleanup, NULL);
	AddTask(sched, Operation1, &x2, time(0)+40, Cleanup, NULL);
	SchedClear(sched);
	
	ErrorMsg(TRUE, SchedIsEmpty(sched), "TestSchedClear()");
	
	SchedDestroy(sched);
	printf("TestSchedClear() tested\n");
}

/******************************************************************************/

static int Operation1(void* data)
{
	printf("happy to be alive\n");
	return RESCHEDULE;
}

static int Operation2(void* data)
{
	printf("love to be dead\n");
	return RESCHEDULE;
}

/******************************************************************************/

static int OperationTrue(void* data)
{
	static size_t i = 0;
	fprintf(stdout, "data = %d\n", *((int*)data));
	++i;
	if(i > 3)
	{
		return FALSE;
	}
	return TRUE;
}

/******************************************************************************/

static int OperationFalse(void* data)
{
	printf("data = %d\n", *((int*)data));

	return FALSE;
}

/******************************************************************************/

static int OperationRemove(void* data)
{
	scheduid *sched_uid = (scheduid*)data;
	RemoveTask(sched_uid->sched, sched_uid->uid);
	
	return TRUE;
}

/******************************************************************************/

static int OperationClear(void* data)
{
	SchedClear((sched_t*)data);
	
	return FALSE;
}

/******************************************************************************/

static void CreateUidSched(scheduid *sched_uid, uid_ty uid)
{
	sched_uid->uid = uid;
}

/******************************************************************************/

static void DestroyUidSched(void *sched_uid)
{
	free((scheduid*)sched_uid);
	sched_uid = NULL;

}

/******************************************************************************/

static int OperationStop(void* data)
{
	SchedStop((sched_t*)data);
	return FALSE;
}

/******************************************************************************/

static void Cleanup(void *data)
{

}
