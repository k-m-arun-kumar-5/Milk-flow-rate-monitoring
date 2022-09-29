/* ********************************************************************
FILE                 : bargraph.c

PURPOSE              :  Simple bargraph library. 
                       Prepare for the bargraph display. update the bargraph and display the bargraph pins.                   
	 
AUTHOR               : K.M. Arun Kumar alias Arunkumar Murugeswaran
	 
KNOWN BUGS           : 

NOTE                 :  used linear scale on bargraph. 

CHANGE LOGS          :

*****************************************************************************/	
#include "main.h"
#include "port.h"

#include "bargraph.h"

// ------ Public variable declarations -----------------------------

/* Data_G  var has total pulse count over period of every ( MAX_UPDATECALL * SYSTEM_TICK_INTERVAL) time,
   to be used for bargraph display */
tBargraph Data_G;

// ------ Private constants ----------------------------------------

#define BARGRAPH_ON (1)
#define BARGRAPH_OFF (0)

/* max pulse count that can have on a every ( MAX_UPDATECALL * SYSTEM_TICK_INTERVAL) period time.
MAX_UPDATECALL = 450 and SYSTEM_TICK_INTERVAL = 30 */
/* we want MAX_PULSE_CNT as 9, because it has 9 possible states, for 8 pin output port in uC, we can easily adopt it */
#define MAX_PULSE_CNT (9)


// ------ Private variables ----------------------------------------

/* use linear bargraph for 9 possible state. These variables used to store the thresholds for bargraph  */
static tBargraph Threshold_1_G;
static tBargraph Threshold_2_G;
static tBargraph Threshold_3_G;
static tBargraph Threshold_4_G;
static tBargraph Threshold_5_G;
static tBargraph Threshold_6_G;
static tBargraph Threshold_7_G;
static tBargraph Threshold_8_G;

/*------------------------------------------------------------*-
FUNCTION NAME  : BARGRAPH_Init

DESCRIPTION    : Prepare for the bargraph display. Initialize bargraph pins as OFF 
                 and configure threshold values of each of the Threshold_* variables

INPUT          : none

OUTPUT         : initialize bargraph pins as OFF.

NOTE           : 
-*------------------------------------------------------------*/
void BARGRAPH_Init(void)
   {
   Pin0 = BARGRAPH_OFF;   
   Pin1 = BARGRAPH_OFF;   
   Pin2 = BARGRAPH_OFF;   
   Pin3 = BARGRAPH_OFF;   
   Pin4 = BARGRAPH_OFF;   
   Pin5 = BARGRAPH_OFF;   
   Pin6 = BARGRAPH_OFF;   
   Pin7 = BARGRAPH_OFF;   

   /*  Use a linear scale to display data possible output states .Bargraph has *9* possible output states.
       In our case, (0 - 8) possible output states. */
	/* fixed Range = (BARGRAPH_MAX - BARGRAPH_MIN) */	
   /* if BARGRAPH_MAX = 29, BARGRAPH_MIN = 2, and MAX_PULSE_CNT = 9,then Threshold_1_G = 3, Threshold_2_G = 6, 
		   Threshold_3_G = 9, so on, to form a linear scale	 */	
	/*	  Do all calculations of threshold values ONCE */
   Threshold_1_G = (BARGRAPH_MAX - BARGRAPH_MIN) / MAX_PULSE_CNT;
   Threshold_2_G = Threshold_1_G * 2;
   Threshold_3_G = Threshold_1_G * 3;
   Threshold_4_G = Threshold_1_G * 4;
   Threshold_5_G = Threshold_1_G * 5;
   Threshold_6_G = Threshold_1_G * 6;
   Threshold_7_G = Threshold_1_G * 7;
   Threshold_8_G = Threshold_1_G * 8;
   }
/*------------------------------------------------------------*-
FUNCTION NAME  : BARGRAPH_Update

DESCRIPTION    :  Update the bargraph and display the bargraph pins
	 
INPUT          : total number of pulse count for  period of every ( MAX_UPDATECALL * SYSTEM_TICK_INTERVAL) time

OUTPUT         : update bargraph display pins.

NOTE           : this func is called for period of every ( MAX_UPDATECALL * SYSTEM_TICK_INTERVAL) time.
-*------------------------------------------------------------*/
void BARGRAPH_Update(void)
   {
		/* if Data_G = 9 and BARGRAPH_MIN = 2, then Data = 7 */ 
	  tBargraph Data = Data_G - BARGRAPH_MIN; 
  /* bargraph has *9* possible output states */
	/* eg pin0 = 1, when data = 7, for Threshold_1_G = 3 */	 
   Pin0 = ((Data >= Threshold_1_G) == BARGRAPH_ON);
	/* eg pin1 = 1, when data = 7, for Threshold_2_G = 6 */		 
   Pin1 = ((Data >= Threshold_2_G) == BARGRAPH_ON);
	/* eg pin2 = 0, when data = 7, for Threshold_3_G = 9 , so on .. */	
  /* eg. in our case exact data lies between Threshold_2_G and Threshold_3_G */		 
   Pin2 = ((Data >= Threshold_3_G) == BARGRAPH_ON);
   Pin3 = ((Data >= Threshold_4_G) == BARGRAPH_ON);
   Pin4 = ((Data >= Threshold_5_G) == BARGRAPH_ON);
   Pin5 = ((Data >= Threshold_6_G) == BARGRAPH_ON);
   Pin6 = ((Data >= Threshold_7_G) == BARGRAPH_ON);
   Pin7 = ((Data >= Threshold_8_G) == BARGRAPH_ON);
   }

/*------------------------------------------------------------------*-
  ---- END OF FILE -------------------------------------------------
-*------------------------------------------------------------------*/
