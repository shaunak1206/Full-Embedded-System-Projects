// Dump.c
// Your solution to ECE319K Lab 3 Spring 2025
// Author: Your name
// Last Modified: Your date


#include <ti/devices/msp/msp.h>
#include "../inc/Timer.h"
#define MAXBUF 50
uint32_t DataBuffer[MAXBUF];
uint32_t TimeBuffer[MAXBUF];
uint32_t DebugCnt; // 0 to MAXBUF (0 is empty, MAXBUF is full)

// *****Debug_Init******
// Initializes your index or pointer.
// Input: none
// Output:none
void Debug_Init(void){
// students write this for Lab 3
// This function should also initialize Timer G12, call TimerG12_Init.
    DebugCnt = 0;  // Reset index
    TimerG12_Init();  // Initialize the timer
}

// *****Debug_Dump******
// Records one data and one time into the two arrays.
// Input: data is value to store in DataBuffer
// Output: 1 for success, 0 for failure (buffers full)
uint32_t Debug_Dump(uint32_t data){
// students write this for Lab 3
// The software simply reads TIMG12->COUNTERREGS.CTR to get the current time in bus cycles.
  if(DebugCnt>=MAXBUF){
    return 0;
  }
  TimeBuffer[DebugCnt] = TIMG12->COUNTERREGS.CTR;
  DataBuffer[DebugCnt] = data;
  DebugCnt++;

  return 1; // success
}
// *****Debug_Dump2******
// Always record data and time on the first call to Debug_Dump2
// However, after the first call
//    Records one data and one time into the two arrays, only if the data is different from the previous call.
//    Do not record data or time if the data is the same as the data from the previous call
// Input: data is value to store in DataBuffer
// Output: 1 for success (saved or skipped), 0 for failure (buffers full)
uint32_t Debug_Dump2(uint32_t data){
// students write this for Lab 3
// The software simply reads TIMG12->COUNTERREGS.CTR to get the current time in bus cycles.

  if(DebugCnt>=MAXBUF){
    return 0;
  }

  if(DataBuffer[DebugCnt-1] != data || DebugCnt==0){
    TimeBuffer[DebugCnt] = TIMG12->COUNTERREGS.CTR;
    DataBuffer[DebugCnt] = data;
    DebugCnt++;
  }
  
  return 1; // success
}

// *****Debug_Period******
// Calculate period of the recorded data using mask
// Input: mask specifies which bit(s) to observe
// Output: period in bus cycles
// Period is defined as rising edge (low to high) to the next rising edge.
// Return 0 if there is not enough collected data to calculate period .
uint32_t Debug_Period(uint32_t mask){
// students write this for Lab 3
// This function should not alter the recorded data.
// AND each recorded data with mask,
//    if nonzero the signal is considered high.
//    if zero, the signal is considered low.

  uint32_t last_time = 0, period_sum = 0, count = 0;
  int found_first = 0; //flag to see if the first rising edge has been found

  for (uint32_t i = 1; i < DebugCnt; i++) {

    if ((DataBuffer[i] & mask) != 0 && (DataBuffer[i - 1] & mask) == 0) {
      if (found_first!=0) {  
        period_sum += ( last_time-TimeBuffer[i]);  
        count++;
      }
      last_time = TimeBuffer[i];
      found_first = 1;


    } 
  }

  if (count == 0) {
    return 0;  // No valid periods found, return 0
  } else {
    return (period_sum / count);  // Return the computed average
  }



 
  return 42; // average period
}


// *****Debug_Duty******
// Calculate duty cycle of the recorded data using mask
// Input: mask specifies which bit(s) to observe
// Output: period in percent (0 to 100)
// Period is defined as rising edge (low to high) to the next rising edge.
// High is defined as rising edge (low to high) to the next falling edge.
// Duty cycle is (100*High)/Period
// Return 0 if there is not enough collected data to calculate duty cycle.
uint32_t Debug_Duty(uint32_t mask) {
    uint32_t last_rise_time = 0;
    uint32_t high_time_sum = 0, period_sum = 0, count = 0;
    int rising_detected = 0, high_started = 0;

    for (uint32_t i = 1; i < DebugCnt; i++) {
        uint32_t prev_state = DataBuffer[i - 1] & mask;
        uint32_t curr_state = DataBuffer[i] & mask;

        if (!prev_state && curr_state) { // Rising edge (low to high)
            if (rising_detected) {
                period_sum += (last_rise_time - TimeBuffer[i]); // Compute period
                count++;
            }
            last_rise_time = TimeBuffer[i];
            rising_detected = 1;
            high_started = 1;
        } 
        else if (prev_state && !curr_state) { // Falling edge (high to low)
            if (high_started!=0) {
                high_time_sum += (last_rise_time - TimeBuffer[i]); // Compute high time
                high_started = 0;
                
            }
        }
    }

    if (count == 0 || period_sum == 0) return 0; // No valid period found

    // Compute duty cycle: (100 * High Time) / Total Period
    return (100 * high_time_sum) / period_sum;
}

// Lab2 specific debugging code
uint32_t Theperiod;
uint32_t dutycycle;
void Dump(void){
  uint32_t out = GPIOB->DOUT31_0&0x0070000; // PB18-PB16 outputs
  uint32_t in = GPIOB->DIN31_0&0x0F;        // PB3-PB0 inputs
  uint32_t data = out|in;                   // PB18-PB16, PB3-PB0
  uint32_t result = Debug_Dump(data);       // calls your Lab3 function
  if(result == 0){ // 0 means full
    Theperiod = Debug_Period(1<<18);        // calls your Lab3 function
    dutycycle = Debug_Duty(1<<18);        // calls your Lab3 function
  __asm volatile("bkpt; \n"); // breakpoint here
// observe Theperiod
  }
}




