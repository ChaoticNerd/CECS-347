#define ECHO_PIN 				(*((volatile unsigned long *)0x40005100))  // PB6 is the echo pin	
#define ECHO_VALUE 			0x40   // trigger at bit 6
#define MC_LEN 					0.0625 // length of one machine cycle in microsecond for 16MHz clock
#define SOUND_SPEED 		0.0343 // centimeter per micro-second

static volatile uint32_t distance = 0;

void Timer1A_Init(unsigned long period, unsigned long setTAMR);
long Timer1A_Stop(void);