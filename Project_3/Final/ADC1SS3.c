// ADC1SS3.c
// Runs on TM4C123, starter file for space invader game
// Group number: 15
// Group members: Natasha Kho, Justin Narciso

#include "tm4c123gh6pm.h"
#include <stdint.h>
#include "ADC1SS3.h"

// This initialization function sets up the ADC1 Sample Sequencer 3 
// Max sample rate: <=125,000 samples/second
// SS3 triggering event: software trigger
// SS3 1st sample source: Ain8 (PE5)
// SS3 interrupts: flag set on completion but no interrupt requested
void ADC1SS3_Init(void){ 
	SYSCTL_RCGCGPIO_R |= SYSCTL_RCGCGPIO_R4;   // 1) activate clock for Port E
	SYSCTL_RCGCADC_R |= SYSCTL_RCGCADC_R1; // 1a) activate ADC1
	while ((SYSCTL_RCGCADC_R&SYSCTL_RCGCADC_R1)!=SYSCTL_RCGCADC_R1){}
	while ((SYSCTL_RCGCGPIO_R&SYSCTL_RCGCGPIO_R4)!=SYSCTL_RCGCGPIO_R4){}
	
  GPIO_PORTE_DIR_R &= ~PORTE_5_MASK;      // 3) make PE5 input
  GPIO_PORTE_AFSEL_R |= PORTE_5_MASK;     // 4) enable alternate function on PE5
  GPIO_PORTE_DEN_R &= ~PORTE_5_MASK;      // 5) disable digital I/O on PE5
	GPIO_PORTE_PCTL_R = GPIO_PORTE_PCTL_R&(~PORTE_5_BITS);
  GPIO_PORTE_AMSEL_R |= PORTE_5_MASK;     // 6) enable analog functionality on PE5
		
		
  ADC1_PC_R &= ~ADC1_PC_CLEAR;              // 8) clear max sample rate field (hidden leading 0s)
  ADC1_PC_R |= ADC1_PC_125k;               //    configure for 125K samples/sec (hidden leading 0s)
		
  ADC1_SSPRI_R = ADC1_SSPRI_3HIGH;          // 9) Sequencer 3 is highest priority
  ADC1_ACTSS_R &= ~ADC1_PSSI_SS3;        // 10) disable sample sequencer 3
  ADC1_EMUX_R &= ~ADC1_EMUX_SS3_TRIGGER;         // 11) seq3 is software trigger
  ADC1_SSMUX3_R = ADC1_SSMUX3_CHANNEL; 						// 12) set channels for SS3 MUX 0		
	ADC1_SSCTL3_R = ADC1_SSCTL3_FIRST;         // 13) no D0 END0 IE0 TS0 D1 END1 IE1 TS1 D2 TS2, yes END2 IE2
  ADC1_IM_R &= ~ADC1_PSSI_SS3;           // 14) disable SS3 interrupts
  ADC1_ACTSS_R |= ADC1_PSSI_SS3;         // 15) enable sample sequencer 3
}

//------------ADC1SS3_In------------
// Busy-wait Analog to digital conversion
// Input: none
// Output: 12-bit result of ADC conversion
uint16_t ADC1SS3_In(void){
	uint16_t ADC_Value = 0;
	
	ADC1_PSSI_R = ADC1_PSSI_SS3;            // 1) initiate SS3 this is a seq value
	
  while((ADC1_RIS_R & ADC1_RIS_SS3)==0);   // 2) wait for conversion done
	
  ADC_Value = ADC1_SSFIFO3_R&ADC_READ_ALL;    // 3A) read first result
  ADC1_ISC_R = ADC1_ISC_SS3;             // 4) acknowledge completion
	return 	ADC_Value;
}


// Convert a ADC value to a value between [0, max_x_axis]
// convert a value range [0,4095] to a range of [0,83]
// max_x_axis is forced to be 83
uint8_t ADCValue_To_X_AXIS(uint16_t ADCValue, uint8_t max_x_axis) {
	uint8_t x_position = 0;
	
	x_position = ((ADCValue * max_x_axis) / MAX_VOLT);
	
	return x_position;
}
