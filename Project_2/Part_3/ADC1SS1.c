/////////////////////////////////////////////////////////////////////////////
// Assignment: Project 2, Part 2
// @author: Justin Narciso
// @author: Natasha Kho
// @date 10/06/2025
// CSULB Computer Engineering
// Description: Initializes ADC1 SS1
// 							
/////////////////////////////////////////////////////////////////////////////

// This file provide initialization function for two analog channels:
// PE2/AIN1 and PD0/AIN7

#include "tm4c123gh6pm.h"
#include <stdint.h>
#include "ADC1SS1.h"
#include "SW_LED.h"
// This initialization function sets up the ADC according to the
// following parameters.  Any parameters not explicitly listed
// below are not modified:
// Max sample rate: <=125,000 samples/second
// Sequencer 0 priority: 1st (highest)
// Sequencer 1 priority: 2nd
// Sequencer 2 priority: 3rd
// Sequencer 3 priority: 4th (lowest)
// SS3 triggering event: software trigger
// SS3 1st sample source: Ain1 (PE2)
// SS3 interrupts: flag set on completion but no interrupt requested
void ADC1_SS1_Init(void){ 
  // SYSCTL_RCGCGPIO_R |= SYSCTL_RCGCGPIO_R4;   // 1) activate clock for Port E
  // SYSCTL_RCGCADC_R |= 0x00000010;    // activate ADC 1
	// while ((SYSCTL_RCGCGPIO_R&SYSCTL_RCGCGPIO_R4)!=SYSCTL_RCGCGPIO_R4){}

	// GPIO_PORTE_DIR_R &= ~0x07;      // 2) make PE0-2 input
  // GPIO_PORTE_AFSEL_R |= 0x07;     // 3) enable alternate function on PE0-2
  // GPIO_PORTE_DEN_R &= ~0x07;      // 4) disable digital I/O on PE0-2
  // GPIO_PORTE_AMSEL_R |= 0x07;     // 5) enable analog function on PE0-2
	// 	// OK 2 THINGS, RCGC0 is a generalized legacy thing that was split up, leading to the creation of RCGADC
  // //SYSCTL_RCGC0_R |= 0x001000000;   // 6) activate ADC1 (LEGACY)
  
  // // we don't give the ports set up time, need the while loop

  // delay = SYSCTL_RCGCGPIO_R;      // 2) allow time for clock to stabilize
  // delay = SYSCTL_RCGCGPIO_R;      // I dont know what these do
	// // while ((SYSCTL_RCGC0_R&0x00100000)!=0x00100000){}
         
  // SYSCTL_RCGC0_R &= ~0x00000C00;  // 7) configure for 125K 
  // ADC1_SSPRI_R = 0x0123;          // 8) Sequencer 3? is highest priority
  // ADC1_ACTSS_R &= ~0x0002;        // 9) disable sample sequencer 1
  // ADC1_EMUX_R &= ~0x00F0;         // 10) seq3 is software trigger
  // ADC1_SSMUX1_R = (ADC1_SSMUX1_R&0xFFFFFF0F)+(7<<4); // 11) channel Ain1-3 (PE0-2)
  // ADC1_SSCTL1_R = 0x0666;         // 12) no TS0 D0, yes IE0 END0-2, IF U=ERROR ENABLE ALL 4
  // ADC1_ACTSS_R |= 0x0002;         // 13) enable sample sequencer 1

  // nvm, i put it in pwm
  //volatile uint16_t delay;
  SYSCTL_RCGCGPIO_R |= SYSCTL_RCGCGPIO_R4;   // 1) activate clock for Port E
	while ((SYSCTL_RCGCGPIO_R&SYSCTL_RCGCGPIO_R4)!=SYSCTL_RCGCGPIO_R4){}
	
  GPIO_PORTE_DIR_R &= ~0x07;      // 3) make PE0-2 input
  GPIO_PORTE_AFSEL_R |= 0x07;     // 4) enable alternate function on PE0-2
  GPIO_PORTE_DEN_R &= ~0x07;      // 5) disable digital I/O on PE0-2
	GPIO_PORTE_PCTL_R = GPIO_PORTE_PCTL_R&0xFFFFF000;
  GPIO_PORTE_AMSEL_R |= 0x07;     // 6) enable analog functionality on PE0-2
	SYSCTL_RCGCADC_R |= 0x00000002; // 1) activate ADC1
	while ((SYSCTL_RCGCADC_R&0x00000002)!=0x00000002){}
		
  ADC1_PC_R &= ~0xF;              // 8) clear max sample rate field
  ADC1_PC_R |= 0x1;               //    configure for 125K samples/sec
  ADC1_SSPRI_R = 0x3210;          // 9) Sequencer 3 is lowest priority
  ADC1_ACTSS_R &= ~0x0002;        // 10) disable sample sequencer 1
  ADC1_EMUX_R &= ~0x00F0;         // 11) seq1 is software trigger
  ADC1_SSMUX1_R = 0x0321; //(ADC1_SSMUX1_R&0xFFFFFF0F)+(3<<4)+(2<<4)+(1<<4);         // 12) set channels for SS1 MUX 1
  ADC1_SSCTL1_R = 0x0644;         // 13) no D0 END0 IE0 TS0 D1 END1 IE1 TS1 D2 TS2, yes END2 IE2
  ADC1_IM_R &= ~0x0002;           // 14) disable SS1 interrupts
  ADC1_ACTSS_R |= 0x0002;         // 15) enable sample sequencer 1
}

