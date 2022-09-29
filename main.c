/* ********************************************************************
FILE                   : main.c

PROGRAM DESCRIPTION    : program for the 8051/8052 with Milk flow rate monitoring (Application Task).
                       the rotating shaft gives rise to a pulse train. Determine flow rate from pulse train .
											 pulse train is given as a input to counter. As an output, bargraph is displayed, 
											 giving the operator on every period of ( MAX_UPDATECALL * SYSTEM_TICK_INTERVAL) time, a 
											 visual representation of the flow rate on that period. 
											 On every period of ( MAX_UPDATECALL * SYSTEM_TICK_INTERVAL) time, an audible alarm, 
											 which will sound when the flow rate on that period falls below an acceptable level.
											 For simulation purpose, pulse train is feed to switch, not to a counter. Flow rate is directly
											 proportional to toggling speed of operation of switch between 0 and 1.											 
	 
AUTHOR                : K.M. Arun Kumar alias Arunkumar Murugeswaran
	 
KNOWN BUGS            : 

NOTE                  : implements sEOS, a simple time triggered ,nonpremptive and single application task executing embedded OS.
                       Switch is connected to P3.0 pin. alarm is connected to P3.7 pin. On every period of 
											 ( MAX_UPDATECALL * SYSTEM_TICK_INTERVAL) time, bargraph of flow rate on that period is displayed is P2 port,
 											 with P2^0 been least discrete value
                       
CHANGE LOGS           : 

*****************************************************************************/
#include "main.h"
#include "port.h"
#include "simple_EOS.h"
#include "bargraph.h"
#include "pulse_count.h"

/* system ticks interval in m sec. for Every TICK_INTERVAL, application Task followed by e0S(system Task)
   is executed. NOTE: make sure that SYSTEM_TICK_INTERVAL is large enough that uC executing maximum time 
	 required to start executing and completes  Application task and system task(sEOS_Go_To_Sleep) 	*/
#define SYSTEM_TICK_INTERVAL  (30)
/*------------------------------------------------------------*-
FUNCTION NAME  : main

DESCRIPTION    :1: Configure ports and initialize bargraph display variables.
                2:  configure Timer T2 for automatic reload with preload and recapture values,
								 corresponding to SYSTEM_TICK_INTERVAL msec and run the T2.								
								3: put the CPU of 8051/8052 in idle mode and Every  SYSTEM_TICK_INTERVAL when T2 overflow interrupt occurs, 
                4: check for falling edge of pulse train, and updates total pulse count over a period of time.
                5: Check 	total pulse count over a period of time is below threshold value and update the bargraph.							
                6: go to step 3, and repeat
								
INPUT          : none

OUTPUT         : 

NOTE           : 
-*------------------------------------------------------------*/

void main(void)
   {
		 /* ========= begin : USER CODE INITIALIZE ========== */
		 /* initialize test bits and configure switch as input */
   PULSE_COUNT_Init();
	/* Prepare for the bargraph display. */	 
   BARGRAPH_Init();
	/* ========= end : USER CODE INITIALIZE ========== */	 

   /* Set up simple EOS. configure Timer 2 and start running Timer 2 */ 
   sEOS_Init_Timer2(SYSTEM_TICK_INTERVAL);   
   
   while(1) // Super Loop
      {
      /* uC enters idle mode to save power consumed by CPU. But timers, UART and other
				peripherals continue to work after idle mode */
			/* ============ PERIODIC SYSTEM TASK EXECUTION ============= */	
      sEOS_Go_To_Sleep();  
			/* here uC does not execute. When interrupt occurs, uC executes its corresponding ISR.
         in our case, only Timer 2 overflow interrupt will occur, which executes its ISR, sEOS_ISR()	*/
      	/* ISR of Timer 2 executes application task */	
      }
   }

/*------------------------------------------------------------------*-
  ---- END OF FILE -------------------------------------------------
-*------------------------------------------------------------------*/
