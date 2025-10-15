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
  SYSCTL_RCGCGPIO_R |= SYSCTL_RCGCGPIO_R4;   // 1) activate clock for Port E
	while ((SYSCTL_RCGCGPIO_R&SYSCTL_RCGCGPIO_R4)!=SYSCTL_RCGCGPIO_R4){}

	GPIO_PORTE_DIR_R &= ~0x07;      // 2) make PE2 input
  GPIO_PORTE_AFSEL_R |= 0x07;     // 3) enable alternate function on PE0-2
  GPIO_PORTE_DEN_R &= ~0x07;      // 4) disable digital I/O on PE0-2
  GPIO_PORTE_AMSEL_R |= 0x07;     // 5) enable analog function on PE0-2
  SYSCTL_RCGC0_R |= 0x001000000;   // 6) activate ADC0 
	while ((SYSCTL_RCGC0_R&0x00100000)!=0x00100000){}
         
  SYSCTL_RCGC0_R &= ~0x00000C00;  // 7) configure for 125K 
  ADC1_SSPRI_R = 0x0123;          // 8) Sequencer 3? is highest priority
  ADC1_ACTSS_R &= ~0x0002;        // 9) disable sample sequencer 1
  ADC1_EMUX_R &= ~0xF000;         // 10) seq3 is software trigger
  ADC1_SSMUX1_R = (ADC1_SSMUX1_R&0xFFFFFFF0)+7; // 11) channel Ain1-3 (PE0-2)
  ADC1_SSCTL1_R = 0x0666;         // 12) no TS0 D0, yes IE0 END0-2, IF U=ERROR ENABLE ALL 4
  ADC1_ACTSS_R |= 0x0002;         // 13) enable sample sequencer 1
}

//------------ADC0_InSeq3------------
// Busy-wait Analog to digital conversion
// Input: none
// Output: 12-bit result of ADC conversion
uint16_t ADC0_InSeq3(void){  
	uint16_t result;
  ADC1_PSSI_R = ADC1_PSSI_SS1;            // 1) initiate SS3:1000
  while((ADC0_RIS_R&ADC1_RIS_SS1)==0){};   // 2) wait for conversion done
  result = ADC1_SSFIFO1_R&0xFFF;   // 3) read result
  ADC1_ISC_R = ADC1_ISC_SS1;             // 4) acknowledge completion
  return result;
}

// This function samples one value, apply a software filter to the value and
// returns filter result.  Some kind of filtering is required because 
// the IR distance sensors output occasional erroneous spikes.  
// This function implements a median filter:
// y(n) = median(x(n), x(n-1), x(n-2))
// Assumes: ADC has already been initialized. 
uint16_t ReadADCMedianFilter(void){
  static uint16_t oldest=0, middle=0;	
  uint16_t newest;
	uint16_t NewValue;
	
  newest = ADC0_InSeq3();  // read one value
  NewValue = median(newest, middle, oldest);
  oldest = middle; 
  middle = newest; 
	return NewValue;
}

// Median function: 
// A helper function for ReadADCMedianFilter()
uint16_t median(uint16_t u1, uint16_t u2, uint16_t u3){
uint16_t result;
	
  if(u1>u2)
    if(u2>u3)   result=u2;     // u1>u2,u2>u3       u1>u2>u3
      else
        if(u1>u3) result=u3;   // u1>u2,u3>u2,u1>u3 u1>u3>u2
        else      result=u1;   // u1>u2,u3>u2,u3>u1 u3>u1>u2
  else
    if(u3>u2)   result=u2;     // u2>u1,u3>u2       u3>u2>u1
      else
        if(u1>u3) result=u1;   // u2>u1,u2>u3,u1>u3 u2>u1>u3
        else      result=u3;   // u2>u1,u2>u3,u3>u1 u2>u3>u1
  return(result);
}