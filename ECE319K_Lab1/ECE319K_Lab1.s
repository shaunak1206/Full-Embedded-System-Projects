//****************** ECE319K_Lab1.s ***************
// Your solution to Lab 1 in assembly code
// Author: Mansa Pathak
// Last Modified: Your date
// Spring 2025
        .data
        .align 2
// Declare global variables here if needed
// with the .space assembly directive

        .text
        .thumb
        .align 2
        .global EID
EID:    .string "MAP7848" // replace ZZZ123 with your EID here

        .global Phase
        .align 2
Phase:  .long 10 
// Phase= 0 will display your objective and some of the test cases, 
// Phase= 1 to 5 will run one test case (the ones you have been given)
// Phase= 6 to 7 will run one test case (the inputs you have not been given)
// Phase=10 will run the grader (all cases 1 to 7)
        .global Lab1
// Input: R0 points to the list
// Return: R0 as specified in Lab 1 assignment and terminal window
// According to AAPCS, you must save/restore R4-R7
// If your function calls another function, you must save/restore LR
Lab1: PUSH {R4-R7,LR}
       // your solution goes here
       
       //LDR R0, =myClass 
       LDR R1, =EID //to store the start of my EID
       MOVS R2, #0 //this is the pointer that will run through the array 
       MOVS R3, #0 //this is the pointer that will run through my EID
        
       LOOP: 
       //this is to check if we have reached the null pointer of the array 
       LDR R5, [R0, R2]
       CMP R5, #0
       BEQ notThere 

       EID_LOOP:
       LDRB R6, [R1, R3] //this is getting the first value of my EID
       LDRB R7, [R5, R3] //this is getting the first value of the name in the list
       CMP R6, R7
       BNE nextValue
       CMP R6, #0
       BNE incrementCharPointer //keep incrementing the pointer until we reach the null pointer
       BEQ next
       next:
       CMP R7, #0
       BNE incrementCharPointer
       BEQ foundEID //if both R6 and R7 are pointing to the null pointer elements of their respective strings
       
      incrementCharPointer:
      ADDS R3, #1 //increment by one because each character is only 1 byte
      B EID_LOOP

      nextValue:
      MOVS R3, #0 //resetting the pointer that goes through the EID
      ADDS R2, #8 //incrementing the pointer towards the next address of the array which is 8 bytes away
      B LOOP

      foundEID:
      ADDS R2, #4 //we now want to get the score of the EID which is 4 bytes after the current EID address  
      LDR R0, [R0, R2]
      B exit

      //case for where the EID is not in the list; return -1 in R0  
      notThere:
      MOVS R0, #0
      MVNS R0, R0
      B exit


      exit:  
      POP  {R4-R7,PC} // return


        .align 2
        .global myClass
myClass: .long pAB123  // pointer to EID
         .long 95      // Score
         .long pXYZ1   // pointer to EID
         .long 96      // Score
         .long pAB5549 // pointer to EID
         .long 94      // Score
         .long 0       // null pointer means end of list
         .long 0
pAB123:  .string "AB123"
pXYZ1:   .string "MAP7448"
pAB5549: .string "AB5549"
        .end
