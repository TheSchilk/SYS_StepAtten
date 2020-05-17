#include <sys_stepatten.h>

int main(void)
{
	// Relay state that selects a specific input
	const uint32_t input_relay_code[INPUT_COUNT] = {0x2,0x0,0x5,0x1};
	// LEDs states that indicate different inputs
	const uint32_t input_led_state[INPUT_COUNT] = {0x1,0x2,0x4,0x7};


	// TODO disable mode
	// TODO Save State to Flash
	// TODO poti dbnc?

	uint32_t current_input = 0;
	uint32_t input_disabled[INPUT_COUNT] = {0,0,0,0};

	systick_init(); // Configure the SysTsick
  	gpio_init();    // Configre all GPIOs
	adc_init();     // Configure the ADC

	// TODO Read last input state from flash and set it here

	// Set initial Input
	gpio_write_leds_decay(input_led_state[current_input], LED_DECAY_MS);
	gpio_set_select(input_relay_code[current_input]);

	//Main Loop
	while(1){
		// Check if the internal or external potentiometer is selected:
		uint32_t use_external_ctrl = gpio_ext_pot_selected();

		// Get the current position of the selected potentiometer
		uint32_t pot_postion = adc_getPotVal(use_external_ctrl);

		// Scale it and write it to the output
		gpio_set_atten(pot_postion >> 5);

		// Mute if the lowest position is selected
		if(pot_postion == 0){
			gpio_set_mute(1);
		} else {
			gpio_set_mute(0);
		}

		// Update the selected input:
		if(gpio_btn_toggled()){

			// Find the next enabled input:
			do{
				current_input = (current_input + 1) % INPUT_COUNT;
			} while(input_disabled[current_input]);

			// Turn on the status led showing which input is selected
			gpio_write_leds_decay(input_led_state[current_input], LED_DECAY_MS);

			// Select the input
			gpio_set_select(input_relay_code[current_input]);
		}

		systick_dlyms(100);

	}
}
