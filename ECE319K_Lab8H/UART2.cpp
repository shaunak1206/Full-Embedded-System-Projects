#include <ti/devices/msp/msp.h>
#include "UART2.h"
#include "../inc/Clock.h"
#include "../inc/LaunchPad.h"
#include "../inc/FIFO2.h"
#include "ti/devices/msp/m0p/mspm0g350x.h"


uint32_t LostData;
Queue FIFO2;


// power Domain PD0
// for 80MHz bus clock, UART2 clock is ULPCLK 40MHz
// initialize UART2 for 2375 baud rate
// no transmit, interrupt on receive timeout
void UART2_Init(void){
   // RSTCLR to GPIOA and UART2 peripherals
  // write this
   UART2->GPRCM.RSTCTL = 0xB1000003;
   UART2->GPRCM.PWREN = 0x26000001;
   Clock_Delay(24); // time for uart to power up
   IOMUX->SECCFG.PINCM[PA22INDEX]  = 0x00040082;
   UART2->CLKSEL = 0x08; // bus clock
   UART2->CLKDIV = 0x00; // no divide
   UART2->CTL0 &= ~0x01; // disable UART0
   UART2->CTL0 |= 0x00020018;


   UART2->IBRD = 1052;//842;
   UART2->FBRD = 40;//7;
   UART2->LCRH = 0x00000030;
   UART2->CPU_INT.IMASK = (1 << 10 ) | (1 << 0);


   UART2->IFLS = 0x0422;


   NVIC->ICPR[0] = 1<<14;
   NVIC->ISER[0] = 1<<14;
   NVIC->IP[3] = (NVIC->IP[3]&(~0x00C00000))|(2<<22); // fix 0xff part
   UART2->CTL0 |= 0x01; // enable UART2




}
//------------UART2_InChar------------
// Get new serial port receive data from FIFO2
// Input: none
// Output: Return 0 if the FIFO2 is empty
//         Return nonzero data from the FIFO1 if available
char UART2_InChar(void){
 char out;
// write this
 //  if(FIFO2.IsEmpty()){
 //    return 0;
 // }
 while(FIFO2.IsEmpty()){
//   //   return 0;
  }
  FIFO2.Get(&out);
 return out;
}


void static copyHardwareToSoftware(void){
 char letter;
 while(((UART2->STAT&0x04) == 0)){
   letter = UART2->RXDATA;
   FIFO2.Put(letter);
  
 }
}


extern "C" void UART2_IRQHandler(void);
void UART2_IRQHandler(void){ uint32_t status; char letter;
 status = UART2->CPU_INT.IIDX; // reading clears bit in RTOUT
 if(status == 0x01){   // 0x01 receive timeout
  
   GPIOB->DOUTTGL31_0 = BLUE; // toggle PB22 (minimally intrusive debugging)
   GPIOB->DOUTTGL31_0 = BLUE; // toggle PB22 (minimally intrusive debugging)
   // read all data, putting in FIFO
   // finish writing this
   copyHardwareToSoftware();
   GPIOB->DOUTTGL31_0 = BLUE; // toggle PB22 (minimally intrusive debugging)
 }
}
