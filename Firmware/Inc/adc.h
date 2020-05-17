/*
 * adc.h
 *
 *  Created on: Apr 18, 2020
 *      Author: pschilk
 */

#ifndef ADC_H_
#define ADC_H_

//Number of samples that are kept track of and averaged
#define ADC_MOVING_AVG_LEN   10

//Length of the DMA buffer
#define ADC_DMABUF_LEN       2*ADC_MOVING_AVG_LEN //2 Channels Sampled, so need 2x the length.

//DMA buffer
volatile uint16_t adc_dmabuf[2*ADC_MOVING_AVG_LEN];

void adc_init();
uint32_t adc_getPotVal(uint32_t use_external);

#endif /* ADC_H_ */
