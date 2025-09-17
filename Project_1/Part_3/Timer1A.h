// Timer1A.h
// Runs on LM4F120/TM4C123
// Use Timer1A in 16-bit periodic countdown mode to request interrupts at a periodic rate
// Ana Arante, Hanna Estrada, Emily Hsu, Suphia Sidiqi
// September 8, 2025

#ifndef __TIMER1INTS_H__ // do not include more than once
#define __TIMER1INTS_H__

// ***************** Timer1A_Init ****************
// Activate Timer1 interrupts to run user task periodically
// Inputs:  task is a pointer to a user function
//          period in units (1/clockfreq)
// Outputs: none
void Timer1A_Init(unsigned long period);

#endif // __TIMER2INTS_H__
