// CECS347 Project 1 Part 1
// Team members: Justin Narciso, Natasha Kho
// Lab description: Creating two traffic lights and a pedestrian light using Moore finite state machine
 //
// Hardware Design
// Port F Will be flashing a the built in Red LED every 0.5 second

#include "tm4c123gh6pm.h"
#include "SysTick.h"
#include "PLL.h"
#include "Timer1.h"

#define RED_LED       (*((volatile unsigned long *)0x40025008))
#define RED_LED_MASK    0x02  // bit pisition for onboard blue LED

void PORTF_Init(void);
void DisableInterrupts(void); // Disable interrupts
void EnableInterrupts(void);  // Enable interrupts
void WaitForInterrupt(void);  // low power mode

int main(void){
	DisableInterrupts();
	PORTF_Init();							// PF1(RED LED) is an output for debugging
  PLL_Init();               // set system clock to 16 MHz
  //SysTick_Init();           // initialize SysTick timer,removed in part2
	Timer1A_Init(40000);			// initialize timer1 (2 Hz), achieved by 16Mhz
	RED_LED = 0x00;
	EnableInterrupts();
	
	
  while(1){
    //SysTick_Wait10ms(25);    // approximately 10*10 ms = 0.1s removed in Part 2
		WaitForInterrupt();
  }
}

void PORTF_Init(void){
	SYSCTL_RCGCGPIO_R |= SYSCTL_RCGCGPIO_R5; // activate port F
	while ((SYSCTL_RCGCGPIO_R&SYSCTL_RCGCGPIO_R5)!=SYSCTL_RCGCGPIO_R5){};
	
	GPIO_PORTF_DIR_R |= RED_LED_MASK; 	// make PF1 out (built-in red LED)
  GPIO_PORTF_AFSEL_R &= ~RED_LED_MASK;// disable alt funct on PF1
  GPIO_PORTF_DEN_R |= RED_LED_MASK; 	// enable digital I/O on PF1                           
  GPIO_PORTF_PCTL_R &= ~0x000000F0; 	// configure PF1 as GPIO
  GPIO_PORTF_AMSEL_R |= RED_LED_MASK; // disable analog functionality on PF  
}
