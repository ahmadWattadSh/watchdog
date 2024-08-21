/******************************************************************************/
/*
  Name:Ahmad Wattad
  Project: Sched   
  Reviewer: ???
  Date: 07/09/2023
*/
/******************************************************************************/

#ifndef __ILRD_SCHEDULER_H__
#define __ILRD_SCHEDULER_H__
/******************************** Libraries  **********************************/

#include <stddef.h> /*size_t*/

/*
struct sched
{
    pq_t *pq;
    int is_running;
    int is_self_terminate;
    task_t *curr_task;    
};
*/

/******************************** Definitions  ********************************/

typedef struct sched sched_t;

typedef enum {SUCCESS = 0, STOPPED , FAILURE} run_status_t;

typedef int (*sched_operation_func_t)(void*);
typedef void (*sched_clean_func_t)(void*);
/*********************** Queue API Declerations  ******************************/

/*Function that creates a scheduler.
  
  Arguments: void.
  
  Return value: pointer to scheduler.
  
  Complexity: O(1). 
  
  Undefined B.:*/
sched_t *SchedCreate(void);

/*Function that destroys a scheduler.

  Arguments: 'sched' - pointer to scheduler.

  Return value: None.

  Complexity: O(n). 

  Undefined B.: NULL ptr of scheduler */
void SchedDestroy(sched_t *sched);

/*Function that adds a task to the scheduler. The return value of 
	sched_func is responsible for rescheduling. if it returns TRUE, 
	it will reschedule, otherwise, it will not.

  Arguments: 'sched' - pointer to scheduler.
  	     'sched_func'- function provided by user for task execution. 
  	     'op_param' - parameters for sched_func.
  	     'time_to_run' - specifies the time in which the task will run 
  	     	since 1970 . 
  	     'clean_func' - a clean function provided by user to clear allocations 
  	     'clean_params' - parameters for clean_func

  Return value: None.

  Complexity: O(n).

  Undefined B.: NULL ptr of scheduler */
uid_ty AddTask(sched_t *sched, sched_operation_func_t operation_func, 
	void *op_param ,time_t time_to_run, sched_clean_func_t clean_func, 
		void *clean_params);

/*Function that removes a task from the scheduler. also, it will handle a task 
	which removes itself.

  Arguments: 'sched' - pointer to scheduler,
	     'uid' - unique identifier for the task.
	     
  Return value: NULL.

  Complexity: O(1).

  Undefined B.: NULL pointer to scheduler, BAD_UID*/
void RemoveTask(sched_t *sched, uid_ty uid);

/*Function that runs all the tasks in the scheduler in the specified time 
	and will auto resechedule if the user specified that in a task function.
	This function could be stopped by using a task which uses the stop function.

  Arguments: 'sched' - pointer to scheduler,
	     
  Return value: if it runs all the tasks it will return SUCCESS, if it is stopped, it will return STOPPED, otherwise, it will return FAILURE.

  Complexity: O(n).

  Undefined B.: NULL pointer to scheduler*/
int SchedRun(sched_t *sched);

/*Function that stops the scheduler's operation. could be included in a task.

  Arguments: 'sched' - pointer to scheduler,
	     
  Return value: void

  Complexity: O(1).

  Undefined B.: NULL pointer to scheduler, a task that executes destroy*/
void SchedStop(sched_t *sched);

/*Function that returns size of the scheduler.

  Arguments: 'sched' - pointer to scheduler,
	     
  Return value: size of scheduler;

  Complexity: O(n).

  Undefined B.: NULL pointer to scheduler*/
size_t SchedSize(const sched_t *sched);

/*Function that returns if scheduler is empty.

  Arguments: 'sched' - pointer to scheduler.
	     
  Return value: it will return TRUE if empty, otherwise, it will return FALSE ;

  Complexity: O(1).

  Undefined B.: NULL pointer to scheduler*/
int SchedIsEmpty(const sched_t *sched);

/*Function that clear all tasks in the scheduler which will render the 
	scheduler empty (note: it will not destroy the scheduler).

  Arguments: 'sched' - pointer to scheduler.
	     
  Return value: void.

  Complexity: O(n).

  Undefined B.: NULL pointer to scheduler*/
void SchedClear(sched_t *sched);


#endif /*(__ILRD_SCHEDULER_H__)*/
