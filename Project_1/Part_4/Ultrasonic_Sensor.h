#include <stdint.h>

uint32_t distance;

#define ECHO_PIN 					(*((volatile unsigned long *)0x40005040))  // PB4 is the echo pin	
#define ECHO_VALUE 				0x10   				// echo at bit 4
#define MC_LEN 						0.0625 				// length of one machine cycle in microsecond for 16MHz clock
#define SOUND_SPEED 			0.0343 				// centimeter per micro-second
#define SW1_MASK		 	0x10          // onboard switch PF4
#define PORTF_INT_PRI4			0x00800000								// Priority 4
#define PortB_INT 0x40000000
#define Modify_PF1_4 0x1F
#define PORTF_CLEAR_PCTL 0x000FFFFF
#define PORTF_PUR 0x10
#define PORTF_IN 0x11
#define PORTF_OUT 0x0E
#define PORTB_PCTL 0x00FF0000
#define PORTB_ENABLE_4_5 0x30
#define PORTB_5_MASK 0x20
#define PORTB_4_MASK 0x10