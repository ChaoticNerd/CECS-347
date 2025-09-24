// PWM.c
// Runs on TM4C123
// Use PWM0/PB6 and PWM1/PB7 to generate pulse-width modulated outputs.
// Daniel Valvano
// March 28, 2014
// Modified by Min He, September 7, 2021

/* This example accompanies the book
   "Embedded Systems: Real Time Interfacing to ARM Cortex M Microcontrollers",
   ISBN: 978-1463590154, Jonathan Valvano, copyright (c) 2014
  Program 6.7, section 6.3.2

 Copyright 2014 by Jonathan W. Valvano, valvano@mail.utexas.edu
    You may use, edit, run or distribute this file
    as long as the above copyright notice remains
 THIS SOFTWARE IS PROVIDED "AS IS".  NO WARRANTIES, WHETHER EXPRESS, IMPLIED
 OR STATUTORY, INCLUDING, BUT NOT LIMITED TO, IMPLIED WARRANTIES OF
 MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE APPLY TO THIS SOFTWARE.
 VALVANO SHALL NOT, IN ANY CIRCUMSTANCES, BE LIABLE FOR SPECIAL, INCIDENTAL,
 OR CONSEQUENTIAL DAMAGES, FOR ANY REASON WHATSOEVER.
 For more information about my classes, my research, and my books, see
 http://users.ece.utexas.edu/~valvano/
 */
#include <stdint.h>
#include "tm4c123gh6pm.h"

#define DIRECTION (*((volatile unsigned long *)0x4002403C))
#define FORWARD 		0x0F	//1111
#define BACKWARD 		0x0A	//1010

#define LIGHT (*((volatile unsigned long *)0x40025038))
#define RED 0x02
#define GREEN 0x08
#define BLUE 0x04

#define STOP 1
#define SPEED_35 3500
#define SPEED_60 6000
#define SPEED_80 8000
#define SPEED_98 9800

uint16_t DUTY = STOP;

// configure the system to get its clock from the PLL
void PLL_Init(void){
  // 0) configure the system to use RCC2 for advanced features
  //    such as 400 MHz PLL and non-integer System Clock Divisor
  SYSCTL_RCC2_R |= SYSCTL_RCC2_USERCC2;
  // 1) bypass PLL while initializing
  SYSCTL_RCC2_R |= SYSCTL_RCC2_BYPASS2;
  // 2) select the crystal value and oscillator source
  SYSCTL_RCC_R &= ~SYSCTL_RCC_XTAL_M;   // clear XTAL field
  SYSCTL_RCC_R += SYSCTL_RCC_XTAL_16MHZ;// configure for 16 MHz crystal
  SYSCTL_RCC2_R &= ~SYSCTL_RCC2_OSCSRC2_M;// clear oscillator source field
  SYSCTL_RCC2_R += SYSCTL_RCC2_OSCSRC2_MO;// configure for main oscillator source
  // 3) activate PLL by clearing PWRDN
  SYSCTL_RCC2_R &= ~SYSCTL_RCC2_PWRDN2;
  // 4) set the desired system divider and the system divider least significant bit
  SYSCTL_RCC2_R |= SYSCTL_RCC2_DIV400;  // use 400 MHz PLL
  SYSCTL_RCC2_R = (SYSCTL_RCC2_R&~0x1FC00000) // clear system clock divider field
                  + (7<<22);      // configure for 50 MHz clock (400MHz / (7+1) = 50MHz)
  // 5) wait for the PLL to lock by polling PLLLRIS
  while((SYSCTL_RIS_R&SYSCTL_RIS_PLLLRIS)==0){};
  // 6) enable use of PLL by clearing BYPASS
  SYSCTL_RCC2_R &= ~SYSCTL_RCC2_BYPASS2;
}

