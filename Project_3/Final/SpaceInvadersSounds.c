// SpaceInvaderSound.c.c
// Runs on TM4C123
// Min He
// September 26, 2013
// Example code for generating Space Invaders game sounds: 
// This program demonstrate how to make all possible sounds for space invader game.


// DAC bit 0 on PB0 (least significant bit)
// DAC bit 1 on PB1
// DAC bit 2 on PB2
// DAC bit 3 on PB3 (most significant bit)

#include "tm4c123gh6pm.h"
#include "PLL.h"
#include <stdint.h>
#include "Sound.h"

extern void EnableInterrupts(void);
extern void DisableInterrupts(void);

void Delay100mst(unsigned long count);


int sound_test(void){
	uint8_t i;
  DisableInterrupts();
  PLL_Init();                   // set system clock to 80 MHz
  Sound_Init();
  EnableInterrupts();
    
  while(1){
		for (i=0;i<8;i++) {
			switch (i) {
				case 0: 
					Sound_Shoot();
				  break;
				case 1:
					Sound_Explosion();
					break;
				case 2:
					Sound_Killed();
					break;
				case 3:
					Sound_Fastinvader1();
					break;
				case 4: 
					Sound_Fastinvader2();
					break;
				case 5: 
					Sound_Fastinvader3();
					break;
				case 6: 
					Sound_Fastinvader4();
					break;
				case 7: 
					Sound_Highpitch();
					break;
				default:
					break;				
		  }
		  Delay100mst(10); // wait for 1 second
    }
  }
}

void Delay100mst(unsigned long count){
  unsigned long volatile time;
  while(count>0){
    time = 72724;  // 0.1sec at 16 MHz
    while(time){
	  	time--;
    }
    count--;
  }
}

