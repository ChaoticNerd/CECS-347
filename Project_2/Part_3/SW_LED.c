#include "SW_LED.h"

// Port F  LED Initialization
void LED_Init(void){ 
	SYSCTL_RCGC2_R |= SYSCTL_RCGC2_GPIOF;	// Activate F clocks
	while ((SYSCTL_RCGC2_R&SYSCTL_RCGC2_GPIOF)==0){};
		
  GPIO_PORTF_AMSEL_R &= ~0x0E;      // 3) disable analog function
  GPIO_PORTF_PCTL_R &= ~0x0000FFF0; // 4) GPIO clear bit PCTL  
  GPIO_PORTF_DIR_R |= 0x0E;         // 6) PF1-PF3 output
  GPIO_PORTF_AFSEL_R &= ~0x0E;      // 7) no alternate function     
  GPIO_PORTF_DEN_R |= 0x0E;         // 8) enable digital pins PF3-PF1
  //LED = Dark;                       // Turn off all LEDs.
}

// Port F SW2 Initialization + Interrupt
void SW_Init(void){ 
  SYSCTL_RCGCGPIO_R |= SYSCTL_RCGC2_GPIOF;     	// activate F clock
	while ((SYSCTL_RCGCGPIO_R&SYSCTL_RCGC2_GPIOF)!=SYSCTL_RCGC2_GPIOF){} // wait for the clock to be ready
		
	GPIO_PORTF_CR_R |= PORTF_IN;         		// allow changes to PF4-0 :11111->0x1F     
  GPIO_PORTF_AMSEL_R &= ~PORTF_IN;        // disable analog function
  GPIO_PORTF_PCTL_R &= ~PORTF_CLEAR_PCTL; 	// GPIO clear bit PCTL  
  GPIO_PORTF_DIR_R &= ~PORTF_IN;          // PF4,PF0 input   
  GPIO_PORTF_DIR_R |= PORTF_OUT;          	// PF3,PF2,PF1 output   
	GPIO_PORTF_AFSEL_R &= ~PORTF_IN;        // no alternate function
  GPIO_PORTF_PUR_R |= PORTF_PUR;          	// enable pullup resistors on PF4,PF0       
  GPIO_PORTF_DEN_R |= PORTF_IN;          	// enable digital pins PF4-PF0

	GPIO_PORTF_IS_R &= ~SW1_MASK;														// PF0 is edge-sensitive
	GPIO_PORTF_IBE_R &= ~SW1_MASK;													// PF0 is not both edges
	GPIO_PORTF_IEV_R |= SW1_MASK; 													// PF0 rising edge event 
	GPIO_PORTF_ICR_R |= SW1_MASK; 													// Clear Flag0
	GPIO_PORTF_IM_R |= SW1_MASK;														// Arm interrupt on PF0

	NVIC_EN0_R = PORTF_IRQ;   				// (h) enable Port F edge interrupt
}