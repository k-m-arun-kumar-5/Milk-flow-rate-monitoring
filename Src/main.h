/**************************************************************************
   FILE          :    main.h
 
   PURPOSE       :    project header.  
 
   AUTHOR        :    K.M. Arun Kumar alias Arunkumar Murugeswaran
 
  KNOWN BUGS     :
	
  NOTE           :   PROJECT header- groups the key information about the 8051 device you have used, along with other key
parameters – such as the oscillator frequency and commonly used information such as common data types in the project
	
  CHANGE LOGS     :
	   
 **************************************************************************/
 
 /* to avoid this header file to be included more than once, conditional directive is used  */
#ifndef _MAIN_H
#define _MAIN_H

/* Must include the appropriate microcontroller header file here eg for eg we use 8051/8052 uC device.
 In most case, microcontroller header is also a device header.in our case, reg52.h
 device header will include the addresses of the special function registers (SFRs) used for port access, plus similar
 details for other on-chip components such as analog-to-digital converters*/
 /* reg52.h is a system header and for <> enclosed one, preprocessor will search the reg52.h in predetermined directory path to locate the header file. */
 // Must include the appropriate microcontroller header file here
#include <reg52.h>
/* set Oscillator / resonator frequency (in Hz) here we set 12MHz */
#define OSC_FREQ (12000000UL)

// Number of oscillations required to execute per instruction or increment a timer (12, etc)
// 12 – Original 8051 / 8052 and numerous modern versions
// 6 – Various Infineon and Philips devices, etc.
// 4 – Dallas 320, 520 etc.
// 1 – Dallas 420, etc.

#define OSC_PER_INST (12)

typedef unsigned char tByte;
typedef unsigned int tWord;
typedef unsigned long tLong;

/* define Interrupts number with macros for easy reference. (ie) when T2 overflows,
 it's  referenced as interrupt number(IE) 5 in 8051/8052, which transfers program control 
 to address 0x2B in uC, corresponding to IE 5. bit 5 of the IE() register will be linked to 
 a T2_overflow_ISR() for ‘Timer 2 Overflow'. refer table 7.2 in book 'embedded C' by pont,
  for interrupt sources, it's address and IE index, for 8051/8052 */
#define INTERRUPT_Timer_0_Overflow 1
#define INTERRUPT_Timer_1_Overflow 3
#define INTERRUPT_Timer_2_Overflow 5

#endif
