/*Name: Ahmad Wattad
  Project: sorting
  Reviewer: ??
  Date: 26/09/2023
*/


/******************************************************************************/
/******************************** LIBRARIES  **********************************/
/******************************************************************************/

#include <stdio.h>/*printf*/
#include <stdlib.h>



#include "watchdog.h"

/******************************************************************************/
/******************************** Definitions  ********************************/
/******************************************************************************/

#define ErrorMsg(expected, result, name) \
	{\
		if (expected == result)\
		{\
			printf("%s has succeeded\n",  name);\
		}\
		else\
		{\
			printf("%s has failed\n",  name);\
		}\
}

#define NUM_OF_EXPRESSIONS 18
#define MAX_EXPRESSION_SIZE 50

#define REPORT(expected , result, str) \
  {\
      if (expected != result)\
      {\
        printf("%s has failed\n", str);\
      }\
  }

/************************** Forwarded declarations ****************************/

static void UnitTesting(int argc, char* argv[]);


/******************************************************************************/
/******************************* Main *****************************************/
/******************************************************************************/

int main(int argc, char* argv[])
{
	UnitTesting(argc, argv);

	return 0;
}

/******************************************************************************/

static void UnitTesting(int argc, char* argv[])
{
	size_t i = 0;
	
	printf("lets see\n");
	MakeMeImmortal(argv, argc, 2, 10);
	
	for (i = 0; i < 4; ++i)
	{
		sleep(10);
		printf("slept 10 secs\n");
	}
	
	D
	
	
}
/******************************************************************************/	





