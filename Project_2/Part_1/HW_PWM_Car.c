/////////////////////////////////////////////////////////////////////////////
// Course Number: CECS 347
// Assignment: Example project for Hardware PWM controlled Car
// Description: 
/////////////////////////////////////////////////////////////////////////////

//////////////////////1. Pre-processor Directives Section////////////////////
#include "tm4c123gh6pm.h"
#include "PWM.h"
#include "GPIO.h"
/////////////////////////////////////////////////////////////////////////////

//////////////////////2. Declarations Section////////////////////////////////

////////// Local Global Variables //////////

////////// Function Prototypes //////////
void DisableInterrupts(void);
void EnableInterrupts(void);
void WaitForInterrupt(void);
void Delay(void);

//////////////////////3. Subroutines Section/////////////////////////////////
// MAIN: This main is meant for the command configuration of the hc05.
int main(void){ 
  LED_Init();
  Car_Dir_Init();
  PWM_PB76_Init();
  PWM_PB76_Duty(START_SPEED, START_SPEED);
    
	// moving forward
	LED = Green;
	WHEEL_DIR = FORWARD;
	PWM0_ENABLE_R |= 0x0000000C; // enable both wheels
	Delay();

	// stop
	LED = Dark;
	PWM0_ENABLE_R &= ~0x0000000C; // stop both wheels
	Delay();

	// Enable blue LED,moving backward
	LED = Blue;
	WHEEL_DIR = BACKWARD;
	PWM0_ENABLE_R |= 0x0000000C; // enable both wheels
	Delay();
	
	// stop
	LED = Dark;
	PWM0_ENABLE_R &= ~0x0000000C; // stop both wheels
	Delay();

	// Forward left turn
	LED = Yellow;
	WHEEL_DIR=FORWARD;
	PWM0_ENABLE_R |= 0x00000008; // Enable right wheel
	PWM0_ENABLE_R &= ~0x00000004; // Disable left wheel
	Delay();
	
	// stop
	LED = Dark;
	PWM0_ENABLE_R &= ~0x0000000C; // stop both wheels
	Delay();

	// Forward right turn
	LED = Purple;
	WHEEL_DIR=FORWARD;
	PWM0_ENABLE_R &= ~0x00000008; // Disable right wheel
	PWM0_ENABLE_R |= 0x00000004; // Enable left wheel
	Delay();
	
	// stop
	LED = Dark;
	PWM0_ENABLE_R &= ~0x0000000C; // stop both wheels
	Delay();

	// Backward left turn
	LED = Yellow;
	WHEEL_DIR = BACKWARD;
	PWM0_ENABLE_R &= ~0x00000008; // Disable right wheel
	PWM0_ENABLE_R |= 0x00000004; // Enable left wheel
	Delay();
		
	// stop
	LED = Dark;
	PWM0_ENABLE_R &= ~0x0000000C; // stop both wheels
	Delay();

	// Backward right turn
	LED = Purple;
	WHEEL_DIR=BACKWARD;
	PWM0_ENABLE_R |= 0x00000008; // Enable right wheel
	PWM0_ENABLE_R &= ~0x00000004; // Disable left wheel
	Delay();

	// stop
	LED = Dark;
	PWM0_ENABLE_R &= ~0x0000000C; // stop both wheels
	Delay();

	// Left pivot turn
	LED = Cran;
	WHEEL_DIR=LEFTPIVOT;
	PWM0_ENABLE_R |= 0x0000000C; // Enable both wheels
	Delay();

	// stop
	LED = Dark;
	PWM0_ENABLE_R &= ~0x0000000C; // stop both wheels
	Delay();

	// right pivot turn
	LED = White;
	WHEEL_DIR=RIGHTPIVOT;
	PWM0_ENABLE_R |= 0x0000000C; // Enable both wheels
	Delay();

	LED = Dark;
	PWM0_ENABLE_R &= ~0x0000000C; // stop both wheels
	
  while(1){}
}

// Subroutine to wait 0.25 sec
// Inputs: None
// Outputs: None
// Notes: ...
void Delay(void){
	unsigned long volatile time;
  time = 727240*500/91;  // 0.25sec
  while(time){
		time--;
  }
}
