/////////////////////////////////////////////////////////////////////////////
// Course Number: CECS 347
// Assignment: Project 2, Part 1
// @author: Justin Narciso
// @author: Natasha Kho
// @date 10/1/2025
// CSULB Computer Engineering
// Description: Contains the initialization for the PWM hardware module on 
// 							the tm4c. Also has the function to control the duty
//							cycles.
/////////////////////////////////////////////////////////////////////////////

//////////////////////2. Declarations Section////////////////////////////////
									////////// Constants //////////
#define WHEEL_DIR (*((volatile unsigned long *)0x40005330)) // PB7632 are the four direction pins for L298

// Constant definitions based on the following hardware interface:
// System clock is 16MHz.
#define TOTAL_PERIOD 16000 //  16MHz/1000=16000
#define FIFTY_DUTY   16000 * 0.5
#define TWENTY_DUTY  16000 * 0.2
#define THIRTY_DUTY  16000 * 0.3
#define EIGHTY_DUTY  16000 * 0.8

// PB7632 are used for direction control on L298.
// Motor 1 is connected to the left wheel, Motor 2 is connected to the right wheel.
#define BACKWARD 0x88 			// 10XX10	
#define FORWARD 0xCC 		// 11XX11
#define RIGHTPIVOT 0xC8 		// 11XX10
#define LEFTPIVOT 0x8C 	// 10XX11

//Wheels 
#define BOTH_WHEEL 0x0000000C;
#define RIGHT_WHEEL 0x00000008;
#define LEFT_WHEEL 0x00000004;
//////////////////////1. Declarations Section////////////////////////////////
////////// Function Prototypes //////////
// Dependency: None
// Inputs: None
// Outputs: None
// Description: 
//	Initializes the PWM module 1 signals tied to PF321 on the Tiva Launchpad 
//		to allow for changing brightness of LEDs based on vehicle speed.
//	Initializes the PWM module 0 signals tied to PB76 to be used with the 
//		L298N motor driver allowing for a variable speed of robot car.
void PWM_PB76_Init(void);

void move_right_pivot(void);
void move_left_pivot(void);
void stop_the_car(void);
void move_backward(void);
void move_forward(void);
void move_left_turn(void);
void move_right_turn(void);

// Dependency: PWM_PB76_Init()
// Inputs: 
//	duty_L is the value corresponding to the duty cycle of the left wheel
//	duty_R is the value corresponding to the duty cycle of the right wheel
// Outputs: None 
// Description: Changes the duty cycles of PB76 by changing the CMP registers
void PWM_PB76_Duty(unsigned long duty_L, unsigned long duty_R);
/////////////////////////////////////////////////////////////////////////////
