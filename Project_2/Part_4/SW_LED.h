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

#define SW1_MASK 	0x10
#define SW2_MASK 	0x01
#define PORTF_CLEAR_PCTL 0x000F000F
#define PORTF_PUR 0x11
#define PORTF_IN 0x11
#define PORTF_OUT 0x0E // out = 1; in = 0
#define PORTF_LED_SW 0x1F
#define PORTF_IRQ 0x40000000
#define PORTF_LED_MASK 0x0E
#define PORTF__LED_BITS_MASK 0x0000FFF0
#define PORTF_UNLOCK_PF0 0x4C4F434B
////////// Local Global Variables //////////

////////// Function Prototypes //////////
void LED_Init(void);
void SW_Init(void);
/////////////////////////////////////////////////////////////////////////////

//////////////////////3. Subroutines Section/////////////////////////////////
