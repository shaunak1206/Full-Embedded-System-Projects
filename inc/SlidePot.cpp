/* SlidePot.cpp
 * Students: Your Name
 * Modified: 04/01/2025
 * 12-bit ADC input on ADC1 channel 5, PB18
 */
#include <ti/devices/msp/msp.h>
#include "../inc/Clock.h"
#include "../inc/SlidePot.h"

#define ADCVREF_VDDA 0x000
#define ADCVREF_INT  0x200

SlidePot::SlidePot(uint32_t m, uint32_t b) {
  this->slope = m;
  this->offset = b;
  this->data = 0;
  this->distance = 0;
  this->flag = 0;
}

void SlidePot::Init(void){
  // Activate ADC1 and GPIOB
  ADC1->ULLMEM.GPRCM.RSTCTL = 0xB1000003;
  ADC1->ULLMEM.GPRCM.PWREN = 0x26000001;
  Clock_Delay(24);
  ADC1->ULLMEM.GPRCM.CLKCFG = 0xA9000000;
  ADC1->ULLMEM.CLKFREQ = 7; // 40-48 MHz
  ADC1->ULLMEM.CTL0 = 0x03010000; // divide by 8, disable conversion
  ADC1->ULLMEM.CTL1 = 0x00000000; // software trigger
  ADC1->ULLMEM.CTL2 = 0x00000000; // 12-bit, right justified
  ADC1->ULLMEM.MEMCTL[0] = 5;     // channel 5 (PB18)
  ADC1->ULLMEM.SCOMP0 = 0;        // 8 clocks to sample
  ADC1->ULLMEM.CPU_INT.IMASK = 0; // no interrupts
  //ADC1->ULLMEM.CTL0 |= 0x01;      // enable ADC1
}

uint32_t SlidePot::In(void){
  ADC1->ULLMEM.CTL0 |= 0x00000001;       // enable conversions
  ADC1->ULLMEM.CTL1 |= 0x00000100;       // start conversion
  volatile uint32_t delay = ADC1->ULLMEM.STATUS;
  while((ADC1->ULLMEM.STATUS & 0x01) == 0x01){} // wait for complete
  return ADC1->ULLMEM.MEMRES[0];         // 12-bit result
}

void SlidePot::Save(uint32_t n){
  data = n;
  distance = Convert(n);
  flag = 1;
}

uint32_t SlidePot::Convert(uint32_t n){
  int32_t result = ((slope * n) >> 12) + offset;
  if(result < 0) return 0;
  return result;
}

float SlidePot::FloatConvert(uint32_t input){
  float result = (((input * 0.44323638f) - 152.8352854f) / 1000.0f);
  if(result < 0.0f) return 0.0f;
  return result;
}
void SlidePot::Sync(void){
  while(flag == 0){};
  flag = 0;
}

uint32_t SlidePot::Distance(void){
  return distance;
}