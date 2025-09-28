// Project1Part4.c
// Runs on LM4F120/TM4C123
// Use GPTM Timer and Edge Interrupt to use Ultrasonic Sensor
// Justin Narciso, Natasha Kho

#include "tm4c123gh6pm.h"
#include "Timer1A.h"
#include "PLL.h"
#include "UART0.h"
#include <stdint.h>
#include <stdio.h>
#include "SysTick.h"


////constant declaration to access port f register
#define SWITCH       (*((volatile unsigned long *)0x40025040))
#define LED       	 (*((volatile unsigned long *)0x40025038))


#define ECHO_PIN 					(*((volatile unsigned long *)0x40005040))  // PB4 is the echo pin	
#define TRIGGER_PIN 			(*((volatile unsigned long *)0x40005080))  // PB5 is the trigger pin	
#define ECHO_VALUE				0x10
#define TRIGGER_VALUE 		0x20   			
#define PULSE_DETECT_RELOAD		38000					// time from sending ultrasonic to returning 
#define TWO_US						2
#define TEN_US						10
#define MC_LEN 						0.0625 				// length of one machine cycle in microsecond for 16MHz clock
#define SOUND_SPEED 			0.0343 				// centimeter per micro-second
#define SW1_MASK		 	0x10          // onboard switch PF4
#define RED             0x02
#define BLUE					  0x04
#define GREEN						0x08
#define OFF 						0x00
#define MAX_STR_LEN 50 // length of char string

#define MIN_DISTANCE	10
#define	MID_DISTANCE	70
#define MAX_DISTANCE	100

#define PORTB_ICR		0x10

// UART STUFF
//void delay(void);

#define QUART_SEC 4000000  // reload value to generate 10ms for system clock 50MHz.
void SysTick_Handler(void);
extern void PortB_Init(void);
extern void PortF_Init(void);
extern void Timer1A_Init(void);
extern void Timer1A_Start(unsigned long period);
extern uint32_t Timer1A_Stop(void);
extern void EnableInterrupts(void);
extern void DisableInterrupts(void);
extern void WaitForInterrupt(void);  // low power mode
void SysTick_Disable(void);
void SysTick_Init(uint32_t period);
void Timer1A_Delay(unsigned long);

 volatile uint32_t SW1_Press = 0;


static volatile uint32_t distance=0;
static volatile uint32_t done=0;
volatile uint32_t counts = 0;

unsigned long In;

void delay(void);
uint8_t str[MAX_STR_LEN];

 int main(void){ 
	DisableInterrupts();
  PLL_Init();    		// bus clock at 16 MHz
	Timer1A_Init(); 	// initialize timer1 
  PortB_Init();
	PortF_Init();
	UART0_Init();
	UART0_OutCRLF();
	//SysTick_Init(QUART_SEC);
	EnableInterrupts();	

  while(1){
		SW1_Press = 0;
		while(!SW1_Press);

		while(!done);
		distance = (uint32_t)(Timer1A_Stop()*MC_LEN*SOUND_SPEED)/2;	
		
		if ((distance > MID_DISTANCE) && (distance <= MAX_DISTANCE)) {
			LED = BLUE;
			sprintf((char *)str, "The current distance is: %d cm.\n\r  ", distance);
			UART0_OutString(str);
			
		} else if ((distance > MIN_DISTANCE )&& (distance <= MID_DISTANCE)){
			LED = GREEN;
			sprintf((char *)str, "The current distance is: %d cm.\n\r  ", distance);
			UART0_OutString(str);

		}  else if ( distance <= MIN_DISTANCE) {
			LED = RED;
			SysTick_Init(QUART_SEC);
			sprintf((char *)str, "The current distance is: %d cm.\n\r  ", distance);
			UART0_OutString(str);
		}
		else {
				//SysTick_Init(0);
				LED = OFF;
				sprintf((char *)str, "OUT OF RANGE ! \n\r");
				UART0_OutString(str);			
	} 
		//SW1_Press = 0;
	
	}
}
 
 void GPIOPortF_Handler(void){   // handles everything with leds and the switch
	// simple debouncing code
	for (uint32_t time=0;time<160000;time++) {}
		
	SysTick_Disable();
	
	done = 0;
	counts = 0;
	
	TRIGGER_PIN &= ~TRIGGER_VALUE;
	Timer1A_Delay(2); // input 2, prescale 15 (+1) ==> 32 / 16Mhz =  2US
	
	TRIGGER_PIN |= TRIGGER_VALUE;
	Timer1A_Delay(10);	// input 10, prescale 15 (+1) ==> 160 / 16Mhz =  2US

	TRIGGER_PIN &= ~TRIGGER_VALUE;
	
	if (GPIO_PORTF_RIS_R & SW1_MASK) {		
		GPIO_PORTF_ICR_R |= SW1_MASK;  
		SW1_Press = 1; // acknowledge flag4: 00010000 for switch 1
	}		
	
}
	
void GPIOPortB_Handler(void){    // port b handles all distance measurements due to pulses
	if (ECHO_PIN==ECHO_VALUE){  				// echo pin rising edge is detected, start timing
		Timer1A_Start(PULSE_DETECT_RELOAD);  		// configure for periodic down-count mode
		//WaitForInterrupt();  

		
	}
	else { 
		// echo pin falling edge is detected, end timing and calculate distance.
    // The following code is based on the fact that the HCSR04 ultrasonic sensor 
    // echo pin will always go low after a trigger with bouncing back
    // or after a timeout. The maximum distance can be detected is 400cm.
		// The speed of sound is approximately 340 meters per second,?
		// or ?.0343 c/?S.
    // Distance = (echo pulse width * 0.0343)/2; = ((# of mc)*MC_LEN*SOUND_SPEED)/2
		Timer1A_Stop();
		

		done=1;
		
	}
	GPIO_PORTB_ICR_R = PORTB_ICR;      // acknowledge flag 4
}

void SysTick_Handler(void) {
		if (distance < MIN_DISTANCE) {
		LED ^= RED;
	} 
}
	