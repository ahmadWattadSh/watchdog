/*Name: AhmadWattad
  Project: SortedList
  Reviewer: Sahar
  Date: 28/08/2023
  Version: 1.0: published the program
  		*/
#include <assert.h> /*assert*/
#include <stdlib.h> /*malloc*/
#include <string.h> /*memcpy*/

#include "sorted_list.h"

/*************************** forwarded declarations ***************************/
static int ActionFuncSize(void* data, void* param);
static int MatchFuncIfSmallerOrEqual(const void *data, void *params);
static int MatchFuncForFIND(const void *data, void *params);

/***************************** Definitions ************************************/


struct sorted_list 
{
	dll_ty *dlist;
	sorted_list_compare_t cmp_func;
};

typedef struct parameters
{
	void *data ;
	sorted_list_compare_t cmp_func_param;
}parameters_t;

/******************************************************************************/
/******************************* API Funcs ************************************/
/******************************************************************************/

/************************** SORTEDLIST CREATE *********************************/

sorted_list_t* SortedListCreate(sorted_list_compare_t CompareFunc)
{
	sorted_list_t *initalized_list = NULL;
	
	assert(NULL != CompareFunc);

	initalized_list = (sorted_list_t *)malloc(sizeof(sorted_list_t));
	
	if(NULL == initalized_list)
	{
		return NULL;
	}

	initalized_list->dlist = DListCreate();
	
	if(NULL == initalized_list->dlist)
	{ 
		return NULL;
	}	
	
	initalized_list->cmp_func = CompareFunc;
	
	return initalized_list;
}

/************************** SORTEDLIST DESTROY ********************************/

void SortedListDestroy(sorted_list_t *list)
{
	assert(NULL != list);
	
	DListDestroy(list->dlist);
	
	free(list);
}

/****************************** SORTEDLIST SIZE ********************************/

size_t SortedListSize(const sorted_list_t *list)
{
	assert(NULL != list);
	
	return DListSize(list->dlist);
}


/****************************** SORTEDLIST EMPTY ******************************/

int SortedListIsEmpty(sorted_list_t *list)
{
	assert(NULL != list);
	
	return DListIsEmpty(list->dlist);
}

/**************************** SORTEDLIST ISEMPTY ******************************/

void* SortedListGetData(sorted_list_iter_t iter)
{
	return DListGetData(iter.internal_iter);
}

/**************************** SORTEDLIST INSERT *******************************/

sorted_list_iter_t SortedListInsert(sorted_list_t *sorted_list, void* data)
{
	sorted_list_iter_t iter = {0};
	parameters_t *paramaters = NULL;
	
	assert(NULL != sorted_list);

	paramaters = (parameters_t*)malloc(sizeof(parameters_t));
	
	if(NULL == paramaters)
	{
		iter.internal_iter = DListEnd(sorted_list->dlist);
		return iter;
	}
	
	paramaters->data = data; 	
	paramaters->cmp_func_param = sorted_list->cmp_func; 		

	iter.internal_iter = DListFind(DListBegin(sorted_list->dlist), 
	DListEnd(sorted_list->dlist), (match_action_ty)MatchFuncIfSmallerOrEqual, (void*)paramaters);
	iter.internal_iter = DListInsertBefore(iter.internal_iter, data, sorted_list->dlist);
	
	#ifndef NDEBUG
	iter.list = sorted_list;
	#endif
	
	free(paramaters);
	paramaters = NULL;
	
	return iter;
}

/**************************** SORTEDLIST INSERT *******************************/

sorted_list_iter_t SortedListRemove(sorted_list_iter_t iter)
{
	sorted_list_iter_t iter_next = {0};
	
	iter_next.internal_iter = DListRemove(iter.internal_iter);
}

/**************************** SORTEDLIST INSERT *******************************/

sorted_list_iter_t SortedListPopFront(sorted_list_t *list)
{
	sorted_list_iter_t iter_next = {0};
	
	assert(NULL != list);
		
	iter_next.internal_iter = DListRemove(DListBegin(list->dlist));
}

/**************************** SORTEDLIST POPBACK ******************************/

sorted_list_iter_t SortedListPopBack(sorted_list_t *list)
{
	sorted_list_iter_t iter_next = {0};
	
	assert(NULL != list);
		
	iter_next.internal_iter = DListRemove(DListPrev(DListEnd(list->dlist)));
}

/**************************** SORTEDLIST FIND *********************************/

sorted_list_iter_t SortedListFind
(sorted_list_iter_t from, sorted_list_iter_t to, void* params, sorted_list_t *list)
{
	parameters_t *paramaters = NULL;
	sorted_list_iter_t iter = {0};
	
	assert(NULL != list);		
	assert(from.list == to.list);
	
	paramaters = (parameters_t*)malloc(sizeof(parameters_t));
	
	if(NULL == paramaters)
	{
		return to;
	}
	
	paramaters->data = params; 	
	paramaters->cmp_func_param = list->cmp_func; 	
	
	iter.internal_iter = DListFind(from.internal_iter, 
	to.internal_iter, (match_action_ty)MatchFuncForFIND, (void*)paramaters);
	
	free(paramaters);
	paramaters = NULL;	
	
	return iter;
}

