// FollowingRobot.c
// Runs TM4C123
// Starter program CECS 347 project 2 - A Follwoing Robot
// by Min He, 03/17/2024

#include "tm4c123gh6pm.h"
#include <stdint.h>
#include "ADC1SS1.h"   // NEED TO USE THIS
#include <stdio.h>
#include "motors.h"			 // GPIO.h == Motors.h
#include "SW_LED.h"		 // GPIO.h == SW_LED.h
#include "PLL.h"
#include "PWM.h"
#include "Timer1A.h"

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

enum robot_modes mode = OBJECT_FOLLOWER;

int main(void){	
	System_Init();
	//LED = 0x0E;
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
	LED_Init();							// Initialize LED in SW_LED.h   G-ibby
	SW_Init();          		// inititlize switches in SW_LED.h
	PWM_PB76_Init();
	Timer1A_Init();
  EnableInterrupts();	
}

// PROJECT 2, PART 3
void object_follower(void){
	uint8_t i;
	unsigned long ahead, frwdright, frwdleft, count, delay;
	delay = 10000;
	//LED = 0x0E; // ENTERS INTO THE GOBJECT FOLLOWER PROPERLY

	//for (i=0;i<10;i++) {
	ReadADCMedianFilter(&ahead, &frwdleft, &frwdright);
	ReadADCMedianFilter(&ahead, &frwdleft, &frwdright);
	ReadADCMedianFilter(&ahead, &frwdleft, &frwdright);
	ReadADCMedianFilter(&ahead, &frwdleft, &frwdright);
	ReadADCMedianFilter(&ahead, &frwdleft, &frwdright);
	ReadADCMedianFilter(&ahead, &frwdleft, &frwdright);
	ReadADCMedianFilter(&ahead, &frwdleft, &frwdright);
	ReadADCMedianFilter(&ahead, &frwdleft, &frwdright);
	ReadADCMedianFilter(&ahead, &frwdleft, &frwdright);
	
	//while(1){
	if(( frwdleft >  ahead ) && ( frwdleft >  frwdright )){
 		switch(frwdleft){
//			case TOO_CLOSE+1 ... MIN_VAL: // if left sensor between 10 cm to 20cm pivot left to face object
//				stop_the_car();
//				LED = 0x02;
//				break;
//			
			case TOO_FAR ... TOO_CLOSE: // if left sensor between 10 cm to 20cm pivot left to face object
				move_left_turn();
				LED = 0x0E;
				break;
//			
//			
//			case MAX_VAL ... TOO_FAR-1: // if left sensor between 10 cm to 20cm pivot left to face object
//				stop_the_car();
//				LED = 0x04;
//				break;
//			
////			default:
////				move_forward();
////				LED = 0x04;
////				break;
////			
		}
	}else if (( frwdright >  ahead ) && ( frwdright >  frwdleft )){
		Timer1A_Delay(delay);//timer for test casing LED values with SENSOR ONLY CONFIG
		switch(frwdright){
//			case TOO_CLOSE+1 ... MIN_VAL: // if left sensor between 10 cm to 20cm pivot left to face object
//				stop_the_car();
//				LED = 0x02;
//				break;
//			
			case TOO_FAR ... TOO_CLOSE: // if right sensor between 10 cm to 20 cm pivot right to face object
				move_right_turn();
				LED = 0x06;
				break;
////			
//			case MAX_VAL ... TOO_FAR-1: // if left sensor between 10 cm to 20cm pivot left to face object
//				stop_the_car();
//				LED = 0x04;
//				break;
//			
////			default:
////				move_forward();
////				LED = 0x04;
////				break;
//		Timer1A_Delay(delay);//timer for test casing LED values with SENSOR ONLY CONFIG
			}
		} else {
			Timer1A_Delay(delay);
			switch(ahead){
			case TOO_CLOSE+1 ... MIN_VAL: // if left sensor between 10 cm to 20cm pivot left to face object
				stop_the_car();
				LED = 0x02; // RED
				//mode = INACTIVE;
				Timer1A_Delay(6769420);
				break;
			
			case FOLLOW_CLOSE+1 ... TOO_CLOSE: // if forward sensor between 15cm to 10 cm back up
				move_backward();
				LED = 0x0A; // YELLOW
				break;
			
			case FOLLOW_FAR ... FOLLOW_CLOSE:	// if forward sensor ahead at 15 cm then stop
				stop_the_car();
				LED = 0x08; // GREEN
				//mode = INACTIVE;
				Timer1A_Delay(6769420);
				break;
			
			case TOO_FAR ... FOLLOW_FAR-1: // if forward sensor between 15 cm to 20 cm go forward
				move_forward();
				LED = 0x0C; // CYAN
				break;
			
			case MAX_VAL ... TOO_FAR-1: // if left sensor between 10 cm to 20cm pivot left to face object
				stop_the_car();
				LED = 0x04; // BLUE
				//mode = INACTIVE;
				Timer1A_Delay(6769420);
				break;
			
//			default:
//				move_forward();
//				LED = 0x04;
//				break;
		} 
		Timer1A_Delay(delay);//timer for test casing LED values with SENSOR ONLY CONFIG
		ReadADCMedianFilter(&ahead, &frwdleft, &frwdright);
	}
}

// PROJECT 2, PART 4
void wall_follower(void){
	LED = 0x00;
}

// L range: 8000,16000,24000,32000,40000,48000,56000,64000,72000
// power:   10%    20%   30%   40%   50%   60%   70%   80%   90%
void GPIOPortF_Handler(void){ // called on touch of either SW1 or SW2
  if(GPIO_PORTF_RIS_R&0x01){  // SW2 touchG
    GPIO_PORTF_ICR_R = 0x01;  // acknowledge flag0
	if(mode == INACTIVE)
		mode = OBJECT_FOLLOWER;
	else if(mode == OBJECT_FOLLOWER)
		mode = WALL_FOLLOWER;

  }
  if(GPIO_PORTF_RIS_R&0x10){  // SW1 touch
    GPIO_PORTF_ICR_R = 0x10;  // acknowledge flag4G
	if(mode == INACTIVE)
		mode = OBJECT_FOLLOWER;
  }
}