// period is 16-bit number of PWM clock cycles in one period 
// Output on PB6/M0PWM0
void PWM0A_Init(uint16_t period){
  SYSCTL_RCGCPWM_R |= 0x01;             // 1) activate PWM0
  SYSCTL_RCGCGPIO_R |= 0x02;            // 2) activate port B: 000010
  while((SYSCTL_RCGCGPIO_R&0x02) == 0){};
	GPIO_PORTB_AFSEL_R |= 0x40;           // enable alt funct on PB6: 0100 0000
  GPIO_PORTB_PCTL_R &= ~0x0F000000;     // configure PB6 as PWM0
  GPIO_PORTB_PCTL_R |= 0x04000000;
  GPIO_PORTB_AMSEL_R &= ~0x40;          // disable analog functionality on PB6
  GPIO_PORTB_DEN_R |= 0x40;             // enable digital I/O on PB6
  GPIO_PORTB_DR8R_R |= 0xC0;    // enable 8 mA drive on PB6,7
  SYSCTL_RCC_R = 0x00100000 |           // 3) use PWM divider
    (SYSCTL_RCC_R & (~0x001E0000));   //    configure for /2 divider: PWM clock: 80Mhz/2=40MHz
  PWM0_0_CTL_R = 0;                     // 4) re-loading down-counting mode
  PWM0_0_GENA_R = 0xC8;                 // low on LOAD, high on CMPA down
  // PB6 goes low on LOAD
  // PB6 goes high on CMPA down
  PWM0_0_LOAD_R = period - 1;           // 5) cycles needed to count down to 0
  PWM0_0_CMPA_R = 0;             // 6) count value when output rises
  PWM0_0_CTL_R |= 0x00000001;           // 7) start PWM0
  PWM0_ENABLE_R |= 0x00000001;          // enable PB6/M0PWM0
}
// change duty cycle of PB6
// duty is number of PWM clock cycles output is high  
void PWM0A_Duty(uint16_t duty){
  PWM0_0_CMPA_R = duty - 1;             // 6) count value when output rises
}
// period is 16-bit number of PWM clock cycles in one period 
// Output on PB7/M0PWM1
void PWM0B_Init(uint16_t period){
  volatile unsigned long delay;
  SYSCTL_RCGCPWM_R |= 0x01;             // 1) activate PWM0
  SYSCTL_RCGCGPIO_R |= 0x02;            // 2) activate port B
  delay = SYSCTL_RCGCGPIO_R;            // allow time to finish activating
  GPIO_PORTB_AFSEL_R |= 0x80;           // enable alt funct on PB7
  GPIO_PORTB_PCTL_R &= ~0xF0000000;     // configure PB7 as M0PWM1
  GPIO_PORTB_PCTL_R |= 0x40000000;
  GPIO_PORTB_AMSEL_R &= ~0x80;          // disable analog functionality on PB7
  GPIO_PORTB_DEN_R |= 0x80;             // enable digital I/O on PB7
  SYSCTL_RCC_R |= SYSCTL_RCC_USEPWMDIV; // 3) use PWM divider
  SYSCTL_RCC_R &= ~SYSCTL_RCC_PWMDIV_M; //    clear PWM divider field
  SYSCTL_RCC_R += SYSCTL_RCC_PWMDIV_2;  //    configure for /2 divider
  PWM0_0_CTL_R = 0;                     // 4) re-loading down-counting mode
  PWM0_0_GENB_R = (PWM_0_GENB_ACTCMPBD_ONE|PWM_0_GENB_ACTLOAD_ZERO); // 0xC08
  // PB7 goes low on LOAD
  // PB7 goes high on CMPB down
  PWM0_0_LOAD_R = period - 1;           // 5) cycles needed to count down to 0: LAB2: PF1->M1PWM5:PWM1_2_
  PWM0_0_CMPB_R = 0;             // 6) count value when output rises: Lab 2:5%2=1->CMPB, GENB
  PWM0_0_CTL_R |= 0x00000001;           // 7) start PWM0: odd->B, even->A
  PWM0_ENABLE_R |= 0x00000002;          // enable PB7/M0PWM1
}
// change duty cycle of PB7
// duty is number of PWM clock cycles output is high  
void PWM0B_Duty(uint16_t duty){
  PWM0_0_CMPB_R = duty - 1;             // 6) count value when output rises
}

// Subroutine to initialize port E pins PE0-3 for output
// PE0-3 control direction of motor
// Inputs: None
// Outputs: None
void PortE_Init(void){
	SYSCTL_RCGC2_R |= SYSCTL_RCGC2_GPIOE; //activate E clock
	while ((SYSCTL_RCGC2_R&SYSCTL_RCGC2_GPIOE)!= SYSCTL_RCGC2_GPIOE){} //wait for clk
	
	GPIO_PORTE_AMSEL_R &= ~0x0F; //disable analog function
	GPIO_PORTE_PCTL_R &= ~0x0000FFFF; //GPIO clear bit PCTL
	GPIO_PORTE_DIR_R |= 0x0F; //PE0-3 output
	GPIO_PORTE_AFSEL_R &= ~0x0F; //no alternate function
	GPIO_PORTE_DEN_R |= 0x0F; //enable digital pins PE0-3
}

