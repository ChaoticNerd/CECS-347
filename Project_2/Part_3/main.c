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
	delay = 2500;
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

	//}
	while(1){
			if (ahead > TOO_FAR || frwdleft > TOO_FAR || frwdright > TOO_FAR) // if any sensor within 20 cm 
	{
		stop_the_car();
		
		if ((ahead > TOO_CLOSE && frwdleft > TOO_CLOSE && frwdright > TOO_CLOSE)) // reverse if all sensor is close
		{
			LED = 0x02;
			move_backward();
		}
		else if ((frwdleft > TOO_CLOSE && ahead > TOO_CLOSE) || (frwdleft > TOO_CLOSE)) // if left sensor is close to obstacle, turn left
		{
			LED = 0x08;
			move_left_turn();
		}
		else if ((frwdright > TOO_CLOSE && ahead > TOO_CLOSE) || (frwdright > TOO_CLOSE)) // if right sensor is close to obstacle, turn right
		{
			LED = 0x04;
			move_right_turn();
		}
		else if (ahead > TOO_FAR)
		{
			/*
			if (frwdleft > IR20CM)
			{
			LED = Green;
			PWM_PB76_Duty(9000, 9000);
			WHEEL_DIR = RIGHTPIVOT;
			PWM0_ENABLE_R &= ~0x00000002; // Disable right wheel
			PWM0_ENABLE_R |= 0x00000001; // Enable left wheel
			}
			else if (frwdright > IR20CM)
			{
			LED = Blue;
			PWM_PB76_Duty(9000, 9000);
			WHEEL_DIR = LEFTPIVOT;
			PWM0_ENABLE_R |= 0x00000002; // Enable right wheel
			PWM0_ENABLE_R &= ~0x00000001; // Disable left wheel				
			}
			*/
			/*
			else 
			{
			LED = Red;
			PWM_PB76_Duty(8000, 8000);
			WHEEL_DIR = BACKWARD;
			PWM0_ENABLE_R |= 0x00000003;
			}
			*/
		}
	}
	else if (ahead < TOO_FAR && frwdleft < TOO_FAR && frwdright < TOO_FAR) // stop when sensor greater than 80 cm
	{
		LED = 0x06;
		stop_the_car();
		//mode = INACTIVE;
	}
	else // if any sensor is not close go straight
	{
		LED = 0x00;
		move_forward();
	}
	}
}
	
//	
//	
//	// Calibrate the sensor
//	for (i=0;i<10;i++) {
//		ReadADCMedianFilter(&ahead, &frwdleft, &frwdright);
//	}
//	
//  // wait until an Gobstacle is in the right distant range.G
// 	do {
//		//LED = 0x02;
//		ReadADCMedianFilter(&ahead, &frwdleft, &frwdright);  // read one value
//		//ADC1_InSeq1(&ahead, &frwdleft, &frwdright);
//		//LED = 0x04;

//	} while ((ahead<TOO_CLOSE) || (ahead>TOO_FAR));
//	LED = 0x08;
//	// CHANGE WHILE CONDITIOINS, and IF STATEMENTS CONDITIONS oh Guck... 
//		
//  while (ahead < TOO_FAR || frwdright < TOO_FAR || frwdleft < TOO_FAR && 
//		ahead > TOO_CLOSE || frwdright > TOO_CLOSE || frwdleft > TOO_CLOSE) {				//IF Left, or Right, or Forward in read is within rage too close and too far				

//		
//		if (ahead > FOLLOW_DIST && (ahead > frwdright || ahead > frwdleft)) { // IF forward > (Fixed distance), && Forward > Left Or Right 
//			move_backward();
//			LED = 0x02;
//		}else if (ahead < FOLLOW_DIST && (ahead > frwdright || ahead > frwdleft)){ // IF forward < (Fixed distance), && Forward > Left Or Right 
//			move_forward();
//			LED = 0x08;
//		}else if(frwdright > ahead || frwdright > frwdleft){ // IF RightG > Forward OR LEFT then pivot right UNTIL FORWARD  > than RIGHT
//			move_right_pivot();
//			LED = 0x06;
//		}else if(frwdleft > ahead || frwdleft > frwdright){ //IF LEFTG > Forawrd or RIGHT then Pivot left until FORWARD > THan LEFT
//			move_left_pivot();
//			LED = 0x0C;
//		}else { // right distance, stop
//			stop_the_car(); 
//			//LED = 0x00;
//		}
//		ReadADCMedianFilter(&ahead, &frwdleft, &frwdright);
//  }
  


// PROJECT 2, PART 4
void wall_follower(void){
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