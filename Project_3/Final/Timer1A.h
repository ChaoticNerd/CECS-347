/////////////////////////////////////////////////////////////////////////////
// Course Number: CECS 347
// Assignment: Project 2, Part 1
// @author: Justin Narciso
// @author: Natasha Kho
// @date 10/1/2025
// CSULB Computer Engineering
// Description: Uses the TIMER1A 16-bit periodic countdown as a regular timer
//							Creates the delays; no actual interrupts
/////////////////////////////////////////////////////////////////////////////

#include <stdint.h>

#ifndef __TIMER1INTS_H__ // do not include more than once
#define __TIMER1INTS_H__

// ***************** Timer1A_Init ****************
// Activate Timer1 interrupts to run user task periodically
// Inputs:  task is a pointer to a user function
//          period in units (1/clockfreq)
// Outputs: none
void Timer1A_Init(void(*task)(void), uint32_t period);
void Timer1A_Handler(void);

#define TIMER1_SETUP	0x02 
#define TIMER1_CLEAR	0x00
#define TIMER1_32BIT	0x00
#define TIMER1_PCD		0x2
#define TIMER1_PRESCALE 0
#define TIMER1_ENABLE	0x00000001

#define TIMER1_PRIORITY_CLEAR 0xFF1FFFFF
#define TIMER1_PRIORITY_SET 0x00A00000
#define TIMER1A_IRQ 0x200000
#endif // __TIMER2INTS_H__


