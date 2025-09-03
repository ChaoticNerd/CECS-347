#include "tm4c123gh6pm.h"
#include <stdint.h>

#define TEN_MS 160000  // reload value to generate 10ms for system clock 50MHz.

// Initialize SysTick with busy wait running at bus clock.
void SysTick_Init(void){
  NVIC_ST_CTRL_R = 0;                   // disable SysTick during setup
  NVIC_ST_CTRL_R |= NVIC_ST_CTRL_CLK_SRC; // set SysTick timer to use core clock: 50MHz
}

// Time delay using busy wait.
// This assumes 50 MHz system clock.
// Input: 16-bit interger for multiple of 10ms
void SysTick_Wait10ms(uint16_t delay){	
	NVIC_ST_RELOAD_R = TEN_MS*delay-1;
  NVIC_ST_CURRENT_R = 0;                // any write to current clears it                                        
  NVIC_ST_CTRL_R |= NVIC_ST_CTRL_ENABLE; // enable SysTick timer
	
	// wait for COUNT bit in control register to be raised.
	while ((NVIC_ST_CTRL_R&NVIC_ST_CTRL_COUNT)==0) {} 
  NVIC_ST_CTRL_R &= ~NVIC_ST_CTRL_ENABLE; // disable SysTick timer
}
