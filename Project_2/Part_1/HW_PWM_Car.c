/////////////////////////////////////////////////////////////////////////////
// Course Number: CECS 347
// Assignment: Example project for Hardware PWM controlled Car
// Description: 
/////////////////////////////////////////////////////////////////////////////

//////////////////////1. Pre-processor Directives Section////////////////////
#include "tm4c123gh6pm.h"
#include "PWM.h"
#include "GPIO.h"
#include <stdint.h>
#include "utils.h"
/////////////////////////////////////////////////////////////////////////////

//////////////////////2. Declarations Section////////////////////////////////

////////// Local Global Variables //////////

////////// Function Prototypes //////////
void DisableInterrupts(void);
void EnableInterrupts(void);
void WaitForInterrupt(void);
void Delay(void);
void Timer1A_Init(void);
void Timer1A_Delay(unsigned long delay);
void PortF_Init(void);
void Stop_Car(void);

// Higher prescale = lower reload value
// thus prescale of 249
// 16000000/(249 + 1) = 64000
const long PRESCALE_VALUE = 64000;

volatile uint32_t SW1_Pressed = 0;

//////////////////////3. Subroutines Section/////////////////////////////////
// MAIN: This main is meant for the command configuration of the hc05.
int main(void){ 
  LED_Init();
  Car_Dir_Init();
  PWM_PB76_Init();
	Timer1A_Init();
	PortF_Init();
	
	while(1){
		
		if(SW1_Pressed){
			// 50% duty cycle
			// Moving Foward
			PWM_PB76_Duty(FIFTY_DUTY, FIFTY_DUTY);
			LED = Green;
			WHEEL_DIR = FORWARD;
			PWM0_ENABLE_R |= BOTH_WHEEL; // enable both wheels
			Timer1A_Delay(PRESCALE_VALUE);

			// STOP
			LED = Dark;
			Stop_Car();

			// Enable blue LED, moving backward
			LED = Blue;
			WHEEL_DIR = BACKWARD;
			PWM0_ENABLE_R |= BOTH_WHEEL; // enable both wheels
			Timer1A_Delay(PRESCALE_VALUE);
			
			// stop
			LED = Dark;
			Stop_Car();

			// 20% Duty Cycle: LEFT
			PWM_PB76_Duty(EIGHTY_DUTY, TWENTY_DUTY);
			
			// Forward left turn
			LED = Yellow;
			WHEEL_DIR=FORWARD;
			PWM0_ENABLE_R |= RIGHT_WHEEL; // Enable right wheel
			PWM0_ENABLE_R &= ~LEFT_WHEEL; // Disable left wheel
			Timer1A_Delay(PRESCALE_VALUE);
			
			// stop
			LED = Dark;
			Stop_Car();
			
			
			// 20% Duty Cycle: RIGHT
			PWM_PB76_Duty(TWENTY_DUTY,EIGHTY_DUTY);
			
			// Forward right turn
			LED = Purple;
			WHEEL_DIR=FORWARD;
			PWM0_ENABLE_R &= ~RIGHT_WHEEL; // Disable right wheel
			PWM0_ENABLE_R |= LEFT_WHEEL; // Enable left wheel
			Timer1A_Delay(PRESCALE_VALUE);
			
			// stop
			LED = Dark;
			Stop_Car();

			// 20% Duty Cycle: LEFT
			PWM_PB76_Duty(TWENTY_DUTY, EIGHTY_DUTY);
			
			// Backward left turn
			LED = Yellow;
			WHEEL_DIR = BACKWARD;
			PWM0_ENABLE_R &= ~RIGHT_WHEEL; // Disable right wheel
			PWM0_ENABLE_R |= LEFT_WHEEL; // Enable left wheel
			Timer1A_Delay(PRESCALE_VALUE);
				
			// stop
			LED = Dark;
			Stop_Car();
			
			// 20% Duty Cycle: RIGHT
			PWM_PB76_Duty(EIGHTY_DUTY, TWENTY_DUTY);
			
			// Backward right turn
			LED = Purple;
			WHEEL_DIR=BACKWARD;
			PWM0_ENABLE_R |= RIGHT_WHEEL; // Enable right wheel
			PWM0_ENABLE_R &= ~LEFT_WHEEL; // Disable left wheel
			Timer1A_Delay(PRESCALE_VALUE);

			// stop
			LED = Dark;
			Stop_Car();

			// 50% Duty Cycle: LEFT
			PWM_PB76_Duty(FIFTY_DUTY, FIFTY_DUTY);

			// Left pivot turn
			LED = Cran;
			WHEEL_DIR=LEFTPIVOT;
			PWM0_ENABLE_R |= BOTH_WHEEL; // Enable both wheels
			Timer1A_Delay(PRESCALE_VALUE);

			Stop_Car();
			
			// 50% Duty Cycle: RIGHT
			PWM_PB76_Duty(FIFTY_DUTY, FIFTY_DUTY);

			// right pivot turn
			LED = White;
			WHEEL_DIR=RIGHTPIVOT;
			PWM0_ENABLE_R |= BOTH_WHEEL; // Enable both wheels
			Timer1A_Delay(PRESCALE_VALUE);

			LED = Dark;
			Stop_Car();
			SW1_Pressed = 0;
			}
	}
}

void Stop_Car(void){
	// stop
		LED = Dark;
		PWM0_ENABLE_R &= ~RIGHT_WHEEL; // stop both wheels
		Timer1A_Delay(PRESCALE_VALUE);
}

 void GPIOPortF_Handler(void){   // handles everything with leds and the switch
	// simple debouncing code
	for (uint32_t time=0;time<160000;time++) {}
	
	if (GPIO_PORTF_RIS_R & SW1_MASK) {		
		GPIO_PORTF_ICR_R |= SW1_MASK;  
		SW1_Pressed = 1; // acknowledge flag4: 00010000 for switch 1
	}		
	
}
