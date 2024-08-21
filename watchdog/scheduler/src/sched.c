/*Name: AhmadWattad
  Project: SortedList
  Reviewer: Uri
  Date: 12/09/2023
  Version: 1.0: published the program
  		*/
#include <assert.h> /*assert*/
#include <stdlib.h> /*malloc*/
#include <string.h> /*memcpy*/
#include<unistd.h> /*time_t*/


#include "pq.h"
#include "task.h"
#include "uid.h"
#include "sched.h"

/*************************** forwarded declarations ***************************/
static int SchedCmp(const void* task , void* task_to_cmp );
static void SleepPerTask(double secs);
static int MatchFuncForRemove(const void* data1 , void* data2);
static int RescheduleOrDestroy(sched_t *sched, int is_reschedule);

/***************************** Definitions ************************************/



struct sched
{
    pq_t *pq;
    int is_running;
    int is_self_terminate;
    task_t *curr_task;    
};


/******************************************************************************/
/******************************* API Funcs ************************************/
/******************************************************************************/


sched_t *SchedCreate(void)
{
	sched_t *new_sched = (sched_t*)malloc(sizeof(sched_t));
	
	if(NULL == new_sched)
	{
		return NULL;
	}
	
	new_sched->pq = PQCreate(SchedCmp);
	
	if(NULL == new_sched->pq)
	{
		free(new_sched);
		new_sched = NULL; 
		return NULL;
	}
	
	new_sched->is_running = FALSE;
	new_sched->is_self_terminate = FALSE;
	new_sched->curr_task = NULL;
	
	return new_sched;
}

/******************************************************************************/

void SchedDestroy(sched_t *sched)
{
	assert(NULL != sched);
	
	SchedClear(sched);

	PQDestroy(sched->pq);
	
	free(sched);
	sched = NULL;
}

/******************************************************************************/
uid_ty AddTask(sched_t *sched, sched_operation_func_t sched_func, 
	void *op_param ,time_t time_to_run, sched_clean_func_t clean_func, 
		void *clean_params)
{
	int res = 0;
	
	assert(NULL != sched);
	assert(NULL != sched_func);
	

	task_t *new_task = TaskCreate(time_to_run, sched_func, op_param, clean_func, clean_params);
	
	if( NULL == new_task)
	{
		return bad_uid;
	}
	
	res = PQEnqueue(sched->pq, new_task);
	
	if(SUCCESS != res) /*TODO: check this one*/
	{
		return TaskGetUID(new_task);
	}
	
	return bad_uid;	


}

/******************************************************************************/
void RemoveTask(sched_t *sched, uid_ty uid)
{

	task_t *task_to_remove = NULL;
	assert(NULL != sched);	

	if(sched->curr_task != NULL && TRUE == TaskIsMatch(sched->curr_task ,uid))
	{
		sched->is_self_terminate = TRUE;
	}
	else
	{
		task_to_remove = (task_t*)PQErase(sched->pq, &uid ,MatchFuncForRemove);
		assert(NULL != task_to_remove);
 		TaskDestroy(task_to_remove);		
	}

}

/******************************************************************************/
static int MatchFuncForRemove(const void* data1 , void* data2)
{
	return TaskIsMatch((task_t*)data1, *(uid_ty*)data2);
}

/******************************************************************************/
int SchedRun(sched_t *sched)
{
	assert(NULL != sched);
	
	int is_reschedule = SUCCESS;

	sched->is_running = TRUE;

	while (TRUE != PQIsEmpty(sched->pq) && FALSE != sched->is_running)
	{

	sched->curr_task = (task_t*)PQDequeue(sched->pq);
	/*TODO: wait till execution*/
	SleepPerTask(difftime(TaskGetTimeToExecute(sched->curr_task),time(0)));
	
	is_reschedule = TaskExecute(sched->curr_task); /*might change is_running*/

	if (FAILURE == 	RescheduleOrDestroy(sched, is_reschedule))
	{
		return FAILURE;	
	}
	
	}
	
	if(sched->is_running == FALSE)
	{
		return STOPPED;
	}
	
	sched->is_running = FALSE;	
	
	return SUCCESS;
	
}
/******************************************************************************/
static int RescheduleOrDestroy(sched_t *sched, int is_reschedule)
{
	/*TODO: && instead of ||*/
	/*destroy*/
	if(TRUE == sched->is_self_terminate || FALSE == sched->is_running ||
		 FALSE == is_reschedule)
	{
		sched->is_self_terminate = FALSE;
 		TaskDestroy(sched->curr_task);
		
	}/*reschedule*/
	else
	{
		TaskUpdateTime(sched->curr_task);
		
		if (FAILURE == PQEnqueue(sched->pq, sched->curr_task))
		{
			return FAILURE;
		}
		
	}
	
	return SUCCESS;
}
/******************************************************************************/
static void SleepPerTask(double secs)
{
	double time_elapsed = secs;
	
 	while(0 != time_elapsed)
 	{
		time_elapsed = sleep(time_elapsed);
	}
}

/******************************************************************************/
void SchedStop(sched_t *sched)
{
	assert(NULL != sched);	

	sched->is_running = FALSE;
}

/******************************************************************************/
size_t SchedSize(const sched_t *sched)
{
	assert(NULL != sched); /*TODO*/
	
	return PQSize(sched->pq) + sched->is_running;
}

/******************************************************************************/
int SchedIsEmpty(const sched_t *sched)
{
	assert(NULL != sched);
	
	return PQIsEmpty(sched->pq) && sched->is_running;
}

/******************************************************************************/
void SchedClear(sched_t *sched)
{
	assert(NULL != sched);		
	
	while (FALSE == PQIsEmpty(sched->pq))
	{
    		TaskDestroy((task_t*)(PQDequeue(sched->pq)));
	}
}

/******************************************************************************/
static int SchedCmp(const void* task , void* task_to_cmp )
{
	return TaskCompare((task_t*)task, (task_t*)task_to_cmp) ;

}
