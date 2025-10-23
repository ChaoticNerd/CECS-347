/////////////////////////////////////////////////////////////////////////////
// Course Number: CECS 347
// Assignment: Project 2, Part 1
// @author: Justin Narciso
// @author: Natasha Kho
// @date 10/1/2025
// CSULB Computer Engineering
// Description: Contains the initialization for the PWM hardware module on 
// 							the tm4c. Also has the function to control the duty
//							cycles.
/////////////////////////////////////////////////////////////////////////////

//////////////////////1. Pre-processor Directives Section////////////////////
#include "tm4c123gh6pm.h"
#include "PWM.h"

////////// Local Global Variables //////////
/////////////////////////////////////////////////////////////////////////////

//////////////////////3. Subroutines Section/////////////////////////////////
// Dependency: None
// Inputs: None
// Outputs: None
// Description: 
// Initializes the PWM module 0 generator 1 outputs A&B tied to PB54 to be used with the 
//		L298N motor driver allowing for a variable speed of robot car. 
void PWM_PB76_Init(void){
	if ((SYSCTL_RCGC2_R&SYSCTL_RCGC2_GPIOB)==0) {
		SYSCTL_RCGC2_R |= SYSCTL_RCGC2_GPIOB;	// Activate B clocks
		while ((SYSCTL_RCGC2_R & SYSCTL_RCGC2_GPIOB)==0){};
	}
	
	GPIO_PORTB_AFSEL_R |= 0x30;	// enable alt funct: PB45 for PWM
  GPIO_PORTB_PCTL_R &= ~0x00FF0000; // PWM to be used
  GPIO_PORTB_PCTL_R |= 0x00440000; // PWM to be used
  GPIO_PORTB_DEN_R |= 0x30;	// enable digital I/O 
	
	// Initializes PWM settings
	SYSCTL_RCGCPWM_R |= 0x01;	// activate PWM0
	SYSCTL_RCC_R &= ~0x001E0000; // Clear any previous PWM divider values
	
	// PWM0_1 output A&B Initialization for PB54
	PWM0_1_CTL_R = 0;	// re-loading down-counting mode  (0_1 refers to module, Generator)  
	PWM0_1_GENA_R |= 0xC8;	// low on LOAD, high on CMPA down 
	PWM0_1_GENB_R |= 0xC08;// low on LOAD, high on CMPB down
	PWM0_1_LOAD_R = TOTAL_PERIOD - 1;	// cycles needed to count down to 0 
  PWM0_1_CMPA_R = 0;	// count value when output rises
	PWM0_1_CMPB_R = 0;	// count value when output rises
	
	PWM0_1_CTL_R |= 0x00000001;	// Enable PWM0 Generator 0 in Countdown mode
	PWM0_ENABLE_R &= ~0x0000000C;	// Disable PB54:PWM0 output 0&1 on initialization
}


// Dependency: PWM_Init()
// Inputs: 
//	duty_L is the value corresponding to the duty cycle of the left wheel
//	duty_R is the value corresponding to the duty cycle of the right wheel
// Outputs: None 
// Description: Changes the duty cycles of PB76 by changing the CMP registers
void PWM_PB76_Duty(unsigned long duty_L, unsigned long duty_R){
	PWM0_1_CMPA_R = duty_L - 1;	// PB4 count value when output rises
  PWM0_1_CMPB_R = duty_R - 1;	// PB5 count value when output rises
}
/////////////////////////////////////////////////////////////////////////////
/*
PB7 - R SLP
PB6 - R DIR
PB3 - L SLP
PB2 - L DIR

*/

void move_forward(void){
			PWM_PB76_Duty(TWENTY_DUTY, TWENTY_DUTY);
			WHEEL_DIR = FORWARD;
			PWM0_ENABLE_R |= BOTH_WHEEL; // enable both wheels
}

void stop_the_car(void){
		PWM0_ENABLE_R &= ~BOTH_WHEEL; // stop both wheels
}

void move_backward(void){
			PWM_PB76_Duty(TWENTY_DUTY, TWENTY_DUTY);
			WHEEL_DIR = BACKWARD;
			PWM0_ENABLE_R |= BOTH_WHEEL; // enable both wheels
}

void move_left_pivot(void){
			PWM_PB76_Duty(THIRTY_DUTY, THIRTY_DUTY);
			WHEEL_DIR=LEFTPIVOT;
			PWM0_ENABLE_R |= BOTH_WHEEL; // Enable both wheels

}

void move_right_pivot(void){
			PWM_PB76_Duty(THIRTY_DUTY, THIRTY_DUTY);
			WHEEL_DIR=RIGHTPIVOT;
			PWM0_ENABLE_R |= BOTH_WHEEL; // Enable both wheels
}

void move_left_turn(void){
PWM_PB76_Duty(EIGHTY_DUTY, TWENTY_DUTY);
			
			// Forward left turn
			WHEEL_DIR=FORWARD;
			PWM0_ENABLE_R |= RIGHT_WHEEL; // Enable right wheel
			PWM0_ENABLE_R &= ~LEFT_WHEEL; // Disable left wheel
			
}
			
void move_right_turn(void){
			// 20% Duty Cycle: RIGHT
			PWM_PB76_Duty(TWENTY_DUTY,EIGHTY_DUTY);
			
			// Forward right turn
			WHEEL_DIR=FORWARD;
			PWM0_ENABLE_R &= ~RIGHT_WHEEL; // Disable right wheel
			PWM0_ENABLE_R |= LEFT_WHEEL; // Enable left wheel
			}