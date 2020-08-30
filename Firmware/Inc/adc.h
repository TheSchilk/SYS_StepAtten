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

void adc_init();
uint32_t adc_getPotVal();

#endif /* ADC_H_ */
