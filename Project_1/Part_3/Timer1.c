#include "tm4c123gh6pm.h"

// ***************** Timer1_Init ****************
// Activate TIMER1 interrupts to run user task periodically
// Inputs:  task is a pointer to a user function
//          period in units (1/clockfreq)
// Outputs: none

void Timer1A_Init(unsigned long period, unsigned long setTAMR){
  SYSCTL_RCGCTIMER_R |= 0x02;   // 0) activate TIMER1
  TIMER1_CTL_R = 0x00000000;    // 1) disable TIMER1A during setup
  TIMER1_CFG_R = 0x00000004;    // 2) configure for 16-bit mode
  TIMER1_TAMR_R = setTAMR;			// 3) configure for periodic down-count mode
  TIMER1_TAILR_R = period-1;    // 4) reload value
  TIMER1_TAPR_R = 0xFF;          // 5) bus clock prescale to 255
  TIMER1_ICR_R = 0x00000001;    // 6) clear TIMER1A timeout flag
  TIMER1_IMR_R = 0x00000001;    // 7) arm timeout interrupt
  NVIC_PRI5_R = (NVIC_PRI5_R&0xFFFF1FFF)|0x0000A000; // 8) enable priority 4
	
// interrupts enabled in the main program after all devices initialized
// vector number 35, interrupt number 19
  NVIC_EN0_R = 1<<21;           // 9) enable IRQ 19 in NVIC
  TIMER1_CTL_R = 0x00000001;    // 10) enable TIMER1A
}

long Timer1A_Stop(void){
	// Stop Timer1A
	TIMER1_CTL_R = 0x00000000;    // 1) disable TIMER1A during setup
	uint32_t timeProgressed =(TIMER1_TAILR_R - TIMER1_TAR_R) *(prescale + 1);
	TIMER1_TAR_R = 0x00000000;		// Resets current time
	return timeProgressed;
}
/* LED CODE FOR PART 4 PICKUP
void Timer1A_Handler(void){	
	RED_LED ^= RED_LED_MASK; // toggle PF1: Red LED
	TIMER1_ICR_R = TIMER_ICR_TATOCINT;// acknowledge TIMER1A timeout
}
*/