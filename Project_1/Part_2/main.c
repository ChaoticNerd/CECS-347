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
	PORTF_Init();							// PF2(Blue LED) is an output for debugging
  PLL_Init();               // set system clock to 50 MHz
  //SysTick_Init();           // initialize SysTick timer
	Timer1A_Init(15625);		// initialize timer1 (1 Hz)
	RED_LED = 0x00;
	EnableInterrupts();
	
	
  while(1){
    //SysTick_Wait10ms(25);    // approximately 10*10 ms = 0.1s
		WaitForInterrupt();
  }
}

void PORTF_Init(void){
	SYSCTL_RCGCGPIO_R |= SYSCTL_RCGCGPIO_R5; // activate port F
	while ((SYSCTL_RCGCGPIO_R&SYSCTL_RCGCGPIO_R5)!=SYSCTL_RCGCGPIO_R5){};
	
	GPIO_PORTF_DIR_R |= RED_LED_MASK; // make PF2 out (built-in blue LED)
  GPIO_PORTF_AFSEL_R &= ~RED_LED_MASK;// disable alt funct on PF2
  GPIO_PORTF_DEN_R |= RED_LED_MASK; // enable digital I/O on PF2                            
  GPIO_PORTF_PCTL_R &= ~0x000000F0; // configure PF2 as GPIO
  GPIO_PORTF_AMSEL_R |= RED_LED_MASK;   // disable analog functionality on PF  
}
