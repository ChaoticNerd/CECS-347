// ADC1SS3.h
// Runs on TM4C123, starter file for space invader game
// Group number: 15
// Group members: Natasha Kho, Justin Narciso

#include <stdint.h>

#define ADC1_PSSI_SS3 (0x0008)    // start sample sequencer 3
#define ADC1_ISC_SS3  (0x0008)    // acknowledge sample sequencer 3 interrupt
#define ADC1_RIS_SS3  (0x08)

#define PORTE_5_MASK 0x20
#define PORTE_5_BITS 0x00F00000

#define ADC1_PC_CLEAR 0x0F
#define ADC1_PC_125k 0x01
#define ADC1_SSPRI_3HIGH 0x0123
#define ADC1_EMUX_SS3_TRIGGER 0xF000
#define ADC1_SSMUX3_CHANNEL 0x08
#define ADC1_SSCTL3_FIRST 0x0006
#define ADC_READ_ALL 0xFFF

#define MAX_VOLT 4095


// This initialization function sets up the ADC1 Samplw Sequencer 3 
// Max sample rate: <=125,000 samples/second
// SS3 triggering event: software trigger
// SS3 1st sample source: Ain9 (PE4)
// SS3 interrupts: flag set on completion but no interrupt requested
void ADC1SS3_Init(void); 

//------------ADC1SS3_In------------
// Busy-wait Analog to digital conversion
// Input: none
// Output: 12-bit result of ADC conversion
uint16_t ADC1SS3_In(void); 

uint8_t ADCValue_To_X_AXIS(uint16_t ADCValue, uint8_t max_x_axis);
