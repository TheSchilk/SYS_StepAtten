/*
 * adc.h
 *
 *  Created on: Apr 18, 2020
 *      Author: pschilk
 */

#ifndef ADC_H_
#define ADC_H_


#define ADC_MOVING_AVG_LEN   10
#define ADC_DMABUF_LEN       2*ADC_MOVING_AVG_LEN

volatile uint32_t adc_dmabuf[2*ADC_MOVING_AVG_LEN];

void adc_init();
uint32_t adc_getPotVal(uint32_t use_external);

#endif /* ADC_H_ */
