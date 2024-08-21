/******************************************************************************/
/*
  Name: AhmadWattad
  Project: Work Sheet 12 - Data Structures - Sorted List
  Reviewer: ???
  Date: 24/08/23
  Version: 1.0
*/

#ifndef __ILRD_SORTED_LIST_H__
#define __ILRD_SORTED_LIST_H__

/*overview: 
This product is a sorted list that consists of a set of sequentially linked 
records. The functions in this api can perform operations for such functionality. 
  */
/******************************** Libraries  **********************************/

#include <stddef.h> /*size_t*/

#include "dll.h"
/******************************** Definitions  ******************************/

typedef struct sorted_list sorted_list_t;
typedef struct sorted_list_iter sorted_list_iter_t;


typedef int (*sorted_list_action_t)(void *data, void *param);
typedef int (*sorted_list_compare_t)(const void* , void* );
typedef int (*sorted_list_is_match_t)(const void* , void* );

/************************ definitions in the document *************************/

/* invalid iterator =  SortedListEnd() */

/* undefined behaviors: iterators that do not 'belong' to the same list */

/*********************** D-list API Declerations  ****************************/

/*Function that creates the list.

  Arguments: A compare function to sort the list, provided by the user.

  Return value: pointer to list.

  Complexity: O(1). 

  Undefined B.: NULL pointer to CompareFunc*/
sorted_list_t* SortedListCreate(sorted_list_compare_t CompareFunc);

/*Function that destroys the list.

  Arguments: pointer to list.

  Return value: None.

  Complexity: O(n). 

  Undefined B.: Null pointer to list*/
void SortedListDestroy(sorted_list_t *list);

/*Function that returns the number of current elements in the list.
  
  Arguments: pointer to list.
  
  Return value: Num of elements currently in list.
  
  Complexity: O(n).
  
  Undefined B.: NULL pointer to list*/
size_t SortedListSize(const sorted_list_t *list);

/*Function that checks if list is empty.

  Arguments: pointer to list.

  Return value: if empty, it returns 0, othewise it will return 1

  Complexity: O(1).

  Undefined B.: NULL pointer to list*/
int SortedListIsEmpty(sorted_list_t *list);

/*Function that retrieves the data from an element of an iterator.
  
  Arguments: iter - an iterator from the list. 
  
  Return value: void pointer.
  
  Complexity: O(1).
  
  Undefined B.: Invalid iterator*/
void* SortedListGetData(sorted_list_iter_t iter);

/*Function that inserts a new element in the list without disrupting the sorting.
    
  Arguments: SortedList - pointer to list, data - data to insert .
  
  Return value: if successfull, it will return the new element of an iterator
    ,otherwise, it will return invalid iterator that needs to be tested again. 
    using IsSameIter.
  
  Complexity: O(n).
  
  Undefined B.: NULL pointer to data, NULL pointer to list*/
sorted_list_iter_t SortedListInsert(sorted_list_t *SortedList, void* data);

/*Function that removes an element from the list.

  Arguments: iterator to an element to be removed.

  Return value:  iterator to next element.

  Complexity: O(1).

  Undefined B.: Invalid iterator*/
sorted_list_iter_t SortedListRemove(sorted_list_iter_t iter);

/*Function that removes an element from the the start of a list.
  
  Arguments: pointer to list.
  
  Return value:  iterator to next element.
  
  Complexity: O(1).
  
  Undefined B.: NULL pointer to List*/
sorted_list_iter_t SortedListPopFront(sorted_list_t *list);

/*Function that removes an element from the the end of a list.
  
  Arguments: pointer to list.
  
  Return value: iterator to next element.
  
  Complexity: O(1).
  
  Undefined B.: NULL pointer to List */
sorted_list_iter_t SortedListPopBack(sorted_list_t *list);

