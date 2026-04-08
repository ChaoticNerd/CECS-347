// Systick.c
// Runs on TM4C123
// CECS 347 Project 2 - Space Invaders
// Group number: 15
// Group members: Natasha Kho, Justin Narciso
#include "Systick.h"

// Runs for 10 Hz
void SysTick_Init(void){
  NVIC_ST_CTRL_R = 0;         // disable SysTick during setup
  NVIC_ST_RELOAD_R = TEN_MS-1;// reload value: if period = 4
  NVIC_ST_CURRENT_R = 0;      // any write to current clears it
  NVIC_ST_CTRL_R |= NVIC_ST_CTRL_CLK_SRC | NVIC_ST_CTRL_INTEN | NVIC_ST_CTRL_ENABLE; // enable SysTick with core clock and interrupts 
	
}
void SysTick_Start(void){
  NVIC_ST_RELOAD_R = TEN_MS-1;// reload value: if period = 4
  NVIC_ST_CURRENT_R = 0;      // any write to current clears it
	NVIC_ST_CTRL_R |= (NVIC_ST_CTRL_ENABLE|NVIC_ST_CTRL_INTEN);
}

void SysTick_Stop(void){
	NVIC_ST_CTRL_R &= ~(NVIC_ST_CTRL_ENABLE|NVIC_ST_CTRL_INTEN); // disable SysTick timer
}

// Time delay using busy wait.
//// This assumes 80 MHz system clock.
// Input: 16-bit interger for multiple of 10ms
void SysTick_Wait10ms(void){	
	NVIC_ST_RELOAD_R = ONE_MS-1;
  NVIC_ST_CURRENT_R = 0;                // any write to current clears it                                        
  NVIC_ST_CTRL_R |= NVIC_ST_CTRL_ENABLE; // enable SysTick timer
	
	// wait for COUNT bit in control register to be raised.
	while ((NVIC_ST_CTRL_R&NVIC_ST_CTRL_COUNT)==0) {} 
  NVIC_ST_CTRL_R &= ~NVIC_ST_CTRL_ENABLE; // disable SysTick timer
}

void SysTick_WaitNs(int delay){
	while(delay){
		SysTick_Wait10ms();
		delay--;
	}
}