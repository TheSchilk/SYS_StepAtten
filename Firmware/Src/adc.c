/*
 * adc.c
 *
 *  Created on: Apr 18, 2020
 *      Author: pschilk
 */


#include "sys_stepatten.h"

// DMA buffer
volatile uint16_t adc_dmabuf[ADC_DMABUF_LEN];


int32_t adc_avgbuf(uint32_t offset);
uint32_t adc_scale(int32_t val, int32_t a, int32_t m, int32_t d);

// Configure ADC
// Runs continuously and DMAs all readings into the
// circular buffer adc_dmabuf
void adc_init(){
	//Turn on ADC Clock
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1, ENABLE);
	ADC_ClockModeConfig(ADC1, ADC_ClockMode_SynClkDiv4);

	//Configure the ADC peripheral
	ADC_InitTypeDef adc_struct;
	ADC_StructInit(&adc_struct);
	adc_struct.ADC_Resolution = ADC_Resolution_12b;
	adc_struct.ADC_ContinuousConvMode = ENABLE;
	adc_struct.ADC_DataAlign = ADC_DataAlign_Right;
	adc_struct.ADC_ScanDirection = ADC_ScanDirection_Upward;

	//Configure the two ADC Channels we are using
	ADC_ChannelConfig(ADC1, AN_CH_POT,       ADC_SampleTime_7_5Cycles);
	ADC_ChannelConfig(ADC1, AN_CH_POT_EXT,   ADC_SampleTime_7_5Cycles);
	ADC_ChannelConfig(ADC1, AN_CH_SENSE_EXT, ADC_SampleTime_7_5Cycles);

	//Setup DMA
    RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1, ENABLE);
	DMA_InitTypeDef initdma = {0};

	initdma.DMA_BufferSize = ADC_DMABUF_LEN;
	initdma.DMA_DIR = DMA_DIR_PeripheralSRC;
	initdma.DMA_M2M = DMA_M2M_Disable;
	initdma.DMA_MemoryBaseAddr = (uint32_t) adc_dmabuf;
	initdma.DMA_MemoryDataSize = DMA_MemoryDataSize_HalfWord;
	initdma.DMA_MemoryInc = DMA_MemoryInc_Enable;
	initdma.DMA_Mode = DMA_Mode_Circular;
	initdma.DMA_PeripheralBaseAddr = (uint32_t) &ADC1->DR;
	initdma.DMA_PeripheralDataSize = DMA_PeripheralDataSize_HalfWord;
	initdma.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
	initdma.DMA_Priority = DMA_Priority_High;
	DMA_Init(DMA1_Channel1,&initdma);
	DMA_Cmd(DMA1_Channel1, ENABLE);

    ADC_DMARequestModeConfig(ADC1, ADC_DMAMode_Circular);
	ADC_DMACmd(ADC1, ENABLE);

	// Fill buffer with 0xFFFF
	// Indicates that no sample is present
	for(int i = 0; i < ADC_DMABUF_LEN; i++){
		adc_dmabuf[i] = 0xFFFF;
	}

	//Turn on ADC
	ADC_Cmd(ADC1, ENABLE);

	 // Wait until the ADC is ready
	while(!ADC_GetFlagStatus(ADC1, ADC_FLAG_ADRDY));

	//Start Conversion
	ADC_ContinuousModeCmd(ADC1, ENABLE);
	ADC_StartOfConversion(ADC1);
}


int32_t adc_avgbuf(uint32_t offset){
	// The ADC continuously samples all 3 channles,
	// these values are store sequentially in the ciruclar
	// DMA buffer.
	// This function extracts one channel give it's offset
	// and averages the readings.
	// Returns -1 if there are no readings yet.

	uint32_t sum = 0;
	uint32_t count = 0;
	for(int i = 0; i < ADC_DMABUF_LEN; i = i + 3){
		// If there is no reading yet, the DMA buffer
		// will read 0xFFFF
		if(adc_dmabuf[i+offset] != 0xFFFF){
			sum += adc_dmabuf[i+offset];
			count++;
		}
	}


	//If there are no samples in the buffer yet, return -1
	if(count == 0){
		return -1;
	}

	//Return average
	return sum/count;
}


uint32_t adc_scale(int32_t val, int32_t a, int32_t m, int32_t d){
	if(val == -1){
		return val;
	}
	int32_t result = ((val + a)*m)/d;
	result = (result > ADC_MAX) ? ADC_MAX : result;
	result = (result < 0) ? 0 : result;
	return result;
}

// Return 1 if there is an external control unit connected, 0 otherwise.
uint32_t adc_ext_sense(){
	// Get the average of the last readings of the sense voltage
	int32_t reading = adc_avgbuf(ADC_EXT_SENSE_BUFOFFSET);

	// if there are no readings yet, stay with the internal potentiometer
	if(reading == -1){
		return 0;
	}

	// if the reading is below the sense threshold, use the external potentiometer
	if(reading <= ADC_THRH_SENSE){
		return 1;
	} else {
		return 0;
	}
}

// Get the average of the last ADC readings
// for the selected potentiometer
uint32_t adc_getPotVal(){
	if(adc_ext_sense()){
		// Extenal Potentiometer
		uint32_t reading = adc_avgbuf(ADC_EXT_POT_BUFOFFSET);
		return adc_scale(reading, ADC_EXT_ADD, ADC_EXT_MULT, ADC_EXT_DIV);
	} else {
		// Internal Potentiometer
		uint32_t reading =  adc_avgbuf(ADC_POT_BUFOFFSET);
		return adc_scale(reading, ADC_INT_ADD, ADC_INT_MULT, ADC_INT_DIV);
	}
}

// Return the position of the external switch (1/0)
// Returns -1 if no external switch is found.
int32_t adc_getExtSwPosition(){
	int32_t reading = adc_avgbuf(ADC_EXT_SENSE_BUFOFFSET);

	// If there are no readings, return -1
	if(reading == -1){
		return -1;
	}

	// if there is no external switch connected, return -1
	if(reading > ADC_THRH_SENSE){
		return -1;
	}

	// if the reading is below the threshold,
	// the switch is pressed. Return 1.
	if(reading <= ADC_THRH_SW){
		return 1;
	} else {
		return 0;
	}
}
