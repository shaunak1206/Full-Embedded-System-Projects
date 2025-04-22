// Sound.cpp
// Runs on MSPM0
// Sound assets in sounds/sounds.h
// your name
// your data 
#include <cstdint>
#include <stdint.h>
#include <ti/devices/msp/msp.h>
#include "Sound.h"
#include "cmsis_gcc.h"
#include "sounds/sounds.h"
#include "../inc/DAC5.h"
#include "../inc/Timer.h"

uint32_t Index = 0;
const uint8_t *SoundPt = 0;
uint32_t SoundCount;

// // Global variables for background music
// static uint8_t currentBackgroundSound = 0;
// static const uint8_t* backgroundSounds[] = {fastinvader1, fastinvader2, fastinvader3, fastinvader4};
// static const uint32_t backgroundSoundLengths[] = {982, 1042, 1054, 1098};

void SysTick_IntArm(uint32_t period, uint32_t priority){
  SysTick->CTRL = 0;         // disable SysTick during setup
  SysTick->LOAD = period-1;  // reload value
  SysTick->VAL = 0;          // any write to current clears it
  SCB->SHP[1] = SCB->SHP[1]&(~0xC0000000) | priority; 
  SysTick->CTRL = 0x07;    // enable SysTick with core clock and interrupts
}

// initialize a 11kHz SysTick, however no sound should be started
// initialize any global variables
// Initialize the 5 bit DAC
void Sound_Init(void){

  Index = 0;
  SoundPt = 0;
  SoundCount = 0;
  
  DAC5_Init();
  
  SysTick->CTRL = 0;         
  SysTick->LOAD = 7273-1;    
  SysTick->VAL = 0;         
  SCB->SHP[1] = SCB->SHP[1]&(~0xC0000000) | 0; 

}

extern "C" void SysTick_Handler(void);
void SysTick_Handler(void){ // called at 11 kHz
  // output one value to DAC if a sound is active

  DAC5_Out(SoundPt[Index]);
  if (Index == SoundCount-1) {
    SysTick->CTRL = 0;
    Index = 0;
  } else {
    Index++;
  }
}

//******* Sound_Start ************
// This function does not output to the DAC. 
// Rather, it sets a pointer and counter, and then enables the SysTick interrupt.
// It starts the sound, and the SysTick ISR does the output
// feel free to change the parameters
// Sound should play once and stop
// Input: pt is a pointer to an array of DAC outputs
//        count is the length of the array
// Output: none
// special cases: as you wish to implement
void Sound_Start(const uint8_t *pt, uint32_t count){
  __disable_irq();

  SoundPt = pt;        
  SoundCount = count;  
  Index = 0;      
  
  SysTick->CTRL = 0x07;

  __enable_irq();
}

void Sound_Shoot(void){
// write this
  Sound_Start(shoot, 4080);
}

void Sound_Killed(void) {
    Sound_Start(invaderkilled, 3377);
}

void Sound_Explosion(void) {
    Sound_Start(explosion, 2000);
}

void Sound_Fastinvader1(void) {
    Sound_Start(fastinvader1, 15140);
}

void Sound_Fastinvader2(void) {
    Sound_Start(fastinvader2, 15140);
}

void Sound_Fastinvader3(void) {
    Sound_Start(fastinvader3, 15140);
}

void Sound_Fastinvader4(void) {
    Sound_Start(fastinvader4, 15140);
}

void Sound_Highpitch(void) {
    Sound_Start(highpitch, 33973);
}

// void Sound_Background(void) {
//     Sound_Start(backgroundSounds[currentBackgroundSound], backgroundSoundLengths[currentBackgroundSound]);
//     currentBackgroundSound = (currentBackgroundSound + 1) % 4;
// }

