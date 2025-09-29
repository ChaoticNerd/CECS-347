/////////////////////////////////////////////////////////////////////////////
// Course Number: CECS 347
// Assignment: Example project for Hardware PWM controlled Car
// Description: 
/////////////////////////////////////////////////////////////////////////////
#define LED (*((volatile unsigned long *)0x40025038))  // use onboard three LEDs: PF321
#define WHEEL_DIR (*((volatile unsigned long *)0x40005330)) // PB7632 are the four direction pins for L298
#define SWITCH       (*((volatile unsigned long *)0x40025040))

////////// Constants //////////  
// Color    LED(s) PortF
// dark     ---    0
// red      R--    0x02
// blue     --B    0x04
// green    -G-    0x08
// yellow   RG-    0x0A
// white    RGB    0x0E
// pink     R-B    0x06
// Cran     -GB    0x0C

#define Dark    	0x00
#define Red     	0x02
#define Blue    	0x04
#define Green   	0x08
#define Yellow  	0x0A
#define Cran      0x0C
#define White   	0x0E
#define Purple  	0x06

// Constant definitions based on the following hardware interface:
// PB5432 are used for direction control on L298.
// Motor 1 is connected to the left wheel, Motor 2 is connected to the right wheel.
#define FORWARD 0x88 			// 10XX10	
#define BACKWARD 0xCC 		// 11XX11
#define LEFTPIVOT 0xC8 		// 11XX10
#define RIGHTPIVOT 0x8C 	// 10XX11
#define SW1_MASK		 	0x10          // onboard switch PF4

#define Modify_PF1_4 0x1F
#define PORTF_CLEAR_PCTL 0x000FFFFF
#define PORTF_PUR 0x10
#define PORTF_IN 0x11
#define PORTF_OUT 0x0E
#define PORTF_IRQ 0x40000000

//////////////////////1. Declarations Section////////////////////////////////
////////// Function Prototypes //////////
// Dependency: None
// Inputs: None
// Outputs: None
// Description: Initializes PB5432 for use with L298N motor driver direction
void Car_Dir_Init(void);
void LED_Init(void);
/////////////////////////////////////////////////////////////////////////////
