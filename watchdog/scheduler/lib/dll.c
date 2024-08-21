/*Name: AhmadWattad
  Project: Work Sheet 11 - Data Structures - DLL
  Reviewer: Idan
  Date: 23/08/2023
  Version: 1.0: published the program
  		*/
#include <assert.h> /*assert*/
#include <stdlib.h> /*malloc*/

#include "dll.h"


/******************************* Definitions **********************************/


struct node
{
  void *data;
  node_ty *next;
  node_ty *prev;
};

struct dll
{
  node_ty head;
  node_ty tail;
};

/****************************** static functions ******************************/

static node_ty *ConvertToNodePtr(dll_iter_ty iter);
static dll_iter_ty ConvertToIter(node_ty * node_ptr);
static node_ty* CreateNode(node_ty *prev_node, node_ty *next_node , void *data);
static int ActionFuncSize(void* data, void* param);

/******************************************************************************/
/******************************* API Funcs ************************************/
/******************************************************************************/

/******************************* CBUFF-CREATE ********************************/

dll_ty *DListCreate(void)
{
	dll_ty *initalized_list = NULL;
	
	initalized_list = (dll_ty *)malloc(sizeof(dll_ty));
	
	if(NULL == initalized_list)
	{
		return (dll_ty*)0;
	}

	initalized_list->tail.next = NULL;	
	initalized_list->tail.prev = &(initalized_list->head);			
	
	initalized_list->head.next = &(initalized_list->tail);
	initalized_list->head.prev = NULL;	
	
	return initalized_list;
}

void DListDestroy(dll_ty *list)
{
	node_ty *current = NULL;
	node_ty *next = NULL;

	
	assert(NULL != list);
	
	current = DListBegin(list);
	
	while (current->next != NULL)
	{
		next = current->next;
		free(current);
		current = NULL;
		current = next;
	}
	
	free(list);
	list = NULL;
}


size_t DListSize(const dll_ty *list)
{
	size_t count  = 0;
	
	assert(NULL != list);
	
	DListForEach(DListBegin((dll_ty *)list), DListEnd((dll_ty *)list), 
		&ActionFuncSize, (void*)&count);

	return count;
}

static int ActionFuncSize(void* data, void* param)
{
	int *int_ptr = NULL;
	
	assert(NULL != param);

	int_ptr = param;
	
	data;
	++*int_ptr;
	
	return 0;
}

int DListIsEmpty(dll_ty *list)
{
	assert(NULL != list);
	
	if(NULL == list->head.next->next)
	{
		return TRUE;
	}
	
	return FALSE;
}


void* DListGetData(dll_iter_ty iter)
{
	node_ty *node = ConvertToNodePtr(iter);
	
	assert(NULL != node);
	
	return node->data;
}


void DListSetData(dll_iter_ty iter, const void *data)
{
	node_ty *node = ConvertToNodePtr(iter);
	
	assert(NULL != node);
	
	node->data = (void *)data;
}

dll_iter_ty DListInsertBefore(dll_iter_ty iter, void* data, dll_ty *dlist)
{
	node_ty *new_node = NULL;
	node_ty *prev = NULL; 
	node_ty *node = ConvertToNodePtr(iter);
	
	assert(NULL != dlist);
	assert(NULL != node);
	
	new_node = CreateNode(node->prev ,node , data);
	
	if(NULL == new_node)
	{
		return (dll_iter_ty)&dlist->tail;
	}

	prev = node->prev;
	prev->next = new_node;	
	node->prev = new_node;

	return ConvertToIter(new_node);

}

dll_iter_ty DListRemove(dll_iter_ty iter)
{
	node_ty *curr_node = ConvertToNodePtr(iter);
	node_ty *next_node = NULL;
	node_ty *prev_node = NULL;
	
	assert(NULL != curr_node);
	
	next_node = curr_node->next;
	prev_node = curr_node->prev;
	
	next_node->prev = curr_node->prev;
	prev_node->next = curr_node->next;
	
	free(curr_node);
	
	return next_node;
}


dll_iter_ty DListPushFront(dll_ty *list, void* data)
{
	assert(NULL != list);
	
	return DListInsertBefore(DListBegin(list), data , list);

}

dll_iter_ty DListPushBack(dll_ty *list, void* data)
{
	assert(NULL != list);
	
	return DListInsertBefore(DListEnd(list), data , list);
}


dll_iter_ty DListPopFront(dll_ty *list)
{
	assert(NULL != list);
	
	return DListRemove(DListBegin(list));
}


dll_iter_ty DListPopBack(dll_ty *list)
{
	assert(NULL != list);
	
	return DListRemove(ConvertToIter(DListEnd(list)->prev));
}


