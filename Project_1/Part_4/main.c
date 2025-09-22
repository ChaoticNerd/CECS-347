// utils. h
// Runs on LM4F120/TM4C123
// Use GPTM Timer and Edge Interrupt to use Ultrasonic Sensor
// @author Natasha Kho
// @author Justin Narciso
// September 22, 2025

#include "tm4c123gh6pm.h"
#include "Timer1A.h"
#include "PLL.h"
#include "UART0.h"
#include "utils.h"
#include <stdint.h>
#include <stdio.h>

#define ECHO_PIN 					(*((volatile unsigned long *)0x40005040))  // PB4 is the echo pin	
#define TRIGGER_PIN 			(*((volatile unsigned long *)0x40005080))  // PB5 is the trigger pin	
#define LEDS							(*((volatile unsigned long *)0x40025038))
#define RED_LED						(*((volatile unsigned long *)0x40025008))
#define SW1								(*((volatile unsigned long *)0x40025040)) // connected to PF4
#define RED_LED_MASK			0x02
#define BLUE_LED_MASK			0x04
#define GREEN_LED_MASK		0x08
#define LED_MASK					0x0E
#define ECHO_VALUE				0x10
#define TRIGGER_VALUE 		0x20   			
#define PERIODIC_RELOAD		38000					// time from sending ultrasonic to returning 
#define MC_LEN 						0.0625 				// length of one machine cycle in microsecond for 16MHz clock
#define SOUND_SPEED 			0.0343 				// centimeter per micro-second
#define QUARTER_SEC				4000000
#define MAX_STR_LEN 			50

static volatile uint32_t distance=0;
static volatile uint32_t done=0;

 int main(void){ 
	uint8_t str[MAX_STR_LEN]; 
	
	DisableInterrupts();
  PLL_Init();    			// bus clock at 16 MHz
	Timer1A_Init(); 		// initialize Timer1 
  PortB_Init();				// initialize PortB
	PortF_Init();
	SysTick_Init(QUARTER_SEC);
	SysTick_Disable();
	UART0_Init();
	UART0_OutCRLF();
	EnableInterrupts();
	 
	 
  while(1){
		if (!(SW1 & 0x10)) {
			done = 0;
			LEDS = 0;
			//LEDS |= LED_MASK;
			TRIGGER_PIN &= ~TRIGGER_VALUE;
			Timer1A_Start(2);
			while((TIMER1_TAR_R) != 0){}
			TRIGGER_PIN |= TRIGGER_VALUE;
			Timer1A_Start(10);		
			while((TIMER1_TAR_R) != 0){}
			TRIGGER_PIN &= ~TRIGGER_VALUE;
			while(!done);		
			WaitForInterrupt();
			sprintf((char *)str, "Current distance is: %d cm\n\r", distance);
			UART0_OutString(str); 
			Timer1A_Start(500000);
		}
  }
}

void GPIOPortB_Handler(void){
	if (ECHO_PIN==ECHO_VALUE){  				// echo pin rising edge is detected, start timing
		Timer1A_Start(38000);   // configure for periodic down-count mode
	}
	else { 
		// echo pin falling edge is detected, end timing and calculate distance.
    // The following code is based on the fact that the HCSR04 ultrasonic sensor 
    // echo pin will always go low after a trigger with bouncing back
    // or after a timeout. The maximum distance can be detected is 400cm.
		// The speed of sound is approximately 340 meters per second,?
		// or ?.0343 c/?S.
    // Distance = (echo pulse width * 0.0343)/2; = ((# of mc)*MC_LEN*SOUND_SPEED)/2
		Timer1A_Stop();
		distance = (uint32_t)(Timer1A_Stop()*MC_LEN*SOUND_SPEED)/2;		
		done=1;
	}
  GPIO_PORTB_ICR_R = 0x10;      // acknowledge flag 6
		if ((70<distance)&&(distance<100)){
				SysTick_Init(0);
			LEDS = BLUE_LED_MASK;
	}else if ((10<distance)&&(distance<70)){
				SysTick_Init(0);
		LEDS = GREEN_LED_MASK;
	}else if (distance<10){
		SysTick_Init(QUARTER_SEC);
	}else if (distance > 100){
				SysTick_Init(0);
		LEDS &= ~LED_MASK;
	}
}

// Interrupt service routine
// Executed every 62.5ns*(period)
void SysTick_Handler(void){
	if (distance < 10){
		LEDS ^= RED_LED_MASK;
	}
}