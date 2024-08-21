/*Name: AhmadWattad
  Project: SortedList
  Reviewer: Sahar
  Date: 28/08/2023
  Version: 1.0: published the program
  		*/
#include <assert.h> /*assert*/
#include <stdlib.h> /*malloc*/
#include <string.h> /*memcpy*/

#include "pq.h"
#include "sorted_list.h"

/*************************** forwarded declarations ***************************/
static int ActionFuncSize(void* data, void* param);
static int MatchFuncIfSmallerOrEqual(const void *data, void *params);
static int MatchFuncForFIND(const void *data, void *params);

/***************************** Definitions ************************************/


struct pq
{
	sorted_list_t *list;
};

/******************************************************************************/
/******************************* API Funcs ************************************/
/******************************************************************************/

/************************** SORTEDLIST CREATE *********************************/


pq_t* PQCreate(pq_compare_t cmp_func)
{
	pq_t* pq_to_init = NULL;
	
	assert(NULL != cmp_func);

	pq_to_init = (pq_t*)malloc(sizeof(pq_t));
	
	if(NULL == pq_to_init )
	{
		return NULL;
	}
	
	pq_to_init->list = SortedListCreate(cmp_func);
	
	if (pq_to_init->list == NULL)
	{
		free(pq_to_init);
		pq_to_init = NULL;
		return NULL;
	}
	
	return pq_to_init;
}

void PQDestroy(pq_t *pq)
{
	assert(NULL != pq);
	
	SortedListDestroy(pq->list);
	free(pq);
	pq = NULL;
}

size_t PQSize(const pq_t *pq)
{
	assert(NULL != pq);
		
	return SortedListSize(pq->list);
}

int PQIsEmpty(const pq_t *pq)
{
	assert(NULL != pq);
		
	return SortedListIsEmpty(pq->list);

}


void* PQPeak(const pq_t *pq)
{
	assert(NULL != pq);
	
	return SortedListGetData(SortedListBegin(pq->list));


}

int PQEnqueue(pq_t *pq, void* data)
{
	int res = 0;
	
	assert(NULL != pq);
		
	sorted_list_iter_t iter = SortedListInsert(pq->list, data);
	
	res = SortedListIsSameIter(iter, SortedListEnd(pq->list));
	
	return !res;

}

void* PQDequeue(pq_t *pq)
{
	void* data = NULL;
	
	assert(NULL != pq);
	
	data = PQPeak(pq);
	SortedListPopFront(pq->list);

	return data;		
}

void PQClear(pq_t *pq)
{
	assert(NULL != pq);
	
	while(TRUE != PQIsEmpty(pq))
	{
		SortedListPopFront(pq->list);
	}
}

void* PQErase(pq_t *pq, void* data, pq_is_match_t match_func)
{
	void * data_to_save_before_remove = NULL; 
	int res = 0;
	
	assert(NULL != match_func);
	assert(NULL != data);
			
	sorted_list_iter_t iter = SortedListFindIF(SortedListBegin(pq->list), 
	 	SortedListEnd(pq->list), match_func, data);
	
	res = SortedListIsSameIter(iter, SortedListEnd(pq->list));	
	
	if (TRUE == res)
	{
		return NULL;
	} 
	
	data_to_save_before_remove = SortedListGetData(iter);
	SortedListRemove(iter);	

	return data_to_save_before_remove;
}