dll_iter_ty DListFind
(dll_iter_ty from, dll_iter_ty to, match_action_ty match_func, void* params)
{
	node_ty *node_from = NULL;
	node_ty *node_to = NULL;
	node_from = ConvertToNodePtr(from);
	node_to = ConvertToNodePtr(to);
		
	assert(NULL != match_func);	
	assert(NULL != node_from);
	assert(NULL != node_to);		
	
	while (node_from != node_to)
	{
		if(TRUE == match_func(node_from->data, params))		
		{
			return (dll_iter_ty)node_from;
		}
		
		node_from = node_from->next;
	}
		
	return node_from;
}

int DListForEach
(dll_iter_ty from, dll_iter_ty to, dll_action_ty action_func, void* params)
{
	node_ty *node_from = NULL;
	node_ty *node_to = NULL;
	node_from = ConvertToNodePtr(from);
	node_to = ConvertToNodePtr(to);
			
	assert(NULL != action_func);
	assert(NULL != node_from);
	assert(NULL != node_to);	

	
	while (node_from != node_to)
	{
		if(SUCCESS != action_func(node_from->data, params))		
		{
			return FAILURE;
		}
	
		node_from = node_from->next;
	}
	
	return 	SUCCESS;	

}


int DListMultiFind(dll_iter_ty from, dll_iter_ty to, match_action_ty match_func,
	void *param, dll_ty *dest_list)
{
	node_ty *node_from = NULL;
	node_ty *node_to = NULL;
	node_from = ConvertToNodePtr(from);
	node_to = ConvertToNodePtr(to);
			
	assert(NULL != match_func);
	assert(NULL != node_from);
	assert(NULL != node_to);	

	
	while (node_from != node_to)
	{
		if(TRUE == match_func(node_from->data, param))		
		{
			DListPushBack(dest_list, node_from->data);
		}
		
		node_from = node_from->next;
	}
	
	if(TRUE == DListIsEmpty(dest_list))
	{	
		return FAILURE;
	}
	
	return SUCCESS;
}

void DListSplice(dll_iter_ty from, dll_iter_ty to, dll_iter_ty where)
{
	node_ty *from_curr = ConvertToNodePtr(from);
	node_ty *to_curr = ConvertToNodePtr(to);
	node_ty *where_curr =ConvertToNodePtr(where);	
	node_ty *from_prev = NULL;
	node_ty *to_prev = NULL;
	node_ty *where_prev = NULL;
	
	assert(NULL != from_curr);
	assert(NULL != to_curr);	
	assert(NULL != where_curr);	
		
	from_prev = from_curr->prev;
	to_prev = to_curr->prev;		
		
	from_prev->next = to;	
	to->prev = from_prev;
	
	to_prev->next = where_curr;
	from->prev = where_curr->prev;
	
	where_prev = where_curr->prev;
	where_prev->next = from;
	where_curr->prev = to->prev;

}	

/********************* Iterator API Declerations  ****************************/

int DListIsSameIter(dll_iter_ty iter, dll_iter_ty iter_to_compare)
{
	node_ty *node = ConvertToNodePtr(iter);
	node_ty *node_to_compare = ConvertToNodePtr(iter_to_compare);	
	
	assert(NULL != node);
	assert(NULL != node_to_compare);	
	
	if(node == node_to_compare)
	{
		return TRUE;
	}
	
	return FALSE;
}


dll_iter_ty DListBegin(const dll_ty *list)
{
	assert(NULL != list);
	
	return ConvertToIter(((dll_ty *)list)->head.next);
}


dll_iter_ty DListEnd(const dll_ty *list)
{
	assert(NULL != list);
		
	return ConvertToIter(&(((dll_ty *)list)->tail));	
}

dll_iter_ty DListNext(dll_iter_ty iter)
{
	node_ty *node =  ConvertToNodePtr(iter);
	
	assert(NULL != node);
	
	node = node->next;
	
	return  ConvertToIter(node);
}


dll_iter_ty DListPrev(dll_iter_ty iter)
{
	node_ty *node =  ConvertToNodePtr(iter);
	
	assert(NULL != node);
	
	node = node->prev;
	
	return ConvertToIter(node); 
}


static node_ty* CreateNode(node_ty *prev_node, node_ty *next_node , void *data)
{
	node_ty *new_node = NULL;
	
	assert(NULL != prev_node);
	assert(NULL != next_node);
	
	new_node = (node_ty*)malloc(sizeof(node_ty));
	
	if(NULL == new_node)
	{
		return (node_ty*)0;
	}
	
	new_node->data = data;
	new_node->next = next_node;
	new_node->prev = prev_node;
			
	return new_node;
}

static node_ty *ConvertToNodePtr(dll_iter_ty iter) 
{
	return (node_ty*)iter;
}

static dll_iter_ty ConvertToIter(node_ty * node_ptr) 
{
	
	return (dll_iter_ty)node_ptr;
}





