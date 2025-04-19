// StringConversion.s
// Student names: Manas Pathak & Shaunak Kapur
// Last modification date: change this to the last modification date or look very silly
// Runs on any Cortex M0
// ECE319K lab 6 number to string conversion
//
// You write udivby10 and Dec2String
     .data
     .align 2
// no globals allowed for Lab 6
    .global OutChar    // virtual output device
    .global OutDec     // your Lab 6 function
    .global Test_udivby10

    .text
    .align 2
// **test of udivby10**
// since udivby10 is not AAPCS compliant, we must test it in assembly
Test_udivby10:
    PUSH {LR}

    MOVS R0,#123
    BL   udivby10
// put a breakpoint here
// R0 should equal 12 (0x0C)
// R1 should equal 3

    LDR R0,=12345
    BL   udivby10
// put a breakpoint here
// R0 should equal 1234 (0x4D2)
// R1 should equal 5

    MOVS R0,#0
    BL   udivby10
// put a breakpoint here
// R0 should equal 0
// R1 should equal 0
    POP {PC}

//****************************************************
// divisor=10
// Inputs: R0 is 16-bit dividend
// quotient*10 + remainder = dividend
// Output: R0 is 16-bit quotient=dividend/10
//         R1 is 16-bit remainder=dividend%10 (modulus)
// not AAPCS compliant because it returns two values
udivby10:
   PUSH {R3, R4, LR}

   MOVS R2, #0 //final answer storage
   MOVS R3, #0 //final remainder storage 
   MOVS R4, #32 //bit counter

   LOOP:
   LSLS R3, R3, #1 //shift the remainder left by 1

   LSLS R0, R0, #1 //put the MSB in the carry bit
   BCC NoCarry
   ADDS R3, R3, #1

   NoCarry:
   CMP R3, #10
   BGE Subtract10
   LSLS R2, R2, #1
   B ContinueLoop

   Subtract10:
   SUBS R3, R3, #10
   LSLS   R2, R2, #1     // Shift quotient left
   ADDS   R2, R2, #1     // Set LSB to 1 since subtraction happened

   ContinueLoop:
   SUBS   R4, R4, #1        // Decrement bit counter
   CMP R4, #0
   BNE LOOP         // If bits left, repeat loop

   MOV R0, R2         // Quotient in R0
   MOV R1, R3         // Remainder in R1

   POP    {R3, R4, PC}       // Restore R4 and return


  
//-----------------------OutDec-----------------------
// Convert a 16-bit number into unsigned decimal format
// Call the function OutChar to output each character
// You will call OutChar 1 to 5 times
// OutChar does not do actual output, OutChar does virtual output used by the grader
// Input: R0 (call by value) 16-bit unsigned number
// Output: none
// Invariables: This function must not permanently modify registers R4 to R11
.global OutDec
.text
.align 2
.equ remainder, 0      // Binding: remainder at offset 0 from SP
//****************************************************
// OutDec
// Inputs: R0 = 16-bit unsigned integer to output
// Outputs: Calls OutChar to output ASCII digits
// AAPCS compliant: preserves R4-R11, uses local variable
//****************************************************

OutDec:
    PUSH  {R4, LR}          // Save R4 and return address
    SUB   SP, SP, #4        // Allocation: make space for remainder

    MOVS  R1, #10           // Load constant 10
    CMP   R0, R1            // Compare input with 10
    BLT   BaseCase          // If less than 10, go to BaseCase

    BL    udivby10          // Divide: R0 = quotient, R1 = remainder

    STR   R1, [SP, #remainder]  // Access: store remainder
    BL    OutDec            // Recursive call with quotient (R0)
    B     OutputDigit       // After recursion, output digit

BaseCase:
    STR   R0, [SP, #remainder]  // Access: store single digit

OutputDigit:
    LDR   R3, [SP, #remainder]  // Access: load remainder/digit
    ADDS  R3, R3, #'0'          // Convert to ASCII
    MOV   R0, R3                // Move to R0 for OutChar
    BL    OutChar               // Output character

    ADD   SP, SP, #4        // Deallocation: release local variable
    POP   {R4, PC}          // Restore R4 and return
//* * * * * * * * End of OutDec * * * * * * * *

// ECE319H recursive version
// Call the function OutChar to output each character
// You will call OutChar 1 to 5 times
// Input: R0 (call by value) 16-bit unsigned number
// Output: none
// Invariables: This function must not permanently modify registers R4 to R11

OutDec2:
   PUSH {LR}
// write this

   POP  {PC}



     .end
