#include "tm4c123gh6pm.h"
#include "SysTick.h"
#include <stdint.h>

#define EN_SYSTICK_CC		NVIC_ST_CTRL_CLK_SRC | NVIC_ST_CTRL_INTEN  // used to set inten and clk source of systick
#define PRI3_TOP3_BITS_RESET	0x1FFFFFFFF  // used to clear all priority bits of systick
#define PRI3_TOP3_BITS_SET	0x600000000  // used to set systick priority as 3
#define VALUE_RESET  0  // usde to clear or disable certain definitions
#define period 4000000

// initialize SysTick
void SysTick_Init(void) {	
  NVIC_ST_CTRL_R = 0;         // disable SysTick during setup

  NVIC_SYS_PRI3_R = (NVIC_SYS_PRI3_R&PRI3_TOP3_BITS_RESET)|PRI3_TOP3_BITS_SET; // priority 3
  NVIC_ST_CTRL_R |= NVIC_ST_CTRL_CLK_SRC|NVIC_ST_CTRL_INTEN|NVIC_ST_CTRL_ENABLE; // enable SysTick with core clock and interrupts      
}

void SysTick_Start(void) {	
	NVIC_ST_RELOAD_R = period - 1; 												// countdown from this number to 0
	NVIC_ST_CURRENT_R = VALUE_RESET; 																// clear countdown counter
	NVIC_ST_CTRL_R |= NVIC_ST_CTRL_ENABLE; 								// enable SysTick timer
}

// Systick interrupt handler:
void SysTick_Handler(void) {
	NVIC_ST_CTRL_R &= ~NVIC_ST_CTRL_ENABLE; // clear enable to end countdown
	RED_LED ^= RED_LED_MASK; // toggle PF2: Blue LED
}