/******************************************************************************/
/*
  Name: AhmadWattad
  Project: Data Structures - UID
  Reviewer: Uri 
  Date: 06/09/23
  Version: 1.0
*/

#ifndef __ILRD_UID_H__
#define __ILRD_UID_H__

/*overview: 
  */
/******************************** Libraries  **********************************/

#include <stddef.h> /*size_t*/
#include <time.h> /*time_t*/
#include <sys/types.h> /*uid_ty*/

/******************************** Definitions  ******************************/

typedef struct uid uid_ty;

typedef enum {FALSE = 0, TRUE} boolean_ty;

extern uid_ty bad_uid;

/*********************** D-list API Declerations  ****************************/

/*Function that creats the new UID.
  
  Arguments: None.
  
  Return value: The created UID.
  
  Complexity: O(1). 
  
  Undefined B.: */
uid_ty UIDCreate(void);

/*Function that checks if an UID and another UID  are the same. 
 
  Arguments: an UID and another UID to compare.
  
  Return value: Same = 1, different = 0.
  
  Complexity: O(1).
  
  Undefined B.:  */
int UIDIsSame(uid_ty uid, uid_ty uid_tyo_compare);



struct uid
{
	size_t counter;
	time_t timestamp;
	pid_t pid;
};



#endif /*(__ILRD_UID_H__)*/


