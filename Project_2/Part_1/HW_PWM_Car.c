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
	Timer1A_Init();
	
	while(1){
		// 50% duty cycle
		PWM_PB76_Duty(FIFTY_DUTY, FIFTY_DUTY);
		
		// moving forward
		LED = Green;
		WHEEL_DIR = FORWARD;
		PWM0_ENABLE_R |= 0x0000000C; // enable both wheels
		Timer1A_Delay(1);

		// stop
		LED = Dark;
		PWM0_ENABLE_R &= ~0x0000000C; // stop both wheels
		Timer1A_Delay(1);

		// Enable blue LED, moving backward
		LED = Blue;
		WHEEL_DIR = BACKWARD;
		PWM0_ENABLE_R |= 0x0000000C; // enable both wheels
		Timer1A_Delay(1);
		
		// stop
		LED = Dark;
		PWM0_ENABLE_R &= ~0x0000000C; // stop both wheels
		Timer1A_Delay(1);

		// 20% Duty Cycle: LEFT
		PWM_PB76_Duty(TWENTY_DUTY, EIGHTY_DUTY);
		
		// Forward left turn
		LED = Yellow;
		WHEEL_DIR=FORWARD;
		PWM0_ENABLE_R |= 0x00000008; // Enable right wheel
		PWM0_ENABLE_R &= ~0x00000004; // Disable left wheel
		Timer1A_Delay(1);
		
		// stop
		LED = Dark;
		PWM0_ENABLE_R &= ~0x0000000C; // stop both wheels
		Timer1A_Delay(1);
		
		
		// 20% Duty Cycle: RIGHT
		PWM_PB76_Duty(EIGHTY_DUTY, TWENTY_DUTY);
		
		// Forward right turn
		LED = Purple;
		WHEEL_DIR=FORWARD;
		PWM0_ENABLE_R &= ~0x00000008; // Disable right wheel
		PWM0_ENABLE_R |= 0x00000004; // Enable left wheel
		Timer1A_Delay(1);
		
		// stop
		LED = Dark;
		PWM0_ENABLE_R &= ~0x0000000C; // stop both wheels
		Timer1A_Delay(1);

		// 20% Duty Cycle: LEFT
		PWM_PB76_Duty(TWENTY_DUTY, EIGHTY_DUTY);
		
		// Backward left turn
		LED = Yellow;
		WHEEL_DIR = BACKWARD;
		PWM0_ENABLE_R &= ~0x00000008; // Disable right wheel
		PWM0_ENABLE_R |= 0x00000004; // Enable left wheel
		Timer1A_Delay(1);
			
		// stop
		LED = Dark;
		PWM0_ENABLE_R &= ~0x0000000C; // stop both wheels
		Timer1A_Delay(1);
		
		// 20% Duty Cycle: RIGHT
		PWM_PB76_Duty(EIGHTY_DUTY, TWENTY_DUTY);
		
		// Backward right turn
		LED = Purple;
		WHEEL_DIR=BACKWARD;
		PWM0_ENABLE_R |= 0x00000008; // Enable right wheel
		PWM0_ENABLE_R &= ~0x00000004; // Disable left wheel
		Timer1A_Delay(1);

		// stop
		LED = Dark;
		PWM0_ENABLE_R &= ~0x0000000C; // stop both wheels
		Timer1A_Delay(1);

		// 50% Duty Cycle: LEFT
		PWM_PB76_Duty(FIFTY_DUTY, FIFTY_DUTY);

		// Left pivot turn
		LED = Cran;
		WHEEL_DIR=LEFTPIVOT;
		PWM0_ENABLE_R |= 0x0000000C; // Enable both wheels
		Timer1A_Delay(1);

		// stop
		LED = Dark;
		PWM0_ENABLE_R &= ~0x0000000C; // stop both wheels
		Timer1A_Delay(1);
		
		// 50% Duty Cycle: RIGHT
		PWM_PB76_Duty(FIFTY_DUTY, FIFTY_DUTY);

		// right pivot turn
		LED = White;
		WHEEL_DIR=RIGHTPIVOT;
		PWM0_ENABLE_R |= 0x0000000C; // Enable both wheels
		Timer1A_Delay(1);

		LED = Dark;
		PWM0_ENABLE_R &= ~0x0000000C; // stop both wheels
	}
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