/*************************** SORTEDLIST FOREACH *******************************/

int SortedListForEach
(sorted_list_iter_t from, sorted_list_iter_t to, sorted_list_action_t action_func, void* params)
{
	int res = 0;
	
	assert(from.list == to.list);	
	assert(NULL != action_func);	
		
	res = DListForEach(from.internal_iter, to.internal_iter, action_func, params);
	
	return res;
}

/*************************** SORTEDLIST FOREACH *******************************/

sorted_list_iter_t SortedListFindIF
(sorted_list_iter_t from, sorted_list_iter_t to, sorted_list_is_match_t conditional_func, void *params)
{
	assert(from.list == to.list);	
	assert(NULL != conditional_func);
	
	while (TRUE != SortedListIsSameIter(from, to))
	{
		if(TRUE == conditional_func(SortedListGetData(from),  params))		
		{
			return from;
		}

		from = SortedListNext(from);
	}	
	
	return to;
} 

/*************************** SORTEDLIST MERGE *********************************/

sorted_list_t* SortedListMerge(sorted_list_t *dest_list, sorted_list_t *src_list)
{
	sorted_list_iter_t src_from = {0};
	sorted_list_iter_t src_to = {0};	
	sorted_list_iter_t dest_from = {0};
	sorted_list_iter_t src_tail = {0};	
	parameters_t *paramaters = NULL;
	
	assert(NULL != src_list);
	assert(NULL != dest_list);
	
	paramaters = (parameters_t*)malloc(sizeof(parameters_t));
	
	if(NULL == paramaters)
	{
		return NULL;
	}

	paramaters->cmp_func_param = src_list->cmp_func; 	
	
	src_from.internal_iter = DListBegin(src_list->dlist);
	dest_from.internal_iter = DListBegin(dest_list->dlist);
	src_tail.internal_iter = DListEnd(src_list->dlist);
	
	while (TRUE !=  SortedListIsSameIter(src_from, src_tail))
	{
	
		paramaters->data = (int*)DListGetData(src_from.internal_iter);

		dest_from.internal_iter = DListFind(dest_from.internal_iter, 
			DListEnd(dest_list->dlist), 
			(match_action_ty)MatchFuncIfSmallerOrEqual, (void*)paramaters);

		paramaters->data = (int*)DListGetData(dest_from.internal_iter); 

		src_to.internal_iter = DListFind(src_from.internal_iter, 
	 	src_tail.internal_iter, (match_action_ty)MatchFuncIfSmallerOrEqual, 
	 		(void*)paramaters); 
	
		DListSplice(src_from.internal_iter, src_to.internal_iter,
			dest_from.internal_iter);
	
		src_from.internal_iter = src_to.internal_iter; 
	}
	
	free(paramaters);
	paramaters = NULL;
	
	return dest_list;
}	

/********************* Iterator API Declerations  ****************************/

int SortedListIsSameIter(sorted_list_iter_t iter, sorted_list_iter_t iter_to_compare)
{
	
	return DListIsSameIter(iter.internal_iter , iter_to_compare.internal_iter);
}


sorted_list_iter_t SortedListBegin(const sorted_list_t *list)
{
	sorted_list_iter_t iter = {0};
	
	assert(NULL != list);
	
	iter.internal_iter = DListBegin(list->dlist);
	
	return iter;
}


sorted_list_iter_t SortedListEnd(const sorted_list_t *list)
{
	sorted_list_iter_t iter = {0};
	
	assert(NULL != list);
	
	iter.internal_iter =DListEnd(list->dlist);
	
	return iter;		
}

sorted_list_iter_t SortedListNext(sorted_list_iter_t iter)
{
	sorted_list_iter_t iter_next = {0};

	iter_next.internal_iter = DListNext(iter.internal_iter);
	
	return iter_next;
}


sorted_list_iter_t SortedListPrev(sorted_list_iter_t iter)
{
	sorted_list_iter_t iter_prev = {0};

	iter_prev.internal_iter = DListPrev(iter.internal_iter);
	
	return iter_prev;	
}

/***************************  helper functions  *******************************/
static int MatchFuncIfSmallerOrEqual(const void *data_from_iters, void *params)
{	
	parameters_t *params_tmp = (parameters_t*)params;
	
	if(0 >= params_tmp->cmp_func_param(data_from_iters, params_tmp->data))
	{
		return TRUE;
	}
	
	return FALSE;
}

static int MatchFuncForFIND(const void *data_from_iters, void *params)
{	
	parameters_t *params_copy = (parameters_t*)params;
	
	if(0 == params_copy->cmp_func_param(data_from_iters, params_copy->data))
	{
		return TRUE;
	}
	
	return FALSE;
}


