// Ultrasonic_Sensor.c
// This program runs on TM4C123.
// PB4 connects to echo pin to generate edge-triggered interrupt.
// PB5 connects to Ultrasonic sensor trigger pin.
// GPTM Timer is used to generate the required timing for trigger pin and measure echo pulse width.

#include <stdint.h>
#include "tm4c123gh6pm.h"
#include "Ultrasonic_Sensor.h"

void PortB_Init(void);


void PortB_Init(void){ 
  SYSCTL_RCGC2_R |= SYSCTL_RCGC2_GPIOB;     // 1) activate clock for Port B
	while ((SYSCTL_RCGC2_R&SYSCTL_RCGC2_GPIOB)!=SYSCTL_RCGC2_GPIOB){}; // wait for clock to start

	GPIO_PORTB_PCTL_R &= ~PORTB_PCTL; 				// 3) regular GPIO
  GPIO_PORTB_AMSEL_R &= ~PORTB_ENABLE_4_5;    // 4) disable analog function on PA2
  GPIO_PORTB_DIR_R &= ~PORTB_4_MASK;        				// 5) PB4:echo pin, input
  GPIO_PORTB_DIR_R |= PORTB_5_MASK;         				// 5) PB5:trigger pin, output
  GPIO_PORTB_AFSEL_R &= ~PORTB_ENABLE_4_5;      				// 6) regular port function
  GPIO_PORTB_DEN_R |= PORTB_ENABLE_4_5;         				// 7) enable digital port
  GPIO_PORTB_IS_R &= ~PORTB_4_MASK;         				// PB4 is edge-sensitive
  GPIO_PORTB_IBE_R |= PORTB_4_MASK;         				// PB4 is both edges
  GPIO_PORTB_ICR_R |= PORTB_4_MASK;          				// clear flag 4
  GPIO_PORTB_IM_R |= PORTB_4_MASK;          				// arm interrupt on PB4
		
  NVIC_PRI0_R = (NVIC_PRI0_R&0xFFFF1FFF)|0x00006000; // (g) priority 3
  NVIC_EN0_R = 0x00000002;          				// (h) enable Port B edge interrupt
}

void PortF_Init(void){ 
  SYSCTL_RCGCGPIO_R |= SYSCTL_RCGC2_GPIOF;     	// activate F clock
	while ((SYSCTL_RCGCGPIO_R&SYSCTL_RCGC2_GPIOF)!=SYSCTL_RCGC2_GPIOF){} // wait for the clock to be ready
		
	GPIO_PORTF_CR_R |= Modify_PF1_4;         		// allow changes to PF4-0 :11111->0x1F     
  GPIO_PORTF_AMSEL_R &= ~Modify_PF1_4;        // disable analog function
  GPIO_PORTF_PCTL_R &= ~PORTF_CLEAR_PCTL; 	// GPIO clear bit PCTL  
  GPIO_PORTF_DIR_R &= ~PORTF_IN;          // PF4,PF0 input   
  GPIO_PORTF_DIR_R |= PORTF_OUT;          	// PF3,PF2,PF1 output   
	GPIO_PORTF_AFSEL_R &= ~Modify_PF1_4;        // no alternate function
  GPIO_PORTF_PUR_R |= PORTF_PUR;          	// enable pullup resistors on PF4,PF0       
  GPIO_PORTF_DEN_R |= Modify_PF1_4;          	// enable digital pins PF4-PF0

	GPIO_PORTF_IS_R &= ~SW1_MASK;														// PF0 is edge-sensitive
	GPIO_PORTF_IBE_R &= ~SW1_MASK;													// PF0 is not both edges
	GPIO_PORTF_IEV_R |= SW1_MASK; 													// PF0 rising edge event 
	GPIO_PORTF_ICR_R |= SW1_MASK; 													// Clear Flag0
	GPIO_PORTF_IM_R |= SW1_MASK;														// Arm interrupt on PF0

	NVIC_EN0_R = PortB_INT;          				// (h) enable Port B edge interrupt

}
