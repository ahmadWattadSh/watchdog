/*Name: AhmadWattad
  Project: Data Structures - UID
  Reviewer: ??
  Date: 06/09/23
  		*/
#include <assert.h> /*assert*/
#include <stdlib.h> /*malloc*/
#include <unistd.h> /*getpid*/
#include <stdio.h> /*printf*/

#include "uid.h"


/******************************* Definitions **********************************/

uid_ty bad_uid = { 0, -1, -1};


/******************************************************************************/
/******************************* API Funcs ************************************/
/******************************************************************************/

/******************************* CBUFF-CREATE ********************************/

uid_ty UIDCreate()
{
	static size_t counter = 1;	
	time_t time_to_init = 0;
	uid_ty uid_for_init = {0};

	time(&time_to_init);
	if( time_to_init == -1)
	{
		return bad_uid;
	}
		
	uid_for_init.counter = counter;
	uid_for_init.timestamp = time_to_init;	
	uid_for_init.pid = getpid();		
	
	++counter;
	
	return uid_for_init;
}


int UIDIsSame(uid_ty uid, uid_ty uid_to_compare)
{

	return !(uid.counter != uid_to_compare.counter || 
		uid.timestamp != uid_to_compare.timestamp || 
			uid.pid != uid_to_compare.pid);

}