/*Function that executes the match_func for every element in the range of
    the 'from' iterator (included) to the 'to' iterator (not included).
  
  Arguments: 'from' - iterator to the beginning of the range, 'to' - 
    iterator to the end of the range(not included in the search), 
    a user match func, user paramters -value to be found.
  
  Return value: iterator to the found element. If the element is not found - 
    it will return the 'to' iterator.
  
  Complexity: O(n).
  
  Undefined B.: invalid iterators, NULL pointer to params. */
sorted_list_iter_t SortedListFind
(sorted_list_iter_t from, sorted_list_iter_t to, void* params, sorted_list_t *list);

/*Function that executes an action func for every element in the range of
   from the 'from' iterator (included) to the 'to' (not included).
  
  Arguments: 'from' - iterator to the beginning of the range, 'to' - 
    iterator to the end of the range(not included in the search), 
    a user action func, user paramters if needed.
  
  Return value: on success it returns 0, otherwise, it will return 1.
  
  Complexity: O(n) * complexity of action func.
  
  Undefined B.: invalid iterators, NULL pointer to action_func, NULL pointer 
    to params. */
int SortedListForEach
(sorted_list_iter_t from, sorted_list_iter_t to, sorted_list_action_t action_func, 
void* params);

/*Function that executes the conditional func for every element in the range 
    of the 'from' iterator (included) to the 'to' iterator (not included).
  
  Arguments: from' - iterator to the beginning of the range, 'to' - 
    iterator to the end of the range(not included in the search),
    conditional function  , params if needed.
  
  Return value: on success it returns the found iter, otherwise, it will 
    return the 'to' iterator.
  
  Complexity: O(n) * complexity of conditional__func .
    
  Undefined B.: invalid iterators, NULL pointer to conditional__func, NULL pointer 
    to params. */ 
sorted_list_iter_t SortedListFindIF
(sorted_list_iter_t from, sorted_list_iter_t to, sorted_list_is_match_t conditional__func, void *params);


/*Function that merges src list into the dest_list in an ordered approach.
  
  Arguments: pointer to dest list and pointer to src list.
  
  Return value: pointer to list to the merged lists
  
  Complexity: O(n*m).
  
  Undefined B.: NULL pointers to lists, */
sorted_list_t* SortedListMerge(sorted_list_t *dest_list, sorted_list_t *src_list);

/********************* Iterator API Declerations  ****************************/

/*Function that checks if the iterators are equal. 
  
  Arguments: an iterator and another to compare to.
  
  Return value: it will return 1 if similar, otherwise, it will return 0.
  
  Complexity: O(1).
  
  Undefined B.: invalid iterators. */
int SortedListIsSameIter(sorted_list_iter_t iter, sorted_list_iter_t iter_to_compare);

/*Function that returns an iter to the begining of the list.
  
  Arguments: pointer to list.
  
  Return value: an iterator of the first element in the list.
  
  Complexity: O(1).
  
  Undefined B.: NULL pointer to list. */
sorted_list_iter_t SortedListBegin(const sorted_list_t *list);

/*Function that returns the element of an iterator to the end of the list.
  
  Arguments: pointer to list.
  
  Return value: an iterator of the last element in the list.
  
  Complexity: O(1).
  
  Undefined B.:  NULL pointer to list. */
sorted_list_iter_t SortedListEnd(const sorted_list_t *list);

/*Function that receives an element in the list, and returns the following element.
  
  Arguments: an iterator from a list.
  
  Return value: iterator to the next element.
  
  Complexity: O(1).
  
  Undefined B.: invalid iterator */
sorted_list_iter_t SortedListNext(sorted_list_iter_t iter);

/*Function that receives an element in the list, and returns the previous element.
  
  Arguments: an iterator from a list.
  
  Return value: iterator to the previous element.
  
  Complexity: O(1).
  
  Undefined B.: invalid iterator*/
sorted_list_iter_t SortedListPrev(sorted_list_iter_t iter);

/******************************************************************************/

struct sorted_list_iter
{
	dll_iter_ty internal_iter;
	#ifndef NDEBUG
	sorted_list_t *list;
	#endif
};

#endif /*(__ILRD_SORTED_LIST_H__)*/


