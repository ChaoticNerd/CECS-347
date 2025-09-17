// Ultrasonic_Sensor.c
// This program runs on TM4C123.
// PB6 connects to echo pin to generate edge-triggered interrupt.
// PB7 connects to Ultrasonic sensor trigger pin.
// GPTM Timer is used to generate the required timing for trigger pin and measure echo pulse width.
// Natasha Kho, Justin Narciso
// September 8, 2025

#include <stdint.h>
#include "tm4c123gh6pm.h"

uint32_t distance;

#define TRIGGER_PIN 			(*((volatile unsigned long *)0x40005200))  // PB7 is the trigger pin	
#define ECHO_PIN 					(*((volatile unsigned long *)0x40005100))  // PB6 is the echo pin	
#define TRIGGER_VALUE 		0x80   				// trigger at bit 7
#define ECHO_VALUE 				0x40   				// echo at bit 6
#define PERIODIC					0x00000002		// bit position for periodic count-down mode
#define MC_LEN 						0.0625 				// length of one machine cycle in microsecond for 16MHz clock
#define SOUND_SPEED 			0.0343 				// centimeter per micro-second
#define PERIODIC_RELOAD		38000					// time from sending ultrasonic to returning 

void PortB_Init(void);
extern void GPIOPortB_Handler(void);
extern void Timer1A_Init(unsigned long period);
extern uint32_t Timer1A_Stop(void);

void PortB_Init(void){ 
  SYSCTL_RCGC2_R |= SYSCTL_RCGC2_GPIOB;     // 1) activate clock for Port B
	while ((SYSCTL_RCGC2_R&SYSCTL_RCGC2_GPIOB)!=SYSCTL_RCGC2_GPIOB){}; // wait for clock to start

	GPIO_PORTB_PCTL_R &= ~0xFF000000; 				// 3) regular GPIO
  GPIO_PORTB_AMSEL_R &= (uint32_t)~0xC0;    // 4) disable analog function on PA2
  GPIO_PORTB_DIR_R &= ~0x40;        				// 5) PB6:echo pin, input
  GPIO_PORTB_DIR_R |= 0x80;         				// 5) PB7:trigger pin, output
  GPIO_PORTB_AFSEL_R &= ~0xC0;      				// 6) regular port function
  GPIO_PORTB_DEN_R |= 0xC0;         				// 7) enable digital port
  GPIO_PORTB_IS_R &= ~0x40;         				// PB6 is edge-sensitive
  GPIO_PORTB_IBE_R |= 0x40;         				// PB6 is both edges
  GPIO_PORTB_IEV_R &= ~0x40;        				// PB6 both edge event
  GPIO_PORTB_ICR_R = 0x40;          				// clear flag 6
  GPIO_PORTB_IM_R |= 0x40;          				// arm interrupt on PB6
  NVIC_PRI0_R = (NVIC_PRI0_R&0xFFFF1FFF)|0x0000A000; // (g) priority 3
  NVIC_EN0_R = 0x00000002;          				// (h) enable Port B edge interrupt
}
/*
void GPIOPortB_Handler(void){
	if (ECHO_PIN==ECHO_VALUE){  				// echo pin rising edge is detected, start timing
			Timer1A_Init(38000);   // configure for periodic down-count mode
	}
	else { 
		// echo pin falling edge is detected, end timing and calculate distance.
    // The following code is based on the fact that the HCSR04 ultrasonic sensor 
    // echo pin will always go low after a trigger with bouncing back
    // or after a timeout. The maximum distance can be detected is 400cm.
		// The speed of sound is approximately 340 meters per second, 
		// or  .0343 c/µS.
    // Distance = (echo pulse width * 0.0343)/2; = ((# of mc)*MC_LEN*SOUND_SPEED)/2
		Timer1A_Stop();
		//while(!ECHO_PIN);
		distance = (uint32_t)(Timer1A_Stop()*MC_LEN*SOUND_SPEED)/2;		
	}
  GPIO_PORTB_ICR_R = 0x40;      // acknowledge flag 6
}*/
