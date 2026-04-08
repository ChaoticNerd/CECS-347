// switches.h
// Runs on TM4C123, starter file for space invader game
// Group number: 15
// Group members: Natasha Kho, Justin Narciso

// Constants
#define SWITCH       (*((volatile unsigned long *)0x40025040))

#define SW1_MASK 	0x10
#define SW2_MASK 	0x01
#define PORTF_CLEAR_PCTL 0x000F000F
#define PORTF_PUR 0x11
#define PORTF_IN 0x11
#define PORTF_OUT 0x0E // out = 1; in = 0
#define PORTF_IRQ 0x40000000
#define PORTF_UNLOCK_PF0 0x4C4F434B

// Initialize the onboard two switches.
void Switch_Init(void);
