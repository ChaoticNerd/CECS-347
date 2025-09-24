// Project1Part4.c
// SysTick.c
// Runs on LM4F120/TM4C123
// Use GPTM Timer and Edge Interrupt to use Ultrasonic Sensor
// Justin Narciso, Natasha Kho
// September 18, 2025

#include "tm4c123gh6pm.h"
#include <stdint.h>
#include "SysTick.h"

// Assume system clock is 16MHz: t=1600000*(1/f)=1600000*(1/16000000)=0.1s

#define QUART_SEC 4000000  // reload value to generate 10ms for system clock 50MHz.

// Initialize SysTick with busy wait running at bus clock.
//void SysTick_Init(void){
//  NVIC_ST_CTRL_R = 0;                   // disable SysTick during setup
//  NVIC_ST_CTRL_R |= NVIC_ST_CTRL_CLK_SRC; // set SysTick timer to use core clock: 50MHz
//	
//}

// Time delay using busy wait.
// This assumes 16 MHz system clock.
// Input: 16-bit integer for multiple of 0.25s
//void SysTick_WaitQuartSec(uint16_t delay){	
//	NVIC_ST_RELOAD_R = QUART_SEC*delay-1;
//  NVIC_ST_CURRENT_R = 0;                // any write to current clears it                                        
//  NVIC_ST_CTRL_R |= NVIC_ST_CTRL_ENABLE; // enable SysTick timer
//	
//	// wait for COUNT bit in control register to be raised.
//	while ((NVIC_ST_CTRL_R&NVIC_ST_CTRL_COUNT)==0) {} 
//  NVIC_ST_CTRL_R &= ~NVIC_ST_CTRL_ENABLE; // disable SysTick timer
//}

// Initialize SysTick with busy wait running at bus clock.
void SysTick_Init(uint32_t period){
	NVIC_ST_CTRL_R = SYSTICK_CLEAR;         // disable SysTick during setup
  NVIC_ST_RELOAD_R = period-1;// reload value: if period = 4
  NVIC_ST_CURRENT_R = SYSTICK_CLEAR;      // any write to current clears it
  NVIC_SYS_PRI3_R = (NVIC_SYS_PRI3_R&SYSTICK_PRICLEAR)|SYSTICK_SETPRI4; // priority 4
  NVIC_ST_CTRL_R |= NVIC_ST_CTRL_CLK_SRC|NVIC_ST_CTRL_INTEN|NVIC_ST_CTRL_ENABLE; // enable SysTick with core clock and interrupts      
}

void SysTick_Disable(void){
	NVIC_ST_CTRL_R = SYSTICK_CLEAR;
}

