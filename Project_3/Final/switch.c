// switches.c
// Runs on TM4C123, starter file for space invader game
// Group number: 15
// Group members: Natasha Kho, Justin Narciso

#include "tm4c123gh6pm.h"
#include <stdint.h>
#include "switch.h"

// Initialize the onboard two switches.
void Switch_Init(void){
	SYSCTL_RCGC2_R |= SYSCTL_RCGC2_GPIOF;	// Activate F clocks
	while ((SYSCTL_RCGC2_R&SYSCTL_RCGC2_GPIOF)==0){};
	
	GPIO_PORTF_LOCK_R = PORTF_UNLOCK_PF0;
	GPIO_PORTF_CR_R |= PORTF_IN;         		// allow changes to PF1&4 
  GPIO_PORTF_AMSEL_R &= ~PORTF_IN;        // disable analog function
  GPIO_PORTF_PCTL_R &= ~PORTF_CLEAR_PCTL; 	// GPIO clear bit PCTL  
	GPIO_PORTF_DIR_R = PORTF_OUT;          	// PF0,4 input ; PF1,2,3 Output
	GPIO_PORTF_AFSEL_R &= ~PORTF_IN;        // no alternate function
  GPIO_PORTF_PUR_R = PORTF_PUR;          	// enable pullup resistors on PF4,PF0       
  GPIO_PORTF_DEN_R |= PORTF_IN;          	// enable digital pins PF4 & PF0

	GPIO_PORTF_IS_R &= ~(SW1_MASK|SW2_MASK);													// PF0 is edge-sensitive
	GPIO_PORTF_IBE_R &= ~(SW1_MASK|SW2_MASK);													// PF0 is not both edges
	GPIO_PORTF_IEV_R |= (SW1_MASK|SW2_MASK); 													// PF0 rising edge event 
	GPIO_PORTF_ICR_R |= (SW1_MASK|SW2_MASK); 													// Clear Flag0
	GPIO_PORTF_IM_R |= (SW1_MASK|SW2_MASK);														// Arm interrupt on PF0

	NVIC_PRI7_R = (NVIC_PRI7_R&0xFF1FFFFF)|0x00400000; // bit 23-21 for PortF, set priority to 2
		// Enable Port F interrupt (IRQ 30) without disturbing other NVIC enables
		NVIC_EN0_R |= PORTF_IRQ;    		// set bit 30 to enable Port F in NVIC
}
