#define TRIGGER_PIN 		(*((volatile unsigned long *)0x40005200))  // PB7 is the trigger pin	
#define TRIGGER_VALUE 	0x80   // trigger at bit 7
#define ECHO_PIN 				(*((volatile unsigned long *)0x40005100))  // PB6 is the echo pin	
#define ECHO_VALUE 			0x40   // trigger at bit 6
#define MC_LEN 					0.0625 // length of one machine cycle in microsecond for 16MHz clock
#define SOUND_SPEED 		0.0343 // centimeter per micro-second