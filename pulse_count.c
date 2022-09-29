/* ********************************************************************
FILE                 : pulse_count.c

PURPOSE              :  Count pulses from a mechanical switch, optical sensor or similar devices
                                                            ___
                        Responds to falling edge of pulse:     |___
												Update total number of pulse count every ( MAX_UPDATECALL * SYSTEM_TICK_INTERVAL) time.
												Check if flow rate is below a threshold value and display bargraph display.  
	 
AUTHOR               : K.M. Arun Kumar alias Arunkumar Murugeswaran
	 
KNOWN BUGS           : 

NOTE                 :MAX_UPDATECALL = 1500U is used here for test purposes (in simulator).
                     [Because there is a limit to how fast, you can simulate pulses by hand 
										 by toggling switch pin(Sw_pin)( by changing Sw_pin's pin value from 
										 1 to 0 to 1 for a single pulse, and repeat ...]

CHANGE LOGS          :

*****************************************************************************/	
#include "main.h"
#include "port.h"

#include "bargraph.h"
#include "pulse_count.h"


// ------ Private function prototypes ------------------------------
void PULSE_COUNT_Check_Below_Threshold(const tByte);

// ------ Public variable declarations -----------------------------
// The data to be displayed

extern tBargraph Data_G;

// ------ Public variable definitions -------------------------------
// Set only after falling edge is detected
bit Falling_edge_G;

// ------ Private variable definitions ------------------------------
/* The results of successive tests of the pulse signal, with Test0 having latest 
  pulse signal level ie high or low level */
// (NOTE: Can't have arrays of bits...)
static bit Test4, Test3, Test2, Test1, Test0;

/* Total_G var = instanteous value of total pulse count during period of ( MAX_UPDATECALL * SYSTEM_TICK_INTERVAL) time. */
static tByte Total_G = 0;
/* Calls_G var = instanteous value of total number of times that PULSE_COUNT_Update() has called, 
   with max value of MAX_UPDATECALL, then reset and increment every time PULSE_COUNT_Update() is called.
	 PULSE_COUNT_Update() is called every SYSTEM_TICK_INTERVAL time.  */
static tWord Calls_G = 0;

// ------ Private constants ----------------------------------------

/* Allows changed of logic without hardware changes. 
 */
#define HI_LEVEL (0)
#define LO_LEVEL (1)

#define ALARM_ON   (1)
#define ALARM_OFF  (0)

/* for every (MAX_UPDATECALL * SYSTEM_TICK_INTERVAL) period of time,
  check if flow rate on that period is below a threshold value (PULSE_THRESHOLD)  */
#define PULSE_THRESHOLD     (2U)
/*  MAX_UPDATECALL = max value of Calls_G (number of times PULSE_COUNT_Update() is called). 
    When Calls_G == MAX_UPDATECALL, update the bargraph display pins, resets Calls_G and counts again */
#define MAX_UPDATECALL      (1500U)
/*------------------------------------------------------------*-
FUNCTION NAME  : PULSE_COUNT_Init

DESCRIPTION    :  Initialization function for the switch library.
                 configure sw_pin for input and initialize test bits,
								 which is used to sample the pulse signal level every SYSTEM_TICK_INTERVAL.

INPUT          : none

OUTPUT         : 

NOTE           : for counter, initialize counter
-*------------------------------------------------------------*/
void PULSE_COUNT_Init(void)
   {
		/* configure this pin for input. input of pulse train is feed to this pin */
   Sw_pin = 1; 
   Alarm_pin = ALARM_OFF;
   /* configure test bits as 1 as 8051/8052 has sw_pin == 0 means that 
		 switch is pressed on and sw_pin == 1, means that sw_pin has been released.
 		so, test pins is initialize as logic 1, that is why falling edge is used for
		pulse count, as 0 means swich is pressed on */
   Test4 = LO_LEVEL;
   Test3 = LO_LEVEL;
   Test2 = LO_LEVEL;
   Test1 = LO_LEVEL;
   Test0 = LO_LEVEL;
   }
/*------------------------------------------------------------*-
FUNCTION NAME  : PULSE_COUNT_Check_Below_Threshold

DESCRIPTION    :  Checks to see if pulse count is below a specified
  threshold value on every( MAX_UPDATECALL * SYSTEM_TICK_INTERVAL) period time. 
	If it is below, sounds an alarm.

INPUT          : threshold value below which if pulse count is sounds an alarm.

OUTPUT         : alarm  

NOTE           : this function with slight modification can be used to control the 
	              actuator, as difference between desired and actual value(Data_G)
                is given as error value to feed input to actuator so that
                desired	value can be maintained.
                This func is called for every( MAX_UPDATECALL * SYSTEM_TICK_INTERVAL) period time.	 
-*------------------------------------------------------------*/
void PULSE_COUNT_Check_Below_Threshold(const tByte THRESHOLD)
   {
   if (Data_G < THRESHOLD)
      {
      Alarm_pin = ALARM_ON;
      }
   else
      {
      Alarm_pin = ALARM_OFF;
      }
   }
