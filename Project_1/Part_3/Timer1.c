// Timer1.c
// Runs on LM4F120/TM4C123
// Use TIMER1A in 16-bit periodic countdown mode to request interrupts at a periodic rate
// Justin Narciso
// September 8, 2025

#include "tm4c123gh6pm.h"
#include <stdint.h>

#define TRIGGER_PIN 						(*((volatile unsigned long *)0x40005080))  // PB7 is the trigger pin	
#define TRIGGER_VALUE 					0x20   			// trigger at bit 7

uint32_t TimeElapsed(void);
uint32_t TIME;

// ***************** Timer1_Init ****************
// Activate TIMER1 interrupts to run user task periodically
// Inputs:  task is a pointer to a user function
//          period in units (1/clockfreq)
// Outputs: none

void Timer1A_Init(unsigned long period){
	SYSCTL_RCGCTIMER_R |= 0x02;   // 0) activate TIMER1
	while((SYSCTL_RCGCTIMER_R&0x02)!=0x02){}; // wait for clock to start
	TIMER1_CTL_R = 0x00000000; 		// disable TIMER1A
	TIMER1_CFG_R = 0x00000004;    // 2) configure for 16-bit mode
  TIMER1_TAMR_R = 0x00000002;   			// 3) configure for periodic down-count mode
  TIMER1_TAILR_R = period-1;    // 4) reload value
  TIMER1_TAPR_R = 15;         	// 5) bus clock prescale
  TIMER1_ICR_R = 0x1;    				// 6) clear TIMER1A timeout flag
  TIMER1_IMR_R = 0x1;    				// 7) arm timeout interrupt
		
  //NVIC_PRI5_R = (NVIC_PRI5_R&0xFFFF1FFF)|0x00006000; // 8) priority 5
	
	// interrupts enabled in the main program after all devices initialized
  //NVIC_EN0_R = 1<<21;           // 9) enable IRQ 21 in NVIC
  //TIMER1_CTL_R = 0x00000001;    // 10) enable TIMER1A
}

void Timer1A_Start(unsigned long period){
	TIMER1_CTL_R = 0x00000000;
	TIMER1_TAILR_R = period-1;
	TIMER1_CTL_R = 0x00000001;	
	
}

// TIMER1_TAR_R: time elapsed since clock activated 
uint32_t Timer1A_Stop(void){
	TIMER1_CTL_R = 0x00000000; 		// disable TIMER1A
	uint32_t TIME = (TIMER1_TAILR_R - TIMER1_TAR_R)*(TIMER1_TAPR_R+1);
	//TIMER1_TAR_R = 0x00000000;
	return TIME;
}


void Timer1A_Handler(void){
	TIMER1_ICR_R = TIMER_ICR_TATOCINT;// acknowledge TIMER1A timeout
}
