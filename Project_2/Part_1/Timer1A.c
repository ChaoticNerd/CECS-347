// Timer1A.c
// Runs on LM4F120/TM4C123
// Use TIMER1A in 16-bit periodic countdown mode to request interrupts at a periodic rate

#include "tm4c123gh6pm.h"
#include <stdint.h>
#include "Timer1A.h"


// ***************** Timer1_Init ****************
// Activate TIMER1 interrupts to run user task periodically
// Inputs:  task is a pointer to a user function
//          period in units (1/clockfreq)
// Outputs: none

void Timer1A_Init(void){
	SYSCTL_RCGCTIMER_R |= TIMER1_SETUP;   // 0) activate TIMER1
	while((SYSCTL_RCGCTIMER_R&TIMER1_SETUP)!=TIMER1_SETUP){}; // wait for clock to start
	TIMER1_CTL_R = TIMER1_CLEAR; 		// disable TIMER1A
	TIMER1_CFG_R = TIMER1_16BIT;    // 2) configure for 16-bit mode
  TIMER1_TAMR_R = TIMER1_PCD;   			// 3) configure for periodic down-count mode
	TIMER1_TAPR_R = TIMER1_PRESCALE;         	// 5) bus clock prescale

}

void Timer1A_Delay(unsigned long delay){
	TIMER1_CTL_R = TIMER1_CLEAR; 		// disable TIMER1A
	TIMER1_TAILR_R = delay-1;
	TIMER1_CTL_R = TIMER1_ENABLE;
	while(TIMER1_TAR_R != 0){}
}
