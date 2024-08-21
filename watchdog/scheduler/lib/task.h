/******************************************************************************/
/*
  Name: Ahmad Wattad
  Project: Work Sheet #task
  Reviewer: HRD32
  Date: 06/09/23
  Version: 1.0
  
  Overview: Implementation of a Task Manager.
*/

#ifndef __ILRD_TASK_H__
#define __ILRD_TASK_H__

/******************************** Libraries  **********************************/

#include <stddef.h> /*size_t*/
#include <sys/types.h> /*pid_t, time_t*/
#include "uid.h"  /*uid_t*/
#include "task.h"   

/******************************** Definitions  ********************************/

typedef struct task task_t;
typedef enum {DONT_RESCHEDULE = 0, RESCHEDULE} status_t;

typedef int (*task_operation_t)(void*);
typedef int (*task_is_match_t)(task_t *task, uid_ty task_uid);
typedef int (*task_compare_t)(const task_t *task1, task_t *task2);
typedef void (*task_cleanup_t)(void*);
/*********************** Task API Declerations  ****************************/

/*Function that creates a Task.
  Arguments:  'time_to_run' - time for execution
              'interval_in_sec' - time for rescheduling
              'op_func' - operation to conduct
              'cleanup_func' - clenaup operation for tasks that allocate memory
              'op_params' - parameters needed for task operation
              'cleanup_params' - parameters needed for cleanup
  Return value: pointer to newly generated Task. On Error, returns NULL.
  Complexity: O(1). 
  Undefined B.:*/
task_t* TaskCreate(time_t time_to_run, task_operation_t task_op, void* op_params,
                  task_cleanup_t clean_func, void* cleanup_params );

/*Function that destroys a Task.
        If Cleanup paramaters are available, they are freed.
  Arguments: 'task' - Task pointer.
  Return value: None.
  Complexity: O(cleanup function). 
  Undefined B.: NULL ptr of task */
void TaskDestroy(task_t *task);

/*Function that executes a task function
  Arguments: 'task' - Task pointer. wont be changes except if user function does
  Return value: status
  Complexity: O(func).
  Undefined B.: NULL pointer to Task. */
int TaskExecute(const task_t *task);

/*Function that retrieves the value of the tasks' execution time.
  Arguments: 'task' - Task pointer, 
  Return value: tasks' execution time
  Complexity: O(1).
  Undefined B.: NULL pointer to Task. */
size_t TaskGetTimeToExecute(const task_t *task);

/*Function that retrieves the UID of the task
  Arguments: 'task' - pointer to Task,
  Return value: task UID
  Complexity: O(1).
  Undefined B.: NULL pointer to Task. */
uid_ty TaskGetUID(const task_t *task);

/*Function that updates a task data
  Arguments: 'task' - pointer to Task,
  Return value: NA.
  Complexity: O(n).
  Undefined B.: NULL pointer to Task */
void* TaskUpdateTime(task_t *task_to_update);

/*Function that searches a math between to tasks
  Arguments: 'task' - pointer to Task, Task UID
  Return value: match result: TRUE, othewise = FALSE.
  Complexity: O(n).
  Undefined B.: NULL pointer to Task, NULL function.*/
int TaskIsMatch(task_t *task, uid_ty task_uid);

/*Function that compares tasks based on user logics
  Arguments: 'task1,2' - pointers to Tasks, matching logics
  Return value: match result: TRUE, othewise = FALSE.
  Complexity: O(n).
  Undefined B.: NULL pointer to Task, NULL function.*/
int TaskCompare(const task_t *task1, task_t *task2);

#endif /*__ILRD_TASK_H__*/
