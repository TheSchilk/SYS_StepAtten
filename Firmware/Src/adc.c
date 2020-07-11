/*
 * adc.c
 *
 *  Created on: Apr 18, 2020
 *      Author: pschilk
 */


#include "sys_stepatten.h"

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
	ADC_ChannelConfig(ADC1, AN_CH_POT,     ADC_SampleTime_7_5Cycles);
	ADC_ChannelConfig(ADC1, AN_CH_POT_EXT, ADC_SampleTime_7_5Cycles);

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

uint32_t adc_getPotVal(uint32_t use_external){

	//The ADC continuously samples both ADC Channels
	//Both values are stored sequentially in the DAM buffer
	//Decide if the first or second value of each pair should be used.

	uint32_t pot_select_offset;
	if(use_external){
		pot_select_offset = (AN_CH_POT_EXT > AN_CH_POT) ? 1 : 0;
	} else {
		pot_select_offset = (AN_CH_POT_EXT > AN_CH_POT) ? 0 : 1;
	}

	//Iterate over the ADC samples for the correct channel and sum all readings
	//If the buffer has not yet been completely filled, the unwritten values
	//are initialized to 0xFFFFFFFF and can be ignored.
	uint32_t sum = 0;
	uint32_t count = 0;
	for(int i = 0; i < ADC_DMABUF_LEN; i = i + 2){
		if(adc_dmabuf[i+pot_select_offset] != 0xFFFF){
			sum += adc_dmabuf[i+pot_select_offset];
			count++;
		}
	}

	//If there are no samples in the buffer yet, return 0
	if(count == 0){
		return 0;
	} else {
		//Return average
		return sum/count;
	}
}
