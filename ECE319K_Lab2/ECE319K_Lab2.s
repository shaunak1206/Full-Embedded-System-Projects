//****************** ECE319K_Lab2H.s ***************
// Your solution to Lab 2 in assembly code
// Author: Your name
// Last Modified: Your date
// ECE319H Spring 2025 (ECE319K students do Lab2)

    .include "../inc/msp.s"
    

        .data
        .align 2
// Declare global variables here if needed
// with the .space assembly directive

        .text
        .thumb
        .align 2
        .global EID
EID:    .string "MAP7848" // replace ZZZ123 with your EID here
        .align 2
  .equ dot,100
  .equ dash,(3*dot)
  .equ shortgap,(2*dot)  // because it will send an interelement too
  .equ interelement,dot
  Morse:
  .long  dot,  dash,    0,    0, 0 // A
  .long  dash,  dot,  dot,  dot, 0 // B
  .long  dash,  dot, dash,  dot, 0 // C
  .long  dash,  dot,  dot,    0, 0 // D
  .long  dot,     0,    0,    0, 0 // E
  .long  dot,   dot, dash,  dot, 0 // F
  .long  dash, dash,  dot,    0, 0 // G
  .long  dot,   dot,  dot,  dot, 0 // H
  .long  dot,   dot,    0,    0, 0 // I
  .long  dot,  dash, dash, dash, 0 // J
  .long  dash,  dot, dash,    0, 0 // K
  .long  dot,  dash,  dot,  dot, 0 // L
  .long  dash, dash,    0,    0, 0 // M
  .long  dash,  dot,    0,    0, 0 // N
  .long  dash, dash, dash,    0, 0 // O
  .long  dot,  dash, dash,  dot, 0 // P
  .long  dash, dash,  dot, dash, 0 // Q
  .long  dot,  dash,  dot,    0, 0 // R
  .long  dot,   dot,  dot,    0, 0 // S
  .long  dash,    0,    0,    0, 0 // T
  .long  dot,   dot, dash,    0, 0 // U
  .long  dot,   dot,  dot, dash, 0 // V
  .long  dot,  dash, dash,    0, 0 // W
  .long  dash,  dot,  dot, dash, 0 // X
  .long  dash,  dot, dash, dash, 0 // Y
  .long  dash, dash,  dot,  dot, 0 // Z

  .align 2
  .global Lab2Grader
  .global Lab2
  .global Debug_Init // Lab3 programs
  .global Dump       // Lab3 programs

// Switch input: PB2 PB1 or PB0, depending on EID
// LED output:   PB18 PB17 or PB16, depending on EID
// logic analyzer pins PB18 PB17 PB16 PB2 PB1 PB0
// analog scope pin PB20
Lab2:
LDR R0, =24000000
BL Delay
    


// Initially the main program will
//   set bus clock at 80 MHz,
//   reset and power enable both Port A and Port B
// Lab2Grader will
//   configure interrupts  on TIMERG0 for grader or TIMERG7 for TExaS
//   initialize ADC0 PB20 for scope,
//   initialize UART0 for grader or TExaS
    
    BL   Lab2Init   // you initialize input pin and output pin
    
loop:

    polling_loop:
    LDR  R0, =GPIOB_DIN31_0    
    LDR  R1, [R0]              
    MOVS R2, #1              
    ANDS R1, R1, R2
    CMP  R1, #0
    BEQ polling_loop

    // wait 1 sec
    MOVS R0,#10  // 0 for info, 1 debug with logic analyzer, 2 debug with scope, 10 for grade
    BL   Lab2Grader

    //this code finds the starting address in the array above of the input letter given puts it in R0
    LDR R1, =0x41
    SUBS R0, R0, R1
    MOVS R1, #20
    MULS R0, R0, R1
    LDR R1, =Morse
    ADDS R0, R1, R0

    SUBS R0, R0, #4

    //BL dotOrDash

    dotOrDash:
    ADDS R0, R0, #4
    

    LDR R1, [R0]
    LDR R2, =0x0000012C
    CMP R1, R2
    BEQ Dash
    LDR R2, =0x00000064
    CMP R1, R2
    BEQ Dot

    CMP R1, #0
    BEQ Lab2

    B dotOrDash

    
    Dot: 
    PUSH {R0}
    BL TurnOn
    LDR R0, =8000000
    BL Delay
    BL TurnOff
    LDR R0, =8000000
    BL Delay
    POP {R0}
    
    B dotOrDash
    

    Dash: 
    PUSH {R0}
    BL TurnOn
    LDR R0, =24000000
    BL Delay
    BL TurnOff
    LDR R0, =8000000
    BL Delay
    POP {R0}

    B dotOrDash

    //BX LR
       
    
    //turns on the LED
    TurnOn:
    LDR R0, =GPIOB_DOUT31_0
    MOVS R1, #1
    LSLS R1, #18
    STR R1, [R0]
    BX LR

    //turn off the LED
    TurnOff:
    LDR R0, =GPIOB_DOUT31_0
    LDR R1, [R0]
    MOVS R2, #1
    LSLS R2, #18
    BICS R1, R2
    STR R1, [R0]
    BX LR

    //delay is 12.5ns * count (R0)
    Delay: SUBS R0,R0,#2
    dloop: SUBS R0,R0,#4 // 4 cycles per loop
        NOP
        BHS  dloop
        BX LR




// make switch an input, LED an output
// PortB is already reset and powered
// Set IOMUX for your input and output
// Set GPIOB_DOE31_0 for your output (be friendly)
Lab2Init:
// ***do not reset/power Port A or Port B, already done****

    PUSH {R0}
   // PB0_Init: // PB0 input
    LDR  R0,=IOMUXPB0 // PINCM
    LDR  R1,=0x00040081
    //LDR R2, [R0]
    //ORRS R2, R2, R1
    STR R1, [R0]
    //friendly way to configure for input
    
    // PB18_Init: // PB18 output
    LDR R0, =IOMUXPB18
    MOVS R1, #0x81
    LDR R2, [R0]
    ORRS R2, R2, R1
    STR R2, [R0]
    //this is the first step of configuring the output pin by saying its not an input by making sure the 18th bit is not a 1

    //the next step is the DOE step; Output Enable
    LDR R0, =GPIOB_DOE31_0
    LDR R1, =0x40000
    LDR R2, [R0]
    ORRS R2, R2, R1
    STR R2, [R0]
    
    POP{R0}
    BX   LR

   .end
