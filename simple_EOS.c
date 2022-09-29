/* ********************************************************************
FILE                 : simple_EOS.c

PURPOSE              : Main file for Simple Embedded Operating System (sEOS) for 8051. 
                       Sets up Timer 2 to drive the simple EOS. ISR of Timer 2, which
                       calls application code. In our case flow rate monitoring and update
                       display bargraph. Then calls system task, in our case, sEOS_Go_To_Sleep() 
                       is system task , to put CPU in idle mode , to save power. 											 
	 
AUTHOR               : K.M. Arun Kumar alias Arunkumar Murugeswaran
	 
KNOWN BUGS           : 

NOTE                 : implements sEOS, a simple time triggered ,nonpremptive and single application task executing embedded OS.

CHANGE LOGS          :

*****************************************************************************/	

#include "main.h"
#include "simple_EOS.h"
#include "port.h"
#include "pulse_count.h"

/* time unit of system tick interval.  eg for 1 msec resolution of Timer 2 delay,
   TIME_UNIT = 1000, as  1 / TIME_UNIT = 1 msec. for 20 msec resolution of Timer 2 delay, 
TIME_UNIT = 50 as 1/50 = 20 msec.
SYSTEM_TICK_INTERVAL should be exact multiple of resolution of Timer 2 delay.	 */
#define TIME_UNIT   (1000UL)
/*------------------------------------------------------------*-
FUNCTION NAME  : sEOS_ISR

DESCRIPTION    :  Invoked periodically of SYSTEM_TICK_INTERVAL in msec by Timer 2 overflow interrupt: 
 
	
INPUT          : none

OUTPUT         : 

NOTE           :  see sEOS_Init_Timer2() for timing details.
-*------------------------------------------------------------*/

void sEOS_ISR() interrupt INTERRUPT_Timer_2_Overflow
   {
   /* Must manually reset the T2 overflow flag, still Timer 2 reload with recapture 
		  register values and automatically starts	T2 timer running */	 
   TF2 = 0;    

 //===== USER CODE - Begin (perodic(SYSTEM_TICK_INTERVAL in msec) executing application task) =======

   /* Call (application task) flow rate every SYSTEM_TICK_INTERVAL time interval */ 
   PULSE_COUNT_Update();

   //===== USER CODE - End =========================================
   }
/*------------------------------------------------------------*-
FUNCTION NAME  : sEOS_Init_Timer2

DESCRIPTION    :  Sets up Timer 2 to drive the simple EOS. 
	
INPUT          :  Parameter gives system tick interval in MILLISECONDS.
	                Max system tick interval is ~65ms (12 MHz oscillator).

OUTPUT         : 

NOTE           :  Precise tick intervals are only possible with certain 
  oscillator / tick interval combinations. For eg If timing is important,
  you should check the timing calculations manually by using simulator. 
	If you require both accurate baud rates and accurate EOS timing, use an
  11.0592 MHz crystal and a tick rate of 5, 10, 15, … 60 or 65 ms. 
	Such ‘divide by 5’ tick rates are precise with an 11.0592 MHz crystal.
-*------------------------------------------------------------*/
void sEOS_Init_Timer2(const tByte TICK_MS)
   {
   tLong Inc;
   tWord Reload_16;
   tByte Reload_08H, Reload_08L;

   /*  Timer 2 is configured as a 16-bit timer, which is automatically reloaded 
		  with recapture register values when it overflows */
   T2CON   = 0x04;   // Load Timer 2 control register

   // Number of timer increments required (max 65536)
   Inc = (tLong)((TICK_MS * OSC_FREQ)/(tLong)(TIME_UNIT * OSC_PER_INST));   

   // 16-bit reload value
   Reload_16 = (tWord) (65536UL - Inc);

   // 8-bit reload values (High & Low)
   Reload_08H = (tByte)(Reload_16 / 256);
   Reload_08L = (tByte)(Reload_16 % 256);

   // Used for manually checking timing (in simulator) by using preload value for Timer 2
   RELOADH_PORT = Reload_08H;
   RELOADL_PORT = Reload_08L; 

   TH2     = Reload_08H;   // Load Timer 2 high byte
   RCAP2H  = Reload_08H;   // Load Timer 2 reload capture register high byte
   TL2     = Reload_08L;   // Load Timer 2 low byte
   RCAP2L  = Reload_08L;   // Load Timer 2 reload capture register low byte

   /* Timer 2 interrupt is enabled, and ISR will be called,  whenever the timer overflows.
    MAKE sure uC global interrupt is enabled */
   ET2     = 1;

   /* Start Timer 2 running */
   TR2   = 1;     

   EA = 1;            // Globally enable interrupts
   }
/*------------------------------------------------------------*-
FUNCTION NAME  : sEOS_Go_To_Sleep

DESCRIPTION    :  This operating system enters 'idle mode' between clock ticks
  to save power.  The next system tick will return the processor to the normal operating state.
	
INPUT          : none

OUTPUT         : 

NOTE           : ADAPT AS REQUIRED FOR YOUR HARDWARE.
	               once an interrupt or HW reset is received by uC, uC awakens and starts executing ISR
	               of the interrupt, then executes instruction following the PCON |= 0x01, which had put uC in idle mode.
               	 
-*------------------------------------------------------------*/
void sEOS_Go_To_Sleep(void)
   {
   PCON |= 0x01;    // Enter idle mode (generic 8051 version)
   }

/*------------------------------------------------------------------*-
  ---- END OF FILE -------------------------------------------------
-*------------------------------------------------------------------*/
