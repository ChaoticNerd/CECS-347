// Project1Part3.c
// Runs on LM4F120/TM4C123
// Use GPTM Timer and Edge Interrupt to use Ultrasonic Sensor
// Ana Arante, Hanna Estrada, Emily Hsu, Suphia Sidiqi
// September 8, 2025

#include "tm4c123gh6pm.h"
#include "Timer1A.h"
#include "PLL.h"
#include <stdint.h>

#define TRIGGER_PIN 						(*((volatile unsigned long *)0x40005200))  // PB7 is the trigger pin	
#define ECHO_PIN 								(*((volatile unsigned long *)0x40005100))  // PB6 is the echo pin	
#define TRIGGER_VALUE 					0x80   			// trigger at bit 7
#define ECHO_VALUE 							0x40   			// trigger at bit 6
//#define ONE_SHOT								0x00000001	// bit position for one-shot mode
//#define ONE_SHOT_RELOAD					10					// 2 us
#define PERIODIC					0x00000002		// bit position for periodic count-down mode
#define PERIODIC_RELOAD		38000					// time from sending ultrasonic to returning 

#define MC_LEN 						0.0625 				// length of one machine cycle in microsecond for 16MHz clock
#define SOUND_SPEED 			0.0343 				// centimeter per micro-second
#define PERIODIC_RELOAD		38000					// time from sending ultrasonic to returning 

extern void PortB_Init(void);
void GPIOPortB_Handler(void);
extern void Timer1A_Init(unsigned long period);
extern uint32_t Timer1A_Stop(void);
extern void Timer1A_Start(unsigned long period);
//extern uint32_t TimeElapsed(void);
extern void DisableInterrupts(void); // Disable interrupts
extern void EnableInterrupts(void);  // Enable interrupts
	
static volatile uint32_t distance=0;
static volatile uint32_t done=0;

 int main(void){ 
  DisableInterrupts();
	PLL_Init();    // bus clock at 16 MHz
	Timer1A_Init(PERIODIC_RELOAD); 	// initialize timer1 
  PortB_Init();
	EnableInterrupts();

  while(1){
		done = 0;
		TRIGGER_PIN &= ~TRIGGER_VALUE;
		Timer1A_Start(10);
		TRIGGER_PIN |= TRIGGER_VALUE;
		Timer1A_Start(2);
		TRIGGER_PIN &= ~TRIGGER_VALUE;
		while(!done);
  }
}


void GPIOPortB_Handler(void){
	if (ECHO_PIN == ECHO_VALUE){  				// echo pin rising edge is detected, start timing
			Timer1A_Start(38000);   // configure for periodic down-count mode
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
		done = 1;
	}
  GPIO_PORTB_ICR_R = 0x40;      // acknowledge flag 6
}