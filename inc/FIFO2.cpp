// FIFO2.cpp
// Runs on any microcontroller
// Provide functions that initialize a FIFO, put data in, get data out,
// and return the current size.  The file includes a transmit FIFO
// using index implementation and a receive FIFO using pointer
// implementation.  Other index or pointer implementation FIFOs can be
// created using the macros supplied at the end of the file.
// Created: 1/16/2020 
// Student names: change this to your names or look very silly
// Last modification date: change this to the last modification date or look very silly


/* This example accompanies the book
   "Embedded Systems: Real Time Interfacing to the Arm Cortex M3",
   ISBN: 978-1463590154, Jonathan Valvano, copyright (c) 2011
   Programs 3.7, 3.8., 3.9 and 3.10 in Section 3.7

 Copyright 2019 by Jonathan W. Valvano, valvano@mail.utexas.edu
    You may use, edit, run or distribute this file
    as long as the above copyright notice remains
 THIS SOFTWARE IS PROVIDED "AS IS".  NO WARRANTIES, WHETHER EXPRESS, IMPLIED
 OR STATUTORY, INCLUDING, BUT NOT LIMITED TO, IMPLIED WARRANTIES OF
 MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE APPLY TO THIS SOFTWARE.
 VALVANO SHALL NOT, IN ANY CIRCUMSTANCES, BE LIABLE FOR SPECIAL, INCIDENTAL,
 OR CONSEQUENTIAL DAMAGES, FOR ANY REASON WHATSOEVER.
 For more information about my classes, my research, and my books, see
 http://users.ece.utexas.edu/~valvano/
 */
#include <stdint.h>

#include "../inc/FIFO2.h"
#include "../inc/ST7735.h"

#define FIFO_SIZE 8
static char Fifo[FIFO_SIZE];
static uint32_t PutI = 0;
static uint32_t GetI = 0;

// A class named Queue that defines a FIFO
Queue::Queue(){
  // Constructor - set PutI and GetI as 0. 
  // We are assuming that for an empty Queue, both PutI and GetI will be equal
    // add code here to initialize on creation
  PutI = GetI = 0;
}

// To check whether Queue is empty or not
bool Queue::IsEmpty(void){
    return PutI == GetI;  // replace this with solution
}

  // To check whether Queue is full or not
bool Queue::IsFull(void){
    return ((PutI + 1) % FIFO_SIZE) == GetI;  // replace this with solution
}

  // Inserts an element in queue at rear end
bool Queue::Put(char x){
  if(IsFull()) return false;
  Fifo[PutI] = x;
  PutI = (PutI + 1) % FIFO_SIZE;
  return true;  // replace this with solution

}

  // Removes an element in Queue from front end. 
bool Queue::Get(char *pt){
  if(IsEmpty()) return false;
  *pt = Fifo[GetI];
  GetI = (GetI + 1) % FIFO_SIZE;
  return true;
}

  /* 
     Printing the elements in queue from front to rear. 
     This function is only to test the code. 
     This is not a standard function for Queue implementation. 
  */
void Queue::Print(void){
  uint32_t i = GetI;
  while(i != PutI){
    ST7735_OutChar(Fifo[i]);
    i = (i + 1) % FIFO_SIZE;
  }
  ST7735_OutChar('\n');
}

