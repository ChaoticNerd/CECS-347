[![Review Assignment Due Date](https://classroom.github.com/assets/deadline-readme-button-22041afd0340ce965d47ae6ef1cefeee28c7c493a6346c4f15d667ab976d596c.svg)](https://classroom.github.com/a/HfAiIg7k)
# CECS 347 Project 3 – Space Invaders

## 🧩 Starter Project

**Starter project:** `SpaceInvadersStarter.zip`  
Download and unzip the starter project, then rename it to `SpaceInvaders`.  
Set `ModuleTest.c` as your main program.

### Tasks
- Implement **PLL** and **ADC** modules and test them with `ModuleTest.c`.
- Implement a module test for the required ADC module.

---

## ⚙️ Software Modules

- Use **PLL** to generate an **80 MHz system clock**.  
- Use **ADC1 Sample Sequencer 3** to convert analog signals received from **analog channel 8** (potentiometer output).  
- Normalize the ADC digital value to represent the **x-axis position** of a screen with a specified width.

---

## 🔌 Hardware Modules

**Components:**  
- TM4C123 LaunchPad  
- Potentiometer  

**Connections:**  
- `VCC` → `3.3 V`  
- `Output` → `Analog Channel 8`  
- `GND` → `LaunchPad GND`

**Demonstration:**  
Put the system in onboard debug mode, set a breakpoint at the function call to `ADCValue_To_X_AXIS()`, and collect at least **three values** between `[0, 83]`.

**Deliverables**
1. Source code: `PLL.c/h`, `ADC1SS3.c/h`, `ModuleTest.c`
2. Schematic and a picture of the embedded system

---

## 📟 Nokia5110 LCD Module Test

**Reference document:** `CECS 347 Project 3 – Nokia5110 LCD Module Test.docx`

### Requirements
- Use **SSI2** to interface with **Nokia5110** LCD.  
- Implement the following functions in `Nokia5110.c`:  
  - `lcdwrite()`  
  - `Nokia5110_Init()`  
- Display the **ending prompt with score**.  
  - Use `sprintf()` to combine a string and an integer.

**Deliverables**
1. Source code: `Nokia5110.c/h`, `ModuleTest.c`
2. Schematic and a picture of the embedded system

---

## 🎮 Version 1 – SpaceInvadersV1

Start with `SpaceInvader.c`, rename it to `SpaceInvaderV1.c`, and use it as your main program.

### Game Features
**Start Screen**



**Gameplay**
- Press `SW1` to start.
- Display at least **three enemies** with different shapes.
- Enemies move left → right, opening/closing arms and legs at **1 pixel per 0.1 s**.
- When an enemy reaches the right edge, it disappears (dead).
- After all enemies die, show the **game-end message for 3 s**, then return to the start screen.
- Use **SysTick** with interrupts for:
  - **10 Hz** screen refresh
  - **3 s** game-end timing control

**Modules implemented:** `main`, `Nokia5110`, `PLL`, `switches`, `SysTick`

**Deliverables**
- **SpaceInvadersV1.zip** containing:  
  `SpaceInvadersV1.c`, `Nokia5110.c/h`, `PLL.c/h`, `SysTick.c/h`, `switches.c/h`

---

## 🛰 Version 2 – SpaceInvadersV2

### Feature Requirements
- Add **spaceship movement** controlled by the **potentiometer**.  
- The spaceship appears at the bottom of the screen; its starting position is determined by the potentiometer output at game start.

**Modules implemented:** `main`, `Nokia5110`, `PLL`, `switches`, `SysTick`, `ADC`

**Deliverables**
- **SpaceInvadersV2.zip** containing:  
  `SpaceInvadersV2.c`, `Nokia5110.c/h`, `PLL.c/h`, `SysTick.c/h`, `ADC.c/h`, `switches.c/h`

---

## 💥 Version 3 – SpaceInvadersV3

### Feature Requirements
- Add **SW1 shooting feature**:
  - When `SW1` is pressed, one bullet is fired from the top of the spaceship (if no bullet is alive).
  - When a bullet hits an enemy, display an explosion.
  - If the bullet misses, it disappears upon reaching the top of the screen.
  - Only **one bullet** can be active at a time.
  - Use **rising-edge interrupt** so that firing occurs after button release.

**Modules implemented:** `main`, `Nokia5110`, `PLL`, `switches`, `SysTick`, `ADC`

**Deliverables**
- **SpaceInvadersV3.zip** containing:  
  `SpaceInvadersV3.c`, `Nokia5110.c/h`, `PLL.c/h`, `SysTick.c/h`, `ADC.c/h`, `switches.c/h`

---

## 🔊 Version 4 – SpaceInvadersV4 (Final)

**Reference project:** `SpaceInvadersSounds.zip`

### Steps to Generate Game Sounds
1. Sample required sounds from a musical instrument.  
2. Convert samples to digital values.  
3. Store digital sound data in arrays.  
4. Output array values at the same frequency as the sampling rate.

(Steps 1 – 3 are provided in the example project.)

### Feature Requirements
- Add **sound effects** for shooting and explosions:
  - Use a **4-bit R-2R DAC** to generate sounds.  
  - Use **Timer 1** for precise timing.
- Build and test your DAC circuit using the `SpaceInvadersSounds` example, then integrate into `SpaceInvadersV3` and rename it to `SpaceInvadersV4`.

**Deliverables**
- **SpaceInvadersV4.zip** containing:  
  `SpaceInvadersV4.c`, `Nokia5110.c/h`, `PLL.c/h`, `SysTick.c/h`, `ADC.c/h`,  
  `DAC.c/h`, `Sound.s/h`, `Timer1.c/h`, `switches.c/h`, `ModuleTest.c`

