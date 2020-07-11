#include <sys_stepatten.h>

int main(void)
{
	// Relay state that selects a specific input
	const uint32_t input_relay_code[INPUT_COUNT] = {0x0,0x2,0x1,0x3};
	// LEDs states that indicate different inputs
	const uint32_t input_led_state[INPUT_COUNT] = {0x1,0x2,0x4,0x7};

	// TODO Save State to Flash
	// TODO poti dbnc?

	uint32_t current_input = 0;

	systick_init(); // Configure the SysTsick
  	gpio_init();    // Configre all GPIOs
	adc_init();     // Configure the ADC

	// TODO Read last input state from flash and set it here



	STATE_T state;
	if(gpio_sw_state() == 0){
		// Start in normal mode
		gpio_leds(input_led_state[current_input], leds_decay, 800);
		gpio_set_select(input_relay_code[current_input]);
		state = state_normal;
	} else {
		// Start in select mode
		state = state_select;
	}

	//Main Loop
	while(1){
		// Get the current position of the potentiometer
		uint32_t pot_postion = adc_getPotVal(0);

		switch(state){
		case state_normal:
			// Write current position to the attenuator
			gpio_set_atten(pot_postion >> 5);

			// Mute if the lowest position is selected
			if(pot_postion == 0){
				gpio_set_mute(1);
			} else {
				gpio_set_mute(0);
			}

			// If switch is pressed, go to selection state
			if(gpio_sw_state()){
				state = state_select;
			}

			break;
		case state_select:
			// Make sure we are muted
			gpio_set_mute(1);

			// See which input is selected
			current_input = (pot_postion) / (4096/INPUT_COUNT);

			// Make sure it is a valid index (rounding errors)
			current_input = current_input >= INPUT_COUNT ? current_input - 1 : current_input;

			gpio_leds(input_led_state[current_input], leds_normal, 0);

			// Once the switch is released, go either to normal or safety mode
			if(gpio_sw_state() == 0){
				if((pot_postion >> 5) == 0){
					// Safe to go back to normal mode
					gpio_set_atten(0);
					gpio_leds(input_led_state[current_input], leds_decay, 800);
					state = state_normal;
				} else {
					// Go to safety mode
					gpio_leds(input_led_state[current_input], leds_blink, 200);
					state = state_safety;
				}
			}

			break;
		case state_safety:
			// Make sure we are muted
			gpio_set_mute(0);

			// if the switch gets pressed again, go to selection mode
			if(gpio_sw_state()){
				state = state_select;
			} else if((pot_postion >> 5) == 0){
				// Otherwise, back to normal mode as soon as volume is at 0
				gpio_set_atten(0);
				state = state_normal;
				gpio_leds(input_led_state[current_input], leds_decay, 800);
			}
			break;
		default:
			state = state_normal; // Should not happen
		}

		systick_dlyms(100);

	}
}
