/******************************************************************************/
/*
  Name: Ahmad Wattad
  Project: Work Sheet #pq
  Reviewer: HRD32
  Date: 06/09/23
  Version: 1.0
  
  Overview: Implementation of an PQ generator.
*/

#ifndef __ILRD_PQ_H__
#define __ILRD_PQ_H__

/******************************** Libraries  **********************************/

#include <stddef.h> /*size_t*/
#include <sys/types.h> /*pid_t, time_t*/
#include "pq.h"   

/******************************** Definitions  ********************************/

typedef struct pq pq_t;

typedef int (*pq_is_match_t)(const void* , void* );
typedef int (*pq_compare_t)(const void* , void* );

/*********************** PQ API Declerations  ****************************/

/*Function that creates a PQ.`
  Arguments: a comparison function .
  Return value: pointer to PQ. On Error, it will return NULL.
  Complexity: O(1). 
  Undefined B.:*/
pq_t* PQCreate(pq_compare_t cmp_func);

/*Function that destroys a PQ.
  Arguments: 'pq' - PQ pointer.
  Return value: None.
  Complexity: O(n). 
  Undefined B.: NULL ptr of pq */
void PQDestroy(pq_t *pq);

/*Function that returns the number of current elements in the PQ.
  Arguments: 'pq' - PQ pointer.
  Return value: Num of current elements in pq.
  Complexity: O(n).
  Undefined B.: NULL pointer to PQ. */
size_t PQSize(const pq_t *pq);

/*Function that checks if PQ is empty.
  Arguments: 'pq' - PQ pointer.
  Return value: if empty = 1, othewise = 0.
  Complexity: O(1).
  Undefined B.: NULL pointer to PQ. */
int PQIsEmpty(const pq_t *pq);

/*Function that retrieves the value of the PQ's head element.
  Arguments: 'pq' - PQ pointer.
  Return value: data - void* pointer.
  Complexity: O(1).
  Undefined B.: NULL pointer to PQ. */
void* PQPeak(const pq_t *pq);

/*Function that inserts a new element in the list without disrupting the sorting.
  Arguments: 'pq' - pointer to PQ,
             'data' - data to be set as the element's new value.
  Return value: int = SUCCESS, on error = FAILURE.
                On Error, returns invalid pointer to PQ. 
  Complexity: O(n).
  Undefined B.: NULL pointer to data, NULL pointer to pq*/
int PQEnqueue(pq_t *pq, void* data);

/*Function that removes PQ's head
  Arguments: 'pq' - pointer to PQ,
  Return value: data of the dequeud element.
  Complexity: O(1).
  Undefined B.: NULL pointer to PQ */
void* PQDequeue(pq_t *pq);

/*Function that removes all of PQ's elements
  Arguments: 'pq' - pointer to PQ,
  Return value: void.
  Complexity: O(n).
  Undefined B.: NULL pointer to PQ */
void PQClear(pq_t *pq);

/*Function that removes a PQ element, based on a match function.
  Arguments: data - the data of the elment to be deleted,
  	     match_func - a match function provided by the user,
  	     params - additional information for the match function 
  Return value: data of the erased element.
  Complexity: O(n).
  Undefined B.: invalid pointer to data, invalid pointer to match func */
void* PQErase(pq_t *pq, void* data, pq_is_match_t match_func);

#endif /*__ILRD_PQ_H__*/
