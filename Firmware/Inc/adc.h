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
#define ADC_DMABUF_LEN       3*ADC_MOVING_AVG_LEN //3 Channels Sampled, so need 3x the length.

// Position of the readings in the buffer
#define ADC_POT_BUFOFFSET          2
#define ADC_EXT_SENSE_BUFOFFSET    0
#define ADC_EXT_POT_BUFOFFSET      1

// Max ADC reading
#define ADC_MAX                    0xFFF // 12bits
// Threshold for detection of external control
#define ADC_THRH_SENSE             (ADC_MAX/3)*2
// Threshold for detection of external switch
#define ADC_THRH_SW                (ADC_MAX/3)

#define ADC_INT_ADD  0
#define ADC_INT_MULT 102
#define ADC_INT_DIV  100

#define ADC_EXT_ADD  0
#define ADC_EXT_MULT 102
#define ADC_EXT_DIV  100


void adc_init();

uint32_t adc_getPotVal();
uint32_t adc_ext_sense();
int32_t  adc_getExtSwPosition();

#endif /* ADC_H_ */
