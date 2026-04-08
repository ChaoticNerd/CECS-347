#include "Motors.h"

void Motors_Init(void){
	if ((SYSCTL_RCGC2_R&SYSCTL_RCGC2_GPIOB)==0) {
		SYSCTL_RCGC2_R |= SYSCTL_RCGC2_GPIOB;	// Activate B clocks
		while ((SYSCTL_RCGC2_R&SYSCTL_RCGC2_GPIOB)==0){};
	}
  GPIO_PORTB_AMSEL_R &= ~PORTB_PB2367_MASK;	// disable analog function
	GPIO_PORTB_AFSEL_R &= ~PORTB_PB2367_MASK;	// no alternate function
  GPIO_PORTB_PCTL_R &= ~PORTB_PB2367_BIT_MASK;	// GPIO clear bit PCTL 
	GPIO_PORTB_DIR_R |= PORTB_PB2367_MASK; // output on pin(s)
  GPIO_PORTB_DEN_R |= PORTB_PB2367_MASK;	// enable digital I/O on pin(s)
}
