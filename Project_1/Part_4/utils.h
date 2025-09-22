#include <stdint.h>

extern void PortB_Init(void);
extern void GPIOPortB_Handler(void);

extern void PortF_Init(void);

extern void Timer1A_Init(void);
extern void Timer1A_Start(unsigned long period);
extern uint32_t Timer1A_Stop(void);

extern void EnableInterrupts(void);
extern void DisableInterrupts(void);
extern void WaitForInterrupt(void);  

extern void UART0_Init(void);
extern void UART0_OutString(uint8_t *pt);
extern void UART0_OutCRLF(void);

extern void SysTick_Init(uint32_t period);
extern void SysTick_Disable(void);
extern void SysTick_Handler(void);

