//////////////////////1. Pre-processor Directives Section////////////////////
#include "tm4c123gh6pm.h"
/////////////////////////////////////////////////////////////////////////////

//////////////////////2. Declarations Section////////////////////////////////
////////// Constants //////////

////////// Local Global Variables //////////
/////////////////////////////////////////////////////////////////////////////

////////// Prototypes //////////
// Description: Initializes PB2376 for use with L298N motor driver direction
void Motors_Init(void);

void follow_me(void);
void stop_the_car(void);
void move_backward(void);
void move_forward(void);

//////////////////////3. Subroutines Section/////////////////////////////////
void Motors_Init(void){
	if ((SYSCTL_RCGC2_R&SYSCTL_RCGC2_GPIOB)==0) {
		SYSCTL_RCGC2_R |= SYSCTL_RCGC2_GPIOB;	// Activate B clocks
		while ((SYSCTL_RCGC2_R&SYSCTL_RCGC2_GPIOB)==0){};
	}
		
  GPIO_PORTB_AMSEL_R &= ~0xCC;	// disable analog function
	GPIO_PORTB_AFSEL_R &= ~0xCC;	// no alternate function
  GPIO_PORTB_PCTL_R &= ~0xFF00FF00;	// GPIO clear bit PCTL 
	GPIO_PORTB_DIR_R |= 0xCC; // output on pin(s)
  GPIO_PORTB_DEN_R |= 0xCC;	// enable digital I/O on pin(s)
}

void move_forward(void){
			PWM_PB76_Duty(FIFTY_DUTY, FIFTY_DUTY);
			WHEEL_DIR = FORWARD;
			PWM0_ENABLE_R |= BOTH_WHEEL; // enable both wheels
}

void stop_the_car(void){
		PWM0_ENABLE_R &= ~BOTH_WHEEL; // stop both wheels
}

void move_backward(void){
			WHEEL_DIR = BACKWARD;
			PWM0_ENABLE_R |= BOTH_WHEEL; // enable both wheels
}
