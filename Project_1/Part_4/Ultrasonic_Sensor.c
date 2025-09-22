// Ultrasonic_Sensor.c
// This program runs on TM4C123.
// PB6 connects to echo pin to generate edge-triggered interrupt.
// PB7 connects to Ultrasonic sensor trigger pin.
// GPTM Timer is used to generate the required timing for trigger pin and measure echo pulse width.

#include <stdint.h>
#include "tm4c123gh6pm.h"

uint32_t distance;

#define ECHO_PIN 					(*((volatile unsigned long *)0x40005040))  // PB4 is the echo pin	
#define ECHO_VALUE 				0x10   				// echo at bit 4
#define MC_LEN 						0.0625 				// length of one machine cycle in microsecond for 16MHz clock
#define SOUND_SPEED 			0.0343 				// centimeter per micro-second

void PortB_Init(void);

void PortB_Init(void){ 
  SYSCTL_RCGC2_R |= SYSCTL_RCGC2_GPIOB;     // 1) activate clock for Port B
	while ((SYSCTL_RCGC2_R&SYSCTL_RCGC2_GPIOB)!=SYSCTL_RCGC2_GPIOB){}; // wait for clock to start

	GPIO_PORTB_PCTL_R &= ~0x00FF0000; 				// 3) regular GPIO
  GPIO_PORTB_AMSEL_R &= ~0x30;    // 4) disable analog function on PA2
  GPIO_PORTB_DIR_R &= ~0x10;        				// 5) PB4:echo pin, input
  GPIO_PORTB_DIR_R |= 0x20;         				// 5) PB5:trigger pin, output
  GPIO_PORTB_AFSEL_R &= ~0x30;      				// 6) regular port function
  GPIO_PORTB_DEN_R |= 0x30;         				// 7) enable digital port
  GPIO_PORTB_IS_R &= ~0x10;         				// PB4 is edge-sensitive
  GPIO_PORTB_IBE_R |= 0x10;         				// PB4 is both edges
  GPIO_PORTB_ICR_R |= 0x10;          				// clear flag 4
  GPIO_PORTB_IM_R |= 0x10;          				// arm interrupt on PB4
  NVIC_PRI0_R = (NVIC_PRI0_R&0xFFFF1FFF)|0x00006000; // (g) priority 3
  NVIC_EN0_R = 0x00000002;          				// (h) enable Port B edge interrupt
}
