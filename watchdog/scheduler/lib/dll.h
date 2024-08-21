/******************************************************************************/
/*
  Name: AhmadWattad
  Project: Work Sheet 11 - Data Structures - DLL
  Reviewer: Idan
  Date: 22/08/23
  Version: 1.0
*/

#ifndef __ILRD_DLL_H__
#define __ILRD_DLL_H__

/*overview: 
This product is a data structure that consists of a set of sequentially linked 
records, each of these records has two links, one to its previous and one to its next.
The functions in this api can perform operations on such data structure. 
  */
/******************************** Libraries  **********************************/

#include <stddef.h> /*size_t*/

/******************************** Definitions  ******************************/

typedef struct node node_ty;
typedef node_ty* dll_iter_ty;
typedef struct dll dll_ty;

typedef enum  {SUCCESS = 0 ,FAILURE} status_ty;
typedef enum {FALSE = 0, TRUE} boolean_ty;

typedef int (*dll_action_ty)(void* data, void* param);
typedef int (*match_action_ty)(const void* list_data, void* params);

/************************ definitions in the document *************************/

/* invalid iterator =  DlistEnd() */

/* undefined behaviors: iterators that do not 'belong' to the same list */

/*********************** D-list API Declerations  ****************************/

/*Function that creats the list.
  
  Arguments: None.
  
  Return value: pointer to list.
  
  Complexity: O(1). 
  
  Undefined B.: */
dll_ty* DListCreate(void);

/*Function that destroys the list.
  
  Arguments: pointer to list.
  
  Return value: None.
  
  Complexity: O(n). 
  
  Undefined B.: NULL pointer to list*/
void DListDestroy(dll_ty *list);

/*Function that returns the number of current elements in the list.
 
  Arguments: pointer to list.
  
  Return value: Num of elements currently in list.
  
  Complexity: O(n).
  
  Undefined B.: NULL pointer to list*/
size_t DListSize(const dll_ty *list);

/*Function that checks if list is empty.
  
  Arguments: pointer to list.
  
  Return value: if empty, it returns 0, othewise it will return 1
  
  Complexity: O(1).
  
  Undefined B.: NULL pointer to list*/
int DListIsEmpty(dll_ty *list);

/*Function that retrieves the data from an element of an iterator.
  
  Arguments: Iterator. 
  
  Return value: void* pointer.
  
  Complexity: O(1).
  
  Undefined B.: Invalid iterator*/
void* DListGetData(dll_iter_ty iter);

/*Function that sets the data to an element of an iterator.
 
  Arguments: An iterator And data to be set.
  
  Return value: None.
  
  Complexity: O(1).
  
  Undefined B.: Invalid iterator, NULL pointer to data */
void DListSetData(dll_iter_ty iter, const void *data);

/*Function that inserts a new element in the list before the current element
    of an an iterator.
 
  Return value: if successfull, The new element of an iterator
    ,otherwise, it will return invalid iterator that needs to be tested again. 
    using IsSameIter
 
  Complexity: O(1).
  
  Undefined B.: Invalid iterator, NULL pointer to data, NULL pointer to list*/
dll_iter_ty DListInsertBefore(dll_iter_ty iter, void* data, dll_ty *dlist);

/*Function that removes an element from the list.
  
  Arguments: iterator to an element to be removed.
  
  Return value: iterator to next element.
  
  Complexity: O(1).
  
  Undefined B.: Invalid iterator*/
dll_iter_ty DListRemove(dll_iter_ty iter);

/*Function that inserts a new element in the beginning of a list.
  
  Arguments: pointer to list, data of the new element.
  
  Return value: It will return the new element of an an iterator.
  
  Complexity: O(1).
  
  Undefined B.: NULL pointer to list, NULL pointer to data */
dll_iter_ty DListPushFront(dll_ty *list, void* data);

/*Function that inserts a new element in the end of a list.
  
  Arguments: pointer to list, data of the new element.
  
  Return value: It will return the new element of an an iterator.
  
  Complexity: O(1).
  
  Undefined B.: NULL pointer to list, NULL pointer to data */
dll_iter_ty DListPushBack(dll_ty *list, void* data);

/*Function that removes an element from the the start of a list.
  
  Arguments: pointer to list.
  
  Return value: iterator to next element.
  
  Complexity: O(1).
 
  Undefined B.: NULL pointer to list*/
dll_iter_ty DListPopFront(dll_ty *list);

/*Function that removes an element from the the end of a list.
  
  Arguments: pointer to list.
  
  Return value: iterator to next element.
  
  Complexity: O(1).
  
  Undefined B.: NULL pointer to list */
dll_iter_ty DListPopBack(dll_ty *list);

/*Function that executes the match_func for every element in the range of
    the 'from' iterator (included) to the 'to' iterator (not included).
  
  Arguments: 'from' - iterator to the beginning of the range, 'to' - 
    iterator to the end of the range(not included in the search), 
    a user match func, user paramters.
  
  Return value: iterator to the found element. If the element is not found - 
    return the 'to' iterator.
  
  Complexity: O(n).
  
  Undefined B.: invalid iterators, NULL pointer to match_func, NULL pointer to params. */
dll_iter_ty DListFind
(dll_iter_ty from, dll_iter_ty to, match_action_ty match_func, void* params);

/*Function that executes an action func for every element in the range of
   from the 'from' iterator (included) to the 'to' (not included).
  
  Arguments: 'from' - iterator to the beginning of the range, 'to' - 
    iterator to the end of the range(not included in the search), 
    a user action func, user paramters.
  
  Return value: on succes it returns 0, otherwise, it will return 1
  
  Complexity: O(n) * complexity of action func.
  
  Undefined B.: invalid iterators, NULL pointer to match_func, NULL pointer to params. */
int DListForEach
(dll_iter_ty from, dll_iter_ty to, dll_action_ty action_func, void* params);

/*Function that executes the match_func for every element in the range of the 
    'from' iterator (included) to the 'to' (not included) and save the found 
    elements to a given list.
  
  Arguments: 'from' - iterator to the beginning of the range, 'to' - 
    iterator to the end of the range(not included in the search), 
    a user match func, match func ,user paramters.
  
  Return value: SUCCESS = Found at least one element, 
    FAILURE = No matching elements found. 
  
  Undefined B.: invalid iterators, NULL pointer action_func, NULL pointer to params, 
    invalid list */
int DListMultiFind(dll_iter_ty from, dll_iter_ty to, match_action_ty match_func,
	void *param, dll_ty *dest_list);


/*Function that relocates the elements in the range of the 'from' iterator 
    (included) to the 'to' iterator (not included) before the where_iter element.
  
  Arguments: 'from' - iterator to the beginning of the range, 'to' - 
    iterator to the end of the range(not included in the search),
    iterator to be relocated before it.
  
  Return value: void
  
  Complexity: O(1).
  
  Undefined B.: invalid iterators. */
void DListSplice(dll_iter_ty from, dll_iter_ty to, dll_iter_ty where_iter);

/********************* Iterator API Declerations  ****************************/

/*Function that checks if the iterators are the same. 
 
  Arguments: an iterator and another to compare to.
  
  Return value: Same = 1, different = 0.
  
  Complexity: O(1).
  
  Undefined B.: invalid iterators. */
int DListIsSameIter(dll_iter_ty iter, dll_iter_ty iter_to_compare);

/*Function that returns an iter to the begining of the list.
  
  Arguments: pointer to list.
  
  Return value: an iterator of the first element in the list.
  
  Complexity: O(1).
  
  Undefined B.: invalid list. */
dll_iter_ty DListBegin(const dll_ty *list);

/*Function that returns an iter to the end of the list.
  
  Arguments: pointer to list.
  
  Return value: invalid element - can be reached by calling next after last element.
  
  Complexity: O(1).
  
  Undefined B.:  invalid list. */
dll_iter_ty DListEnd(const dll_ty *list);

/*Function that receives an element in the list, and returns the following element.
  
  Arguments: an iterator from a list.
  
  Return value: iterator to the next element,
  
  Complexity: O(1).
  
  Undefined B.: invalid iterator*/
dll_iter_ty DListNext(dll_iter_ty iter);

/*Function that receives an element in the list, and returns the previous element.
  
  Arguments: an iterator from a list.
  
  Return value: iterator to the previous element.
  
  Complexity: O(1).
  
  Undefined B.: invalid iterator*/
dll_iter_ty DListPrev(dll_iter_ty iter);

/******************************************************************************/

#endif /*(__ILRD_DLL_H__)*/


