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

#define LED_BLUE 	0x04
#define LED_RED 	0x02
#define LED_GREEN 0x08
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
int direction_sensor(unsigned long,unsigned long,unsigned long);

enum robot_modes mode = INACTIVE;

int main(void){	
	System_Init();
	LED = LED_RED;
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
	unsigned long ahead, frwdright, frwdleft, count, delay, pivot_delay,direct_switch;
	LED = LED_BLUE;
	delay = 200;

	for (int i=0;i<10;i++) {
		ReadADCMedianFilter(&ahead, &frwdleft, &frwdright);
	}
	
	do{
		ReadADCMedianFilter(&ahead, &frwdleft, &frwdright);
	}while((ahead < TOO_FAR && ahead > TOO_CLOSE) || (frwdleft < TOO_FAR && frwdleft > TOO_CLOSE) ||(frwdright < TOO_FAR && frwdright > TOO_CLOSE) );

	direct_switch = direction_sensor(ahead,frwdleft,frwdright);
	stop_the_car();
	Timer1A_Delay(delay);
	switch(direct_switch){
		case 1:
			if((TOO_FAR < ahead)&&(ahead < FOLLOW_FAR))
				move_forward();
			else if ((ahead < TOO_CLOSE)&&(ahead > FOLLOW_CLOSE))
				move_backward();
			else{
				stop_the_car();
				LED = LED_RED;
//				mode = INACTIVE;
			}
			break;
		case 2:
			if((TOO_FAR < frwdleft)&&(frwdleft < FOLLOW_FAR))
				move_left_back();
			else if ((frwdleft < TOO_CLOSE)&&(frwdleft > FOLLOW_CLOSE))
				move_left_pivot();
			else{
				stop_the_car();
				LED = LED_RED;
//				mode = INACTIVE;
			}
			break;
			
		case 3:
			if((TOO_FAR < frwdright)&&(frwdright < FOLLOW_FAR))
				move_right_turn();
			else if ((frwdright < TOO_CLOSE)&&(frwdright > FOLLOW_CLOSE))
				move_right_back();
			else{
				stop_the_car();
				LED = LED_RED;
//				mode = INACTIVE;
			}
			break;
			default:
				break;
	}
		
		Timer1A_Delay(OBJECT_FOLLOWING_DELAY);//timer for test casing LED values with SENSOR ONLY CONFIG
		//ReadADCMedianFilter(&ahead, &frwdleft, &frwdright);
	//}
}

// PROJECT 2, PART 4
void wall_follower(void){
	LED = LED_GREEN;
}

// L range: 8000,16000,24000,32000,40000,48000,56000,64000,72000
// power:   10%    20%   30%   40%   50%   60%   70%   80%   90%
void GPIOPortF_Handler(void){ // called on touch of either SW1 or SW2
  if(GPIO_PORTF_RIS_R&SW2_MASK){  // SW2 touchG
    GPIO_PORTF_ICR_R = SW2_MASK;  // acknowledge flag0
		if(mode == INACTIVE)
			mode = OBJECT_FOLLOWER;
//		else if(mode == OBJECT_FOLLOWER)
//			mode = WALL_FOLLOWER;
		else 
			mode = OBJECT_FOLLOWER;
  }
	
  if(GPIO_PORTF_RIS_R&SW1_MASK){  // SW1 touch
    GPIO_PORTF_ICR_R = SW1_MASK;  // acknowledge flag4G
		if(mode == INACTIVE)
			mode = OBJECT_FOLLOWER;
  }
}

int direction_sensor(unsigned long u1,unsigned long u2,unsigned long u3){
	if ((u1 >= u2) && (u1>=u3))
		return 1; // ahead w
	else if ((u2> u1) && (u2>u3))
		return 2; // left w
	else if ((u3> u1) && (u3>u2))
		return 3; // right w
	return 1;
}