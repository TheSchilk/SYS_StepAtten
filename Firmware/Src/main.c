#include <sys_stepatten.h>

int main(void)
{
	// TODO ADC DMA / Read
	// TODO Attenuator CTRL
	// TODO Button Debounce
	// TODO Select CTRL
	// TODO Save State to Flash

	gpio_init();
	adc_init();
	systick_init();

	gpio_set_mute(0);
	gpio_set_select(0x01);


	while(1){


		ADC_StartOfConversion(ADC1);
		while(ADC_GetFlagStatus(ADC1, ADC_FLAG_EOC) == RESET);
		uint32_t potval = ADC_GetConversionValue(ADC1);


		potval = (potval >> 5);

		if(potval == 0){
			gpio_set_mute(1);
		} else {
			gpio_set_atten(potval);
			gpio_set_mute(0);
		}


		for(int i = 0; i < 50000; i++){

		}

	}
}
