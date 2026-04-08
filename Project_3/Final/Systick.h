// Systick.h
// Runs on TM4C123
// CECS 347 Project 2 - Space Invaders
// Group number: 15
// Group members: Natasha Kho, Justin Narciso

#include "tm4c123gh6pm.h"
#include <stdint.h>

#define TEN_MS 8000000  // reload value to generate 10ms for system clock 80MHz.
#define ONE_MS 800000

void SysTick_Init(void);

void SysTick_WaitNs(int delay);
void SysTick_Stop(void);
void SysTick_Start(void);