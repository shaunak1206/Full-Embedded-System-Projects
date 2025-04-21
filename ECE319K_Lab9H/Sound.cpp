// Sound.cpp
// Runs on MSPM0
// Sound assets in sounds/sounds.h
// Manas Pathak
// March 2024
#include <stdint.h>
#include <cstddef>  // For NULL definition
#include <ti/devices/msp/msp.h>  // For SysTick definition
#include "Sound.h"
#include "sounds/sounds.h"
#include "../inc/DAC5.h"
#include "../inc/Timer.h"

// Global variables for sound playback
static const uint8_t* currentSound = NULL;
static uint32_t soundLength = 0;
static uint32_t soundIndex = 0;
static bool soundPlaying = false;

// Initialize the 5-bit DAC and sound system
void Sound_Init(void) {
    DAC5_Init();  // Initialize 5-bit DAC
    
    // Initialize SysTick for 11kHz sound output
    SysTick->CTRL = 0;                   // Disable SysTick during setup
    SysTick->LOAD = 7272;                // 80MHz/11kHz = 7272 cycles
    SysTick->VAL = 0;                    // Any write to current clears it
    SCB->SHP[1] = (SCB->SHP[1]&~0xC0000000)|(0x40000000); // Priority 2
    SysTick->CTRL = 0x07;                // Enable with core clock and interrupts
    
    soundPlaying = false;
    currentSound = NULL;
    soundLength = 0;
    soundIndex = 0;
}

// SysTick interrupt handler for sound playback
extern "C" void SysTick_Handler(void) {
    if (soundPlaying && currentSound != NULL) {
        // Output current sample to DAC
        DAC5_Out(currentSound[soundIndex]);
        soundIndex++;
        
        // Check if sound is complete
        if (soundIndex >= soundLength) {
            soundPlaying = false;
            currentSound = NULL;
        }
    }
}

// Start playing a sound
void Sound_Start(const uint8_t *pt, uint32_t count) {
    currentSound = pt;
    soundLength = count;
    soundIndex = 0;
    soundPlaying = true;
}

// Sound effects for different game events
void Sound_Shoot(void) {
    Sound_Start(shoot, 4080);
}

void Sound_Killed(void) {
    Sound_Start(explosion, 8731);
}

void Sound_Explosion(void) {
    Sound_Start(explosion, 8731);
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

