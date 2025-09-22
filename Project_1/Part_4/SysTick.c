// SysTick.c
// Runs on LM4F120 or TM4C123
// Use the SysTick timer to request interrupts at a particular period.

#include "tm4c123gh6pm.h"
#include <stdint.h> // C99 data types

#define RED_LED					(*((volatile unsigned long *)0x40025008))
#define LEDS						(*((volatile unsigned long *)0x40025038))
#define RED_LED_MASK    0x02  																		// bit position for onboard red LED : PF
#define BLUE_LED_MASK		0x04
#define GREEN_LED_MASK	0x08
#define LED_MASK				0x0E
#define QUARTER_SEC			4000000

// functions defined in this file
void SysTick_Init(uint32_t period);
void SysTick_Disable(void);
void PortF_Init(void);

// **************SysTick_Init*********************
// Initialize SysTick periodic interrupts
// Input: interrupt period
//        Units of period are 62.5ns (assuming 16 MHz clock)
//        Maximum is 2^24-1
//        Minimum is determined by length of ISR
// Output: none
void SysTick_Init(uint32_t period){
  NVIC_ST_CTRL_R = 0;         // disable SysTick during setup
  NVIC_ST_RELOAD_R = period-1;// reload value: if period = 4
  NVIC_ST_CURRENT_R = 0;      // any write to current clears it
  NVIC_SYS_PRI3_R = (NVIC_SYS_PRI3_R&0x1FFFFFFF)|0x80000000; // priority 4
  NVIC_ST_CTRL_R |= NVIC_ST_CTRL_CLK_SRC|NVIC_ST_CTRL_INTEN|NVIC_ST_CTRL_ENABLE; // enable SysTick with core clock and interrupts      
}

void SysTick_Disable(void){
	NVIC_ST_CTRL_R = 0;
}

void PortF_Init(void) {
  SYSCTL_RCGCGPIO_R |= SYSCTL_RCGCGPIO_R5;     	// activate F clock
	while ((SYSCTL_RCGCGPIO_R&SYSCTL_RCGCGPIO_R5)!=SYSCTL_RCGCGPIO_R5){} // wait for the clock to be ready
		
  GPIO_PORTF_DIR_R |= LED_MASK;   // make PF1-3 output 
	GPIO_PORTF_DIR_R &= 0x10; // set pf4 SW1 to input
  GPIO_PORTF_AFSEL_R &= ~0x1E;// disable alt funct on PF1-3
  GPIO_PORTF_DEN_R |= 0x1E;   // enable digital I/O on PF1-3
																	// configure PF1-3 as GPIO
  GPIO_PORTF_PCTL_R &= ~0x000FFFF0;
	GPIO_PORTF_PUR_R |= 0x10; // enable pull upresistor on pf4
  GPIO_PORTF_AMSEL_R |= 0x1E;     // disable analog functionality on PF1-3
}