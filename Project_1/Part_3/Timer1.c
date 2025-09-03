#include "tm4c123gh6pm.h"

// ***************** Timer1_Init ****************
// Activate TIMER1 interrupts to run user task periodically
// Inputs:  task is a pointer to a user function
//          period in units (1/clockfreq)
// Outputs: none

#define RED_LED       (*((volatile unsigned long *)0x40025008))
#define RED_LED_MASK    0x02  // bit position for onboard blue LED

void Timer1A_Init(unsigned long period){
  SYSCTL_RCGCTIMER_R |= 0x02;   // 0) activate TIMER1
  TIMER1_CTL_R = 0x00000000;    // 1) disable TIMER1A during setup
  TIMER1_CFG_R = 0x00000004;    // 2) configure for 16-bit mode
  TIMER1_TAMR_R = 0x00000002;   // 3) configure for periodic down-count mode
  TIMER1_TAILR_R = period-1;    // 4) reload value
  TIMER1_TAPR_R = 0xFF;          // 5) bus clock prescale to 255
  TIMER1_ICR_R = 0x00000001;    // 6) clear TIMER0A timeout flag
  TIMER1_IMR_R = 0x00000001;    // 7) arm timeout interrupt
  NVIC_PRI5_R = (NVIC_PRI5_R&0xFFFF1FFF)|0x0000A000; // 8) enable priority 4
// interrupts enabled in the main program after all devices initialized
// vector number 35, interrupt number 19
  NVIC_EN0_R = 1<<21;           // 9) enable IRQ 19 in NVIC
  TIMER1_CTL_R = 0x00000001;    // 10) enable TIMER1A
}

void Timer1A_Handler(void){	
	RED_LED ^= RED_LED_MASK; // toggle PF1: Red LED
	TIMER1_ICR_R = TIMER_ICR_TATOCINT;// acknowledge TIMER1A timeout
}