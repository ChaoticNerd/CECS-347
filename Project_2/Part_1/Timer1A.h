// Timer1A.h
// Runs on LM4F120/TM4C123
// Use Timer1A in 16-bit periodic countdown mode to request interrupts at a periodic rate

#include <stdint.h>

#ifndef __TIMER1INTS_H__ // do not include more than once
#define __TIMER1INTS_H__

// ***************** Timer1A_Init ****************
// Activate Timer1 interrupts to run user task periodically
// Inputs:  task is a pointer to a user function
//          period in units (1/clockfreq)
// Outputs: none
void Timer1A_Init(void);

#define TIMER1_SETUP	0x02
#define TIMER1_CLEAR	0x00
#define TIMER1_16BIT	0x4
#define TIMER1_PCD		0x2
#define TIMER1_PRESCALE 15
#define TIMER1_ENABLE	0x00000001
#endif // __TIMER2INTS_H__


