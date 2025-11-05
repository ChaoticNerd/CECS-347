// ADC1SS3.c
// Runs on TM4C123, starter file for space invader game
// Min He
// November 15, 2022

#include "tm4c123gh6pm.h"
#include <stdint.h>

// TODO
#define ADC1_PSSI_SS3 (0x0008)    // start sample sequencer 3
#define ADC1_ISC_SS3  (0x0008)    // acknowledge sample sequencer 3 interrupt
#define ADC1_RIS_SS3  (0x8)

// Function Prototype
void ADC1SS3_Init(void);

// This initialization function sets up the ADC1 Sample Sequencer 3 
// Max sample rate: <=125,000 samples/second
// SS3 triggering event: software trigger
// SS3 1st sample source: Ain8 (PE5)
// SS3 interrupts: flag set on completion but no interrupt requested
void ADC1SS3_Init(void){ 
	SYSCTL_RCGCGPIO_R |= SYSCTL_RCGCGPIO_R4;   // 1) activate clock for Port E
	while ((SYSCTL_RCGCGPIO_R&SYSCTL_RCGCGPIO_R4)!=SYSCTL_RCGCGPIO_R4){}
	
  GPIO_PORTE_DIR_R &= ~0x20;      // 3) make PE0-2 input
  GPIO_PORTE_AFSEL_R |= 0x20;     // 4) enable alternate function on PE0-2
  GPIO_PORTE_DEN_R &= ~0x20;      // 5) disable digital I/O on PE0-2
	GPIO_PORTE_PCTL_R = GPIO_PORTE_PCTL_R&0x00F00000;
  GPIO_PORTE_AMSEL_R |= 0x20;     // 6) enable analog functionality on PE0-2
		
	SYSCTL_RCGCADC_R |= 0x00000002; // 1) activate ADC1
	while ((SYSCTL_RCGCADC_R&0x00000002)!=0x00000002){}
		
  ADC1_PC_R &= ~0xF;              // 8) clear max sample rate field (hidden leading 0s)
  ADC1_PC_R |= 0x1;               //    configure for 125K samples/sec (hidden leading 0s)
		
  ADC1_SSPRI_R = 0x0123;          // 9) Sequencer 3 is highest priority
  ADC1_ACTSS_R &= ~ADC1_PSSI_SS3;        // 10) disable sample sequencer 3
  ADC1_EMUX_R &= ~0xF000;         // 11) seq3 is software trigger
  ADC1_SSMUX3_R = 0x8; 						// 12) set channels for SS3 MUX 0		
  ADC1_SSCTL3_R = 0x6000;         // 13) no D0 END0 IE0 TS0 D1 END1 IE1 TS1 D2 TS2, yes END2 IE2
  ADC1_IM_R &= ~0x0008;           // 14) disable SS3 interrupts
  ADC1_ACTSS_R |= ADC1_PSSI_SS3;         // 15) enable sample sequencer 3
}

//------------ADC1SS3_In------------
// Busy-wait Analog to digital conversion
// Input: none
// Output: 12-bit result of ADC conversion
uint16_t ADC1SS3_In(void){
	uint16_t ADC_Value = 0;
	
	ADC1_PSSI_R = ADC1_ISC_SS3;            // 1) initiate SS1 this is a seq value
  while((ADC1_RIS_R&ADC1_RIS_SS3)==0);   // 2) wait for conversion done
	
  ADC_Value = ADC1_SSFIFO1_R&0xFFF;    // 3A) read first result
  ADC1_ISC_R = ADC1_ISC_SS3;             // 4) acknowledge completion
	return 	ADC_Value;
}

// Convert a ADC value to a value between [0, max_x_axis]
// convert a value range [0,4095] to a range of [0,83]
// max_x_axis is forced to be 83
uint8_t ADCValue_To_X_AXIS(uint16_t ADCValue, uint8_t max_x_axis) {
	uint8_t x_position = 0;
	// some math bullshit
	//x_position = (ADCValue * 3.3) / 4095; // this converts ADC value to voltage
	x_position = (ADCValue * max_x_axis) / 4095;
	return x_position;
}