//------------ADC1_InSeq1------------
// Busy-wait Analog to digital conversion
// Input: none
// Output: 12-bit result of ADC conversion
void ADC1_InSeq1(unsigned long *ain3, unsigned long *ain2, unsigned long *ain1){  
  ADC1_PSSI_R = 0x0002;            // 1) initiate SS1 this is a seq value
	//LED = 0x04;
  while((ADC1_RIS_R&0x02)==0);   // 2) wait for conversion done WAIT 0x04 is seq2,,, 0x02 is seq 1
  *ain3 = ADC1_SSFIFO1_R&0xFFF;    // 3A) read first result
  *ain2 = ADC1_SSFIFO1_R&0xFFF;    // 3B) read second result
  *ain1 = ADC1_SSFIFO1_R&0xFFF;    // 3C) read third result
  ADC1_ISC_R = 0x0002;             // 4) acknowledge completion
}

/*
 * Software filter using median to prevent crazy data
*/
// This function implements a median filter:
// y(n) = median(x(n), x(n-1), x(n-2))
// ain3 = PE0, ain2 = PE1, ain1 = 2
void ReadADCMedianFilter(unsigned long *ain3, unsigned long *ain2, unsigned long *ain1){
  //                   x(n-2)        x(n-1)
  static unsigned long ain3oldest=0, ain3middle=0;
  static unsigned long ain2oldest=0, ain2middle=0;
  static unsigned long ain1oldest=0, ain1middle=0;
  // save some memory; these do not need to be 'static'
  //            x(n)
  unsigned long ain3newest;
  unsigned long ain2newest;
  unsigned long ain1newest;
  ADC1_InSeq1(&ain3newest, &ain2newest, &ain1newest); // sample AIN2(PE1), AIN1 (PE2), AIN3 (PE0)
  *ain3 = median(ain3newest, ain3middle, ain3oldest);
  *ain2 = median(ain2newest, ain2middle, ain2oldest);
  *ain1 = median(ain1newest, ain1middle, ain1oldest);
  ain3oldest = ain3middle;
	ain1oldest = ain1middle;
	ain2oldest = ain2middle;
  ain3middle = ain3newest;
	ain1middle = ain1newest;
	ain2middle = ain2newest;
}

// Median function: 
// A helper function for ReadADCMedianFilter()
uint16_t median(unsigned long u1, unsigned long u2, unsigned long u3){
  unsigned long result;
  if(u1>u2)
    if(u2>u3)   result=u2;     // u1>u2,u2>u3       focus this: u1>u2>u3
      else
        if(u1>u3) result=u3;   // u1>u2,u3>u2,u1>u3 focus this: u1>u3>u2
        else      result=u1;   // u1>u2,u3>u2,u3>u1 focus this: u3>u1>u2
  else
    if(u3>u2)   result=u2;     // u2>u1,u3>u2       focus this: u3>u2>u1
      else
        if(u1>u3) result=u1;   // u2>u1,u2>u3,u1>u3 focus this: u2>u1>u3
        else      result=u3;   // u2>u1,u2>u3,u3>u1 focus this: u2>u3>u1
  return(result);
}