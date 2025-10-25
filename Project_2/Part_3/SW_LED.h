//////////////////////1. Pre-processor Directives Section////////////////////
#include "tm4c123gh6pm.h"
/////////////////////////////////////////////////////////////////////////////

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

//////////////////////2. Declarations Section////////////////////////////////
////////// Constants //////////
#define SWITCH       (*((volatile unsigned long *)0x40025040))
#define LED (*((volatile unsigned long *)0x40025038))  // use onboard three LEDs: PF321

#define SW1_MASK 	0x01
#define SW2_MASK 	0x10
#define Modify_PF4 0x10
#define PORTF_CLEAR_PCTL 0x000F0000
#define PORTF_PUR 0x10
#define PORTF_IN 0x11
#define PORTF_OUT 0x0E
#define PORTF_IRQ 0x40000000
////////// Local Global Variables //////////

////////// Function Prototypes //////////
void LED_Init(void);
void SW_Init(void);
/////////////////////////////////////////////////////////////////////////////

//////////////////////3. Subroutines Section/////////////////////////////////
