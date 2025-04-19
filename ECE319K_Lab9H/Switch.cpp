/*
 * Switch.cpp
 *
 *  Created on: Nov 5, 2023
 *      Author:
 */
#include <ti/devices/msp/msp.h>
#include "Switch.h"
#include "../inc/LaunchPad.h"  // needed for PB12INDEX and PB16INDEX

// Initialize switches on PB12 and PB16
void Switch_Init(void) {
    // Configure PB12 and PB16 as inputs with no pull
    IOMUX->SECCFG.PINCM[PB12INDEX] = 0x00040081;  // input, no pull
    IOMUX->SECCFG.PINCM[PB16INDEX] = 0x00040081;  // input, no pull
}

// Return current state of switches
// Bit 0: PB12 (Switch 1)
// Bit 1: PB16 (Switch 2)
uint32_t Switch_In(void) {
    uint32_t data = GPIOB->DIN31_0;
    // Extract PB12 (bit 12) for Switch 1 and PB16 (bit 16) for Switch 2
    return ((data >> 12) & 0x01) | (((data >> 16) & 0x01) << 1);
}
