// ADC0SS3.h
// Runs on TM4C123
// Provide functions that initialize ADC0 SS3 to be triggered by
// software and trigger a conversion, wait for it to finish,
// and return the result.
// Daniel Valvano
// October 20, 2013
// Modified by Min He, 10/09/2022

// This file provide initialization function for two analog channels:
// PE2/AIN1 and PD0/AIN7

#include <stdint.h>

#define ADC1_PSSI_SS1 0x0002    // start sample sequencer 3
#define ADC1_ISC_SS1  0x0002    // acknowledge sample sequencer 3 interrupt
#define ADC1_RIS_SS1  0x02

//347 Room values
//#define TOO_FAR 		(1463)  // replace the zero with the ADC output value for maximum distance
//#define FOLLOW_DIST (1979)  // replace the zero with the ADC output value for object following distance
//#define TOO_CLOSE 	(2663)  // replace the zero with the ADC output value for minimum distance
//Library values
#define MAX_VAL			(0) 
#define TOO_FAR 		(1463)  // replace the zero with the ADC output value for maximum distance
#define FOLLOW_FAR (1815)		//1473 INCASE
//#define FOLLOW_DIST (1979)  // replace the zero with the ADC output value for object following distance
#define FOLLOW_CLOSE (2143)
#define TOO_CLOSE 	(2663)  // replace the zero with the ADC output value for minimum distance
#define MIN_VAL 		(4096) 

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
void ADC1_SS1_Init(void); 

//------------ADC0_InSeq3------------
// Busy-wait Analog to digital conversion
// Input: none
// Output: 12-bit result of ADC conversion
uint16_t ADC0_InSeq3(void);

void ADC1_InSeq1(unsigned long *ain3, unsigned long *ain2, unsigned long *ain1);

void ReadADCMedianFilter(unsigned long *ain3, unsigned long *ain2, unsigned long *ain1);

uint16_t median(unsigned long u1, unsigned long u2, unsigned long u3);