// Initilize port F and arm PF4, PF0 for falling edge interrupts
void SwitchLED_Init(void){  
	unsigned long volatile delay;
  SYSCTL_RCGC2_R |= SYSCTL_RCGC2_GPIOF; // (a) activate clock for port F
  delay = SYSCTL_RCGC2_R;
  GPIO_PORTF_LOCK_R = GPIO_LOCK_KEY; // unlock GPIO Port F
  GPIO_PORTF_CR_R |= 0x1F;         // allow changes to PF4,0
  GPIO_PORTF_DIR_R &= ~0x11;    // (c) make PF4,0 in (built-in button)
  GPIO_PORTF_DIR_R |= 0x0E;     // make PF1-3 out
  GPIO_PORTF_AFSEL_R &= ~0x11;  //     disable alt funct on PF4,0
  GPIO_PORTF_DEN_R |= 0x1F;     //     enable digital I/O on PF4,0
  GPIO_PORTF_PCTL_R &= ~0x000FFFFF; //  configure PF4,0 as GPIO
  GPIO_PORTF_AMSEL_R &= ~0x1F;  //     disable analog functionality on PF4,0
  GPIO_PORTF_PUR_R |= 0x11;     //     enable weak pull-up on PF4,0
  GPIO_PORTF_IS_R &= ~0x11;     // (d) PF4,PF0 is edge-sensitive
  GPIO_PORTF_IBE_R &= ~0x11;    //     PF4,PF0 is not both edges
  GPIO_PORTF_IEV_R |= 0x11;    //     PF4,PF0 rising edge event
  GPIO_PORTF_ICR_R = 0x11;      // (e) clear flags 4,0
  GPIO_PORTF_IM_R |= 0x11;      // (f) arm interrupt on PF4,PF0
  NVIC_PRI7_R = (NVIC_PRI7_R&0xFF1FFFFF)|0x00400000; // (g) bits:23-21 for PORTF, set priority to 5
  NVIC_EN0_R |= 0x40000000;      // (h) enable interrupt 30 in NVIC
}

// PORTF ISR:
// Change delivered power based on switch press: 
// sw1: increase 10% until reach 90%
// sw2: decrease 10% until reach 10%
void GPIOPortF_Handler(void){ // called on touch of either SW1 or SW2
	int TENMS = 727240*20/91; //that's actually 20ms not 10
	while(TENMS) TENMS--;
	
  if(GPIO_PORTF_RIS_R&0x01){  // SW2 touched 
    GPIO_PORTF_ICR_R = 0x01;  // acknowledge flag0
		if(DIRECTION == FORWARD){
			DIRECTION = BACKWARD;
			if(DUTY != STOP) GPIO_PORTF_DATA_R = BLUE; 			//make LED blue
		} else if (DIRECTION == BACKWARD){
			DIRECTION = FORWARD;
			if(DUTY != STOP) GPIO_PORTF_DATA_R = GREEN; 		//make LED green
		}
  }
	
  if(GPIO_PORTF_RIS_R&0x10){  // SW1 touch
    GPIO_PORTF_ICR_R = 0x10;  // acknowledge flag4
		switch(DUTY){							// update duty cycle value
			case STOP: DUTY = SPEED_35;
				break;
			case SPEED_35: DUTY = SPEED_60;
				break;
			case SPEED_60: DUTY = SPEED_80;
				break;
			case SPEED_80: DUTY = SPEED_98;
				break;
			default: DUTY = STOP;
		}
		if(DUTY != STOP){					//update led color
			if(DIRECTION == FORWARD)
				GPIO_PORTF_DATA_R = GREEN;
			if(DIRECTION == BACKWARD)
				GPIO_PORTF_DATA_R = BLUE;
		} else 
			GPIO_PORTF_DATA_R = RED;
		
		PWM0A_Duty(DUTY);				// update duty cycle
		PWM0B_Duty(DUTY);
  }
}




