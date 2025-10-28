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
void Timer1A_Init(void);
void Timer1A_Delay(unsigned long delay);
#define TIMER1_SETUP	0x02
#define TIMER1_CLEAR	0x00
#define TIMER1_16BIT	0x4
#define TIMER1_PCD		0x2
#define TIMER1_PRESCALE 249
#define TIMER1_ENABLE	0x00000001
#define OBJECT_FOLLOWING_DELAY 10000 // 10k
#endif // __TIMER2INTS_H__


