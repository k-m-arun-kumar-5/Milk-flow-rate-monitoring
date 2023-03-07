/**************************************************************************
   FILE          :    bargraph.h
	 
   PURPOSE       :    Type declarations for the bargraph.c
 
   AUTHOR        :   K.M. Arun Kumar alias Arunkumar Murugeswaran
 
 KNOWN BUGS      :
 
  NOTE           :   bargraph.c for details. we use linear scale on bargraph 
	
  CHANGE LOGS     :
	   
 **************************************************************************/
 #ifndef _BARGRAPH_H
#define _BARGRAPH_H

#include "main.h"

// ------ Public data type declarations ----------------------------

typedef tByte tBargraph;

// ------ Public function prototypes -------------------------------

void BARGRAPH_Init(void);
void BARGRAPH_Update(void);

// ------ Public constants -----------------------------------------

/* flow rate is directly propotional to pulse count. */
/* MAX value on bargraph */
#define BARGRAPH_MAX (9)
/* MIN value on bargraph */
#define BARGRAPH_MIN (0)

#endif

/*------------------------------------------------------------------*-
  ---- END OF FILE -------------------------------------------------
-*------------------------------------------------------------------*/
