/* Name: Ahmad Wattad
  Project: Work Sheet #task
  Reviewer: HRD32
  Date: 06/09/23
  Version: 1.0
  */
#include <assert.h> /*assert*/
#include <stdlib.h> /*malloc*/
#include <string.h> /*memcpy*/


#include "task.h"
#include "uid.h"

/*************************** forwarded declarations ***************************/
static int ActionFuncSize(void* data, void* param);
static int MatchFuncIfSmallerOrEqual(const void *data, void *params);
static int MatchFuncForFIND(const void *data, void *params);

/***************************** Definitions ************************************/


struct task
{
	uid_ty uid;
	time_t time_to_run;
	double interval_in_sec;
	task_operation_t op_func;
	task_cleanup_t cleanup_func;
	void* op_params;
	void* cleanup_params;
};

/******************************************************************************/
/******************************* API Funcs ************************************/
/******************************************************************************/

/************************** SORTEDLIST CREATE *********************************/


task_t* TaskCreate(time_t time_to_run, task_operation_t task_op , void* op_params,
                  task_cleanup_t clean_func, void* cleanup_params )
{
	task_t *new_task = NULL;
	
	assert(NULL != task_op);
	/*assert(NULL != clean_func);*/
		
	new_task = (task_t*)malloc(sizeof(task_t));
	
	if(NULL == new_task)
	{
		return NULL;
	}
	
	new_task->uid = UIDCreate();
	
	if(UIDIsSame(new_task->uid , bad_uid))
	{
		free(new_task);
		new_task = NULL;
		return NULL;
	}
	
	new_task->time_to_run = time_to_run;
	new_task->interval_in_sec = difftime(time_to_run, time(0));
	new_task->op_func = task_op;
	new_task->cleanup_func = clean_func;
	new_task->op_params = op_params;
	new_task->cleanup_params = cleanup_params;	
	
	return 	new_task;	
}

void TaskDestroy(task_t *task)
{
	assert(NULL != task);
	
	if(task->cleanup_func != NULL)
	{
		task->cleanup_func(task->cleanup_params);
	}
	
	free(task);
	task = NULL;
}


int TaskExecute(const task_t *task)
{
	assert(NULL != task);
	
	return task->op_func(task->op_params);
	
}

size_t TaskGetTimeToExecute(const task_t *task)
{
	assert(NULL != task);
	
	return task->time_to_run;
}

uid_ty TaskGetUID(const task_t *task)
{
	assert(NULL != task);
	
	return task->uid;
}

void* TaskUpdateTime(task_t *task_to_update)
{
	assert(NULL != task_to_update);
	
	task_to_update->time_to_run += task_to_update->interval_in_sec;
	
	return (void*)task_to_update->time_to_run;
}

int TaskIsMatch(task_t *task, uid_ty task_uid)
{
	assert(NULL != task);

	if(UIDIsSame(task->uid , task_uid))	
	{
		return TRUE;
	}
		return FALSE;
}

int TaskCompare(const task_t *task1,  task_t *task2)
{
	int res = 0;
	
	assert(NULL != task1);
	assert(NULL != task2);
		
	res = (int)difftime(task2->time_to_run, task1->time_to_run);
	
	if(0 == res)
	{
		return res;
	}
	if(0 < res)
	{
		return 1;
	}
	
	return -1;
	
}