/*------------------------------------------------------------*-
FUNCTION NAME  : PULSE_COUNT_Update

DESCRIPTION    :  This is the main switch function. 
	                check for falling edge of pulse. update total pulse count during period of 
	                ( MAX_UPDATECALL * SYSTEM_TICK_INTERVAL).On every( MAX_UPDATECALL * SYSTEM_TICK_INTERVAL) 
	               period time, check if flow rate is below a threshold	value and update the bargraph display.

INPUT          : counter input or switch for a pulse train.

OUTPUT         :  update bargraph pins and alarm on every MAX_UPDATECALL * SYSTEM_TICK_INTERVAL) 
	               period time 

NOTE           :  It should be called every SYSTEM_TICK_INTERVAL ms 
	                (to allow for typical 20ms debounce time).
	                 mechnical switch should require debounce time, for correct value.
	 
-*------------------------------------------------------------*/
void PULSE_COUNT_Update(void)
   {
   // Clear timer overflow flag
   TF2 = 0;
		 
		 /* NOTE: if counter is used, then  dont require to check falling edge */
   // Shuffle the test results for pulse signal level with test0 having latest pulse signal level
   Test4 = Test3;
   Test3 = Test2;
   Test2 = Test1;
   Test1 = Test0;

   // Get latest test result
   Test0 = Sw_pin;

   // Required result:
   // Test4 == HI_LEVEL
   // Test3 == HI_LEVEL
   // Test1 == LO_LEVEL
   // Test0 == LO_LEVEL

   if ((Test4 == HI_LEVEL) && 
       (Test3 == HI_LEVEL) && 
       (Test1 == LO_LEVEL) && 
       (Test0 == LO_LEVEL))
      {
      // Falling edge detected
      Falling_edge_G = 1;
      }
   else
      {
      // falling edage is not detected
      Falling_edge_G = 0;
      }

   /* calculate sum  for every 45 calls to this task ie every (45 * SYSTEM_TICK_INTERVAL) time,
 			total pulse count is calculated for this period. It is used when pulse train is feed to counter, 
			not to the switch pin */
   //   if (++Calls_G < 45)

   /* 1500 used here for test purposes (in simulator) [Because there is a limit to how fast 
		you can simulate pulses by hand by toggling switch pin 1 to 0 to 1 for a single pulse ...] */
			
	  /* calculate sum  for every MAX_UPDATECALL calls to this task (ie) every ( MAX_UPDATECALL  * SYSTEM_TICK_INTERVAL) time,
 			total pulse count is calculated for this period  */		
   if (++Calls_G < MAX_UPDATECALL)  
      {
			/* Total_G =  instantaneous of total pulse count during the period of ( MAX_UPDATECALL * SYSTEM_TICK_INTERVAL) time.
				Total_G can have Maximum  value of 9 total pulses count on period of MAX_UPDATECALL = 1500 and SYSTEM_TICK_INTERVAL = 30 time */	
				
			Total_G += (int) Falling_edge_G;
      }
   else
      {
      /* we want MAX_PULSE_CNT as 9, because it has 9 possible states, for 8 pin output port in uC, we can easily adopt it */
			/* Data_G = total pulse count on every period of ( MAX_UPDATECALL * SYSTEM_TICK_INTERVAL) time.	*/
      Data_G = Total_G; // Max is 9
				
			/* reset and start counting total pulse count during the period of next ( MAX_UPDATECALL * SYSTEM_TICK_INTERVAL) time */	
      Total_G = 0;
      Calls_G = 0;
			/* for every (MAX_UPDATECALL * SYSTEM_TICK_INTERVAL) period of time, check if flow rate on that period is below a threshold value */  	
      PULSE_COUNT_Check_Below_Threshold(PULSE_THRESHOLD);
			/* for every (MAX_UPDATECALL * SYSTEM_TICK_INTERVAL) period of time. Update the bargraph display pins	 */
      BARGRAPH_Update();
      }
   }

/*------------------------------------------------------------------*-
  ---- END OF FILE -------------------------------------------------
-*------------------------------------------------------------------*/
