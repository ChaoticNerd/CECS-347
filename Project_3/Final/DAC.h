// dac.c
// Runs on LM4F120 + TM4C123
// Program written by: Natasha Kho & Justin Narciso
// Date Created: 3/6/17 
// Last Modified: 12/03/2025
// Hardware connections
/*
	PD2 - LSB
	PD3
	PD4
	PD5 - MSB
*/

#ifndef DAC_H
#define DAC_H
#include <stdint.h>
#define PORTA_2345 0x3C
#define PORTA_2345_BITS 0x00FFFF00

// Header files contain the prototypes for public functions
// this file explains what the module does

// **************DAC_Init*********************
// Initialize 4-bit DAC, called once 
// Input: none
// Output: none
void DAC_Init(void);


// **************DAC_Out*********************
// output to DAC
// Input: 4-bit data, 0 to 15 
// Input=n is converted to n*3.3V/15
// Output: none
void DAC_Out(uint8_t data);

#endif
