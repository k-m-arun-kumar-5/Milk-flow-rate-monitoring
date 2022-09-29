/**************************************************************************
   FILE          :    port.h
 
   PURPOSE       :    port header - define port and its pin assignment.
 
   AUTHOR        :     K.M. Arun Kumar alias Arunkumar Murugeswaran
 
  KNOWN BUGS     :
	
  NOTE           :    port header - user interface to external device, such as LED, Switch,
 	connection to other microcontroller via RS232, USB, etc. 
 To avoid this header file to be included more than once, conditional directive is used  
	
  CHANGE LOGS     :  
	   
 **************************************************************************/
#ifndef _PORT_H
#define _PORT_H

// Connect pulse train input to this pin(switch pin) - debounced in software
sbit  Sw_pin = P3^0;  

// Connect alarm to this pin (set if pulse is below threshold)
sbit Alarm_pin = P3^7;

// ------ Bargraph.C -----------------------------------------------

// Bargraph display on these pins
// The 8 port pins may be distributed over several ports if required
sbit Pin0 = P2^0;
sbit Pin1 = P2^1;
sbit Pin2 = P2^2;
sbit Pin3 = P2^3;
sbit Pin4 = P2^4;
sbit Pin5 = P2^5;
sbit Pin6 = P2^6;
sbit Pin7 = P2^7;

// Used for manually checking timing (in simulator) by using preload value for Timer 2.
#define RELOADH_PORT P1
#define RELOADL_PORT P0

#endif 

/*------------------------------------------------------------------*-
  ---- END OF FILE -------------------------------------------------
-*------------------------------------------------------------------*/

