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
void PWM_PB45_Init(void){
	if ((SYSCTL_RCGC2_R&SYSCTL_RCGC2_GPIOB)==0) {
		SYSCTL_RCGC2_R |= SYSCTL_RCGC2_GPIOB;	// Activate B clocks
		while ((SYSCTL_RCGC2_R & SYSCTL_RCGC2_GPIOB)==0){};
	}
	
	GPIO_PORTB_AFSEL_R |= PORTB_45_MASK;	// enable alt funct: PB45 for PWM
  GPIO_PORTB_PCTL_R &= ~PORTB_45_PWM1_CLEAR; // PWM to be used
  GPIO_PORTB_PCTL_R |= PORTB_45_PWM1_EN; // PWM to be used
  GPIO_PORTB_DEN_R |= PORTB_45_MASK;	// enable digital I/O 
	
	// Initializes PWM settings
	SYSCTL_RCGCPWM_R |= SYSCTL_RCGCPWM_PWM0;	// activate PWM0
	SYSCTL_RCC_R &= ~PORTB_CLEAR_PREV_PWM_VALUES; // Clear any previous PWM divider values
	
	// PWM0_1 output A&B Initialization for PB54
	PWM0_1_CTL_R = PWM0_1_RESET;	// re-loading down-counting mode  (0_1 refers to module, Generator)  
	PWM0_1_GENA_R |= PWM0_1_GENA_LOWLOAD_HIGH_CMPADOWN;	// low on LOAD, high on CMPA down 
	PWM0_1_GENB_R |= PWM0_1_GENB_LOWLOAD_HIGH_CMPBDOWN;// low on LOAD, high on CMPB down
	PWM0_1_LOAD_R = TOTAL_PERIOD - 1;	// cycles needed to count down to 0 
  PWM0_1_CMPA_R = PWM0_1_RESET;	// count value when output rises
	PWM0_1_CMPB_R = PWM0_1_RESET;	// count value when output rises
	PWM0_1_CTL_R |= PWM0_1_CTL_EN_COUNTDOWN;	// Enable PWM0 Generator 0 in Countdown mode
	PWM0_ENABLE_R &= ~PWM0_PB54_EN;	// Disable PB54:PWM0 output 0&1 on initialization
}


// Dependency: PWM_Init()
// Inputs: 
//	duty_L is the value corresponding to the duty cycle of the left wheel
//	duty_R is the value corresponding to the duty cycle of the right wheel
// Outputs: None 
// Description: Changes the duty cycles of PB76 by changing the CMP registers
void PWM_Duty(unsigned long duty_L, unsigned long duty_R){
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

void forward(void){
	PWM_Duty(TWENTY_DUTY, TWENTY_DUTY);
	WHEEL_DIR = FORWARD;
	PWM0_ENABLE_R |= BOTH_WHEEL; // enable both wheels
}

void stop(void){
	PWM0_ENABLE_R &= ~BOTH_WHEEL; // stop both wheels
}

void backward(void){
	PWM_Duty(TWENTY_DUTY, TWENTY_DUTY);
	WHEEL_DIR = BACKWARD;
	PWM0_ENABLE_R |= BOTH_WHEEL; // enable both wheels
}

void left_pivot(void){
	PWM_Duty(TWENTY_DUTY, TWENTY_DUTY);
	WHEEL_DIR = LEFTPIVOT;
	PWM0_ENABLE_R |= BOTH_WHEEL; // Enable both wheels

}

void right_pivot(void){
	PWM_Duty(TWENTY_DUTY, TWENTY_DUTY);
	WHEEL_DIR = RIGHTPIVOT;
	PWM0_ENABLE_R |= BOTH_WHEEL; // Enable both wheels
}

void right_forward(void){
	PWM_Duty(TWENTY_DUTY, EIGHTY_DUTY);
			
	// Forward right turn
	WHEEL_DIR = FORWARD;
	PWM0_ENABLE_R &= ~RIGHT_WHEEL; // Disable right wheel
	PWM0_ENABLE_R |= LEFT_WHEEL; // Enable left wheel
			
}

void left_forward(void){
	PWM_Duty(EIGHTY_DUTY,TWENTY_DUTY);
			
	// Forward left turn
	WHEEL_DIR = FORWARD;
	PWM0_ENABLE_R |= RIGHT_WHEEL; // Enable right wheel
	PWM0_ENABLE_R &= ~LEFT_WHEEL; // Disable left wheel
			
}

void fast_right_forward(void){
	//           L           R
	PWM_Duty(EIGHTY_DUTY,TWENTY_DUTY);
			
	// Forward right turn
	WHEEL_DIR = FORWARD;
	PWM0_ENABLE_R |= BOTH_WHEEL;
			
}

void fast_left_forward(void){
	//           L           R
	PWM_Duty(TWENTY_DUTY, EIGHTY_DUTY);

	// Forward left turn
	WHEEL_DIR = FORWARD;
	PWM0_ENABLE_R |= BOTH_WHEEL;
			
}

void left_back(void){
	PWM_Duty(EIGHTY_DUTY,TWENTY_DUTY);
	
	// Backward right turn
	WHEEL_DIR = BACKWARD;
	PWM0_ENABLE_R |= RIGHT_WHEEL; // Enable right wheel
	PWM0_ENABLE_R &= ~LEFT_WHEEL; // Disable left wheel
}

void right_back(void){
	PWM_Duty(TWENTY_DUTY,EIGHTY_DUTY);
	
	// Backward right turn
	WHEEL_DIR = BACKWARD;
	PWM0_ENABLE_R &= ~RIGHT_WHEEL; // Disable right wheel
	PWM0_ENABLE_R |= LEFT_WHEEL; // Enable left wheel
}

void sharp_left(void){
	PWM_Duty(SEVENTY_DUTY,THIRTY_DUTY);
			
	// Forward left turn
	WHEEL_DIR = FORWARD;
	PWM0_ENABLE_R |= BOTH_WHEEL; // Disable left wheel
}

void sharp_right(void){
	PWM_Duty(THIRTY_DUTY,SEVENTY_DUTY);
			
	// Forward right turn
	WHEEL_DIR = FORWARD;
	PWM0_ENABLE_R |= BOTH_WHEEL; // Enable both wheel
}