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
				//LED = 0x0E; G
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
	LED = 0x0E; // ENTERS INTO THE GOBJECT FOLLOWER PROPERLY
	ReadADCMedianFilter(&ahead, &frwdleft, &frwdright);
	ReadADCMedianFilter(&ahead, &frwdleft, &frwdright);
	ReadADCMedianFilter(&ahead, &frwdleft, &frwdright);
	
	while((ahead > TOO_FAR || frwdright > TOO_FAR || frwdleft > TOO_FAR) && 
		(ahead < TOO_CLOSE || frwdright < TOO_CLOSE || frwdleft < TOO_CLOSE)){//while in range 
		if((ahead > FOLLOW_DIST)&&((ahead > frwdleft)&&(ahead > frwdright))){ // AHEAD CLOSE
			LED = 0x06;
			move_backward();
			
		}else if((ahead < FOLLOW_DIST)&&((ahead > frwdleft)&&(ahead > frwdright))){// AHEAD FAR
			LED = 0x0C; 
			move_forward();
			
		}else if((frwdleft > ahead)&&(frwdleft > frwdright)&& ((frwdleft > TOO_FAR)&&(frwdleft < TOO_CLOSE))){ // TO LEFT(frwdleft < FOLLOW_DIST)
			LED = 0x02;
			move_left_pivot();
			
		}else if((frwdright > ahead)&&(frwdright > frwdleft)&& ((frwdright > TOO_FAR)&&(frwdright < TOO_CLOSE))){ // TO RIGHT(frwdright < FOLLOW_DIST)&&
			LED = 0x04;
			move_right_pivot();
			
////		}else if((ahead > FOLLOW_DIST)&&(ahead < TOO_FAR)){ //NA
////		
////		}else if((ahead < FOLLOW_DIST)&&(ahead > TOO_CLOSE)){ // NA
////		
////		}else if(){
////		
////		}else if(){
////		
////		}else if(){
		} 
//		while((ahead > TOO_FAR || frwdright > TOO_FAR || frwdleft > TOO_FAR) && 
//		(ahead < TOO_CLOSE || frwdright < TOO_CLOSE || frwdleft < TOO_CLOSE)){//while in range 
//		if((frwdleft > ahead)&&(frwdleft > frwdright)&& ((frwdleft > TOO_FAR)&&(frwdleft < TOO_CLOSE))){ // AHEAD CLOSE
//			LED = 0x06;
//			move_left_pivot();
//			
//		}else if((frwdright > ahead)&&(frwdright > frwdleft)&& ((frwdright > TOO_FAR)&&(frwdright < TOO_CLOSE))){// AHEAD FAR
//			LED = 0x0C; 
//			move_right_pivot();
//			
//			}else{
//			LED = 0x08;
//			}
		
			
		if (((ahead>TOO_CLOSE) || (ahead<TOO_FAR))&&((frwdleft>TOO_CLOSE) || (frwdleft<TOO_FAR))&&((frwdright>TOO_CLOSE) || (frwdright<TOO_FAR))){
			stop_the_car(); 
			LED = 0x00;
			mode = INACTIVE;
			break;
		}
		Timer1A_Delay(delay);
		
			
			
		
		ReadADCMedianFilter(&ahead, &frwdleft, &frwdright);
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
  
}

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