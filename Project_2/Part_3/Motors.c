#include "Motors.h"

void Motors_Init(void){
	if ((SYSCTL_RCGC2_R&SYSCTL_RCGC2_GPIOB)==0) {
		SYSCTL_RCGC2_R |= SYSCTL_RCGC2_GPIOB;	// Activate B clocks
		while ((SYSCTL_RCGC2_R&SYSCTL_RCGC2_GPIOB)==0){};
	}
		
  GPIO_PORTB_AMSEL_R &= ~0xCC;	// disable analog function
	GPIO_PORTB_AFSEL_R &= ~0xCC;	// no alternate function
  GPIO_PORTB_PCTL_R &= ~0xFF00FF00;	// GPIO clear bit PCTL 
	GPIO_PORTB_DIR_R |= 0xCC; // output on pin(s)
  GPIO_PORTB_DEN_R |= 0xCC;	// enable digital I/O on pin(s)
}
