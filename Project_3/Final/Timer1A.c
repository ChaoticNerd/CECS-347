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

#include "tm4c123gh6pm.h"
#include <stdint.h>
#include "Timer1A.h"
#include "switch.h"


// ***************** Timer1_Init ****************
// Activate TIMER1 interrupts to run user task periodically
// Inputs:  task is a pointer to a user function
//          period in units (1/clockfreq)
// Outputs: none

static void (*PeriodicTask1)(void);

void Timer1A_Init(void(*task)(void), uint32_t period){
  PeriodicTask1 = task;
  SYSCTL_RCGCTIMER_R |= TIMER1_SETUP;          // activate TIMER1
  while((SYSCTL_RCGCTIMER_R & TIMER1_SETUP)==0){}; 

  TIMER1_CTL_R &= ~TIMER1_ENABLE;              // disable TIMER1A
  TIMER1_CFG_R = TIMER1_32BIT;                 // 32-bit
  TIMER1_TAMR_R = TIMER1_PCD;                  // periodic, down-count
  TIMER1_TAILR_R = period - 1;
  TIMER1_TAPR_R = TIMER1_PRESCALE;             // prescale (0)

  TIMER1_ICR_R = TIMER1_ENABLE;                // clear timeout flag
  TIMER1_IMR_R |= TIMER1_ENABLE;               // *** arm timeout interrupt ***

  // set priority for IRQ 21 to 5
  NVIC_PRI5_R = (NVIC_PRI5_R & TIMER1_PRIORITY_CLEAR) | TIMER1_PRIORITY_SET;

  // enable IRQ 21 in NVIC
  NVIC_EN0_R |= TIMER1A_IRQ;
  TIMER1_CTL_R |= TIMER1_ENABLE; 
}

void Timer1A_Handler(void){
  TIMER1_ICR_R = TIMER1_ENABLE;   // acknowledge timer1A timeout
  (*PeriodicTask1)();           // call Play()
}