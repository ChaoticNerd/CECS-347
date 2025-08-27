#define RED_LED       (*((volatile unsigned long *)0x40025008))
#define RED_LED_MASK    0x02  // bit pisition for onboard red LED

// Initialize SysTick with busy wait running at bus clock.
void SysTick_Init(void);

// Time delay using busy wait.
// The delay parameter is in units of the core clock. (units of 20 nsec for 50 MHz clock)
void SysTick_Wait(unsigned long delay);

// Time delay using busy wait.
// This assumes 50 MHz system clock.
void SysTick_Wait10ms(unsigned long delay);
