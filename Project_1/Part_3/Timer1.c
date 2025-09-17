#include "tm4c123gh6pm.h"
#include <stdint.h>
#include "Timer1.h"

// ***************** Timer1_Init ****************
// Activate TIMER1 interrupts to run user task periodically
// Inputs:  task is a pointer to a user function
//          period in units (1/clockfreq)
// Outputs: none
#define ECHO_PIN                             (((volatile unsigned long)0x40005100))
#define TRIGGER_PIN                         (((volatile unsigned long)0x40005200))

#define TRIGGER_VALUE     									0x80   // trigger at bit 7
#define ECHO_VALUE             							0x40   // trigger at bit 6

void Timer1A_Init(unsigned long period, unsigned long setTAMR){
  SYSCTL_RCGCTIMER_R |= 0x02;   // 0) activate TIMER1
  TIMER1_CTL_R = 0x00000000;    // 1) disable TIMER1A during setup
  TIMER1_CFG_R = 0x00000004;    // 2) configure for 16-bit mode
  TIMER1_TAMR_R = setTAMR;			// 3) configure for periodic down-count or one-shot depending on setTAMR
  TIMER1_TAILR_R = period-1;    // 4) reload value
  TIMER1_TAPR_R = 0x63;         // 5) bus clock prescale to 255
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
	uint32_t timeProgressed =(TIMER1_TAILR_R - TIMER1_TAR_R) *(TIMER1_TAPR_R + 1);
	TIMER1_TAR_R = 0x00000000;		// Resets current time
	TIMER1_CTL_R = 0x00000000;    // 1) disable TIMER1A during setup
	return timeProgressed;
}
void Timer1A_OneShot(void){
		TRIGGER_PIN &= ~TRIGGER_VALUE; // send low to trigger
	
		Timer1A_Init(121600, 0x00000001); // 2us down
		while(TIMER1_TAR_R != 0);
		TRIGGER_PIN |= TRIGGER_VALUE; // send high to trigger
		
		Timer1A_Init(608000, 0x00000001); // 10us up
		while(TIMER1_TAR_R != 0);
		TRIGGER_PIN &= ~TRIGGER_VALUE; // send low to trigger
}
/* LED CODE FOR PART 4 PICKUP
void Timer1A_Handler(void){	
	RED_LED ^= RED_LED_MASK; // toggle PF1: Red LED
	TIMER1_ICR_R = TIMER_ICR_TATOCINT;// acknowledge TIMER1A timeout
}
*/