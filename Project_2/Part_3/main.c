// FollowingRobot.c
// Runs TM4C123
// Starter program CECS 347 project 2 - A Follwoing Robot
// by Min He, 03/17/2024

#include "tm4c123gh6pm.h"
#include <stdint.h>
#include "ADC1SS1.h"   // NEED TO USE THIS
#include "motors.h"			 // GPIO.h == Motors.h
#include "SW_LED.h"		 // GPIO.h == SW_LED.h
#include "PLL.h"
#include "PWM.h"

enum robot_modes {INACTIVE, OBJECT_FOLLOWER, WALL_FOLLOWER};

// Function prototypes
// external functions
extern void DisableInterrupts(void);
extern void EnableInterrupts(void);  // Enable interrupts
extern void WaitForInterrupt(void);  // low power mode

// functions defined in this file
void System_Init(void);
void object_follower(void);
void wall_follower(void);

enum robot_modes mode = INACTIVE;

int main(void){	
	System_Init();
	
  while(1){
		switch (mode) {
			case OBJECT_FOLLOWER:
				object_follower();
			  break;
			case WALL_FOLLOWER:
				wall_follower();
			  break;
			default:
				WaitForInterrupt();
			  break;				
		}
  }
}

void System_Init(void){
	DisableInterrupts();
  PLL_Init();             // set system clock to 16 MHz 
	ADC1_SS1_Init();       	// Initialize ADC1 Sample sequencer 1 
	Motors_Init();
	LED_Init();							// Initialize LED in SW_LED.h
  SW_Init();          		// inititlize switches in SW_LED.h
  EnableInterrupts();	
}

void object_follower(void){
	uint8_t i;
  volatile uint16_t ADCvalue;
	
	// Calibrate the sensor
	for (i=0;i<10;i++) {
    ADCvalue = ReadADCMedianFilter();  // read one value
	} 
	
  // wait until an obstacle is in the right distant range.
 	do {
			ADCvalue = ReadADCMedianFilter();  // read one value
	} while ((ADCvalue>TOO_CLOSE) || (ADCvalue<TOO_FAR));
		
  while ((ADCvalue<TOO_CLOSE) && (ADCvalue>TOO_FAR)) {								
		ADCvalue = ReadADCMedianFilter();
		
		if (ADCvalue>FOLLOW_DIST) { // negative logic: too close, move back
			move_backward();
		}
		else if (ADCvalue<FOLLOW_DIST){ // negative logic: too far, move forward
			move_forward();
		}
		else { // right distance, stop
			stop_the_car();
		}
  }	
}

void wall_follower(void){
}

