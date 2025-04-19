// BusyWait.s
// Student names: change this to your names or look very silly
// Last modification date: change this to the last modification date or look very silly

// Note: these functions do not actually output to SPI or Port A. 
// They are called by the grader to see if the functions would work

// As part of Lab 6, students need to implement these two functions

      .global   SPIOutCommand
      .global   SPIOutData
      .text
      .align 2

// ***********SPIOutCommand*****************
// This is a helper function that sends an 8-bit command to the LCD.
// Inputs: R0 = 32-bit command (number)
//         R1 = 32-bit SPI1->STAT, SPI status register address
//         R2 = 32-bit SPI1->TXDATA, SPI tx data register address
//         R3 = 32-bit GPIOA->DOUTCLR31_0, PA13 is D/C
// Outputs: none
// Assumes: SPI and GPIO have already been initialized and enabled
// Note: must be AAPCS compliant
// Note: using the clear register to clear will make it friendly
SPIOutCommand:
// --UUU-- Code to write a command to the LCD
//1) Read the SPI status register (R1 has address of SPI1->STAT) and check bit 4,
//2) If bit 4 is high, loop back to step 1 (wait for BUSY bit to be low)
//3) Clear D/C (GPIO PA13) to zero, be friendly (R3 has address of GPIOA->DOUTCLR31_0)
//    Hint: simply write 0x2000 to GPIOA->DOUTCLR31_0
//4) Write the command to the SPI data register (R2 has address of SPI1->TXDATA)
//5) Read the SPI status register (R1 has address of SPI1->STAT) and check bit 4,
//6) If bit 4 is high, loop back to step 5 (wait for BUSY bit to be low)
    PUSH   {R4,R5, LR}               // Save return address

    WaitBusy1:
    LDR    R4, [R1]           // Load SPI1->STAT into R4
    LSRS   R4, R4, #4         // Shift right 4 to bring BUSY bit (bit 4) to bit 0
    MOVS R5, #1
    ANDS R4, R4, R5
    CMP R4, #1
    BEQ    WaitBusy1          // If BUSY bit is 1, still busy, loop until clear

    MOVS   R4, #1          // 0x2000 
    LSLS   R4, R4, #13         // Shift left to get 0x2000 (bit 13 for PA13)
    STR    R4, [R3]           // Clear PA13 (D/C=0), it's a command

    STR    R0, [R2]           // Write command to SPI1->TXDATA

    WaitBusy2:
    LDR    R4, [R1]           // Read SPI1->STAT again
    LSRS   R4, R4, #4         // Check BUSY bit
    CMP R4, #1
    BEQ    WaitBusy2          // Still busy? Keep waiting

    POP    {R4,R5, PC}               // Return



// ***********SPIOutData*****************
// This is a helper function that sends an 8-bit data to the LCD.
// Inputs: R0 = 32-bit data (number)
//         R1 = 32-bit SPI1->STAT, SPI status register address
//         R2 = 32-bit SPI1->TXDATA, SPI data register address
//         R3 = 32-bit GPIOA->DOUTSET31_0, PA13 is D/C
// Outputs: none
// Assumes: SPI and GPIO have already been initialized and enabled
// Note: must be AAPCS compliant
// Note: using the set register to clear will make it friendly
SPIOutData:
// --UUU-- Code to write data to the LCD
//1) Read the SPI status register (R1 has address of SPI1->STAT) and check bit 1,
//2) If bit 1 is low, loop back to step 1 (wait for TNF bit to be high)
//3) Set D/C (GPIO PA13) to one, be friendly (R3 has address of GPIOA->DOUTSET31_0)
//    Hint: simply write 0x2000 to GPIOA->DOUTSET31_0
//4) Write the data to the SPI data register (R2 has address of SPI1->TXDATA)

    PUSH   {R4-R7,LR}               // Save return address

    WaitTNF:
    LDR    R4, [R1]           // Load SPI1->STAT
    MOVS R5, #2
    ANDS R4, R4, R5
    CMP R4, #2
    BNE    WaitTNF            // If bit 1 == 0 (TNF low), loop

    MOVS   R4, #1          // 0x2000 -> 0x20 shifted later
    LSLS   R4, R4, #13         // Shift left to get 0x2000 (bit 13 for PA13)
    STR    R4, [R3]           // Set PA13 (D/C=1), it's data

    STR    R0, [R2]           // Write data to SPI1->TXDATA

    POP    {R4-R7, PC}               // Return
//****************************************************

    .end
