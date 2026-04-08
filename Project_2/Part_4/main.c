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
extern void ADC1_SS1_Init(void);

// functions defined in this file
void System_Init(void);
void object_follower(void);
void wall_follower(void);
int direction_sensor(unsigned long,unsigned long,unsigned long);
int locate_wall(unsigned long left,unsigned long right);

enum robot_modes mode = INACTIVE;

unsigned long ahead, right, left, count, direct_switch;

int active = 0;

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
	PWM_PB45_Init();
	Timer1A_Init();
  EnableInterrupts();	
}

// PROJECT 2, PART 3
void object_follower(void){
	LED = LED_BLUE;
		do{
			ReadADCMedianFilter(&ahead, &left, &right);
		}while( (ahead < TOO_FAR || ahead > TOO_CLOSE) && (left < TOO_FAR || left > TOO_CLOSE) && (right < TOO_FAR || right > TOO_CLOSE) && ( mode == OBJECT_FOLLOWER) );
		
		while(mode == OBJECT_FOLLOWER){
			ReadADCMedianFilter(&ahead, &left, &right);
			direct_switch = direction_sensor(ahead,left,right);
		
			switch(direct_switch){
				case 1:
					if((TOO_FAR <= ahead)&&(ahead <= FOLLOW_FAR))
						forward();
					else if ((ahead < TOO_CLOSE)&&(ahead > FOLLOW_CLOSE))
						backward();
				
					else{
						LED = LED_RED;
						stop();
						mode = INACTIVE;
						active = 0;
					}
					break;
					
				case 2:
					if((TOO_FAR < left)&&(left < FOLLOW_FAR))
						left_forward();
					else if ((left < TOO_CLOSE)&&(left > FOLLOW_CLOSE))
						left_back();
					else{
						stop();
						LED = LED_RED;
						mode = INACTIVE;
						active = 0;
					}
					break;
					
				case 3:
					if((TOO_FAR < right)&&(right < FOLLOW_FAR))
						right_forward();
					else if ((right < TOO_CLOSE)&&(right > FOLLOW_CLOSE))
						right_back();
					else{
						stop();
						LED = LED_RED;
						mode = INACTIVE;
						active = 0;
					}
					break;
					
				default:
					forward();
					break;
				}
			Timer1A_Delay(OBJECT_FOLLOWING_DELAY);
		}
}

// PROJECT 2, PART 4
void wall_follower(void){
	LED = LED_GREEN;
	ReadADCMedianFilter(&ahead, &left, &right);
	direct_switch = locate_wall(left, right);

	switch(direct_switch){
		case 1:
			while(mode == WALL_FOLLOWER){
				if ((ahead > TOO_CLOSE)&&(left > TOO_CLOSE)){	 // PIVOT AWAY CORNER: IF AHEAD less than 20cm THEN PIVOT
					right_pivot();
					Timer1A_Delay(PIVOT_DELAY + WALL_FOLLOWING_DELAY);

				}else if (left > FOLLOW_CLOSE){	// BOUNCE AWAY OP: IF LEFT between 10cm and Lo-20cm THEN TURN RIGHT 
					fast_left_forward();
					Timer1A_Delay(PIVOT_DELAY); 
					
				} else if ((left < FOLLOW_CLOSE) && (left > TOO_FAR)){	// BONUNCE BACK OP: IF LEFT between Hi-20cm and 30cm THEN TURN LEFT
					fast_right_forward();
					Timer1A_Delay(WALL_FOLLOWING_DELAY+ PIVOT_DELAY);
					
				} else if ((left < TOO_FAR)&&(ahead < TOO_FAR)&&(right < TOO_FAR)){	// SHARP TURN OP: IF LEFT GREATER THAN 30 cm THEN PERFORM SHARP TURN LEFT
					sharp_left();
					Timer1A_Delay(WALL_FOLLOWING_DELAY+WALL_FOLLOWING_DELAY);

				} else
					stop();
				
				ReadADCMedianFilter(&ahead, &left, &right);
			}
			break;
		case 2:
			while(mode == WALL_FOLLOWER){
				if((ahead > TOO_CLOSE)&&(right > TOO_CLOSE)){ 	// PIVOT AWAY CORNER: IF AHEAD less than lo-20cm THEN PIVOT
					left_pivot();
					Timer1A_Delay(PIVOT_DELAY + WALL_FOLLOWING_DELAY);

				} else if ((right < FOLLOW_CLOSE) && (right > TOO_FAR)){ // BONUNCE BACK OP: IF RIGHT between Hi-20cm and 30cm THEN TURN RIGHT
					fast_left_forward();
					Timer1A_Delay(WALL_FOLLOWING_DELAY+ PIVOT_DELAY);
					
				} else if (right > FOLLOW_CLOSE){	// BOUNCE AWAY OP: IF RIGHT range between 10cm and Lo-20cm THEN TURN LEFT 
					fast_right_forward();
					Timer1A_Delay(PIVOT_DELAY); 

					
				} else if ((right < TOO_FAR)&&(ahead < TOO_FAR)&&(left < TOO_FAR)){	// SHARP TURN OP: IF RIGHT GREATER THAN Hi-20 cm THEN PERFORM SHARP TURN RIGHT
					sharp_right();
					Timer1A_Delay(WALL_FOLLOWING_DELAY+WALL_FOLLOWING_DELAY);

				} else
					stop();
				ReadADCMedianFilter(&ahead, &left, &right);
			}
			break;
	}
}
void GPIOPortF_Handler(void){ // called on touch of either SW1 or SW2
  if(GPIO_PORTF_RIS_R&SW1_MASK){  // SW1 touch
		active = 1;
		if(mode == INACTIVE)
			mode = OBJECT_FOLLOWER;
    GPIO_PORTF_ICR_R |= SW1_MASK;  // acknowledge flag4G
  }   
	
	if(GPIO_PORTF_RIS_R&SW2_MASK){
			if(active == 1){  
				if(mode == WALL_FOLLOWER)
					mode = OBJECT_FOLLOWER;
				else if(mode == OBJECT_FOLLOWER)
					mode = WALL_FOLLOWER;
		}
		GPIO_PORTF_ICR_R |= SW2_MASK;  // acknowledge flag0
  }
}

int locate_wall(unsigned long left,unsigned long right){
	if (left > right)
		return 1; // left 
	else if (left < right)
		return 2; // right 
	return 1; // default left
}

int direction_sensor(unsigned long ahead,unsigned long left,unsigned long right){
	if ((ahead >= left) && (ahead >= right))
		return 1; // ahead 
	else if ((left > ahead) && (left > right))
		return 2; // left 
	else if ((right > ahead) && (right > left))
		return 3; // right 
	return 1;
}