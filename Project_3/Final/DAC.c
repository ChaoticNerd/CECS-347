// dac.c
// Runs on LM4F120 + TM4C123
// Program written by: Natasha Kho & Justin Narciso
// Date Created: 3/6/17 
// Last Modified: 12/03/2025
// Hardware connections
/*
	PD0 - LSB
	PD1
	PD2
	PD3 - MSB
*/

#include <stdint.h>
#include "tm4c123gh6pm.h"
#include "DAC.h"
#include "switch.h"

#define DAC_DATA (*((volatile uint32_t *)0x400040F0))  // PA2-5: output to DAC circuit
  
// Code files contain the actual implemenation for public functions
// this file also contains an private functions and private data

// **************DAC_Init*********************
// Initialize 3-bit DAC, called once 
// DAC bit 2 on PA2 (least significant bit)
// DAC bit 3 on PA3
// DAC bit 4 on PA4
// DAC bit 5 on PA5 (most significant bit)
// Input: none
// Output: none
void DAC_Init(void){  
  SYSCTL_RCGC2_R |= SYSCTL_RCGC2_GPIOA; // activate port D
	while ((SYSCTL_RCGC2_R&SYSCTL_RCGC2_GPIOA)!=SYSCTL_RCGC2_GPIOA){}

  GPIO_PORTA_AMSEL_R &= ~PORTA_2345;      // no analog 
  GPIO_PORTA_PCTL_R &= ~PORTA_2345_BITS; // regular function
  GPIO_PORTA_DIR_R |= PORTA_2345;      // make  PA2-5 out
  GPIO_PORTA_AFSEL_R &= ~PORTA_2345;   // disable alt funct on PA2-5
  GPIO_PORTA_DEN_R |= PORTA_2345;      // enable digital I/O on PA2-5
}


// **************DAC_Out*********************
// output to DAC
// Input: 4-bit data, 0 to 15 
// Input=n is converted to n*3.3V/15
// Output: none
void DAC_Out(uint8_t data){
  DAC_DATA = data;
}
