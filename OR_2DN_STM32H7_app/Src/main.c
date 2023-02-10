#include "stm32h750xx.h"
#include <stdio.h>
#include "uart.h"
#include "systick.h"
#include "adc.h"
#include "fpu.h"

// Water sensor: 		D7 -> PI8
// Light sensor:		A1 -> PF8
// External water LED:	D8 -> PE3
// External light LED:	D4 -> PK1


#define GPIOEEN			(1u << 4)
#define GPIOIEN			(1u << 8)
#define GPIOKEN			(1u << 10)

#define PIN3			(1u << 3)
#define PIN8			(1u << 8)
#define	PIN1			(1u << 1)

#define EXT_WATER_LED	PIN3
#define	EXT_LIGHT_LED	PIN1
#define LIGHT_SENS_PIN	PIN8
#define WATER_SENS_PIN	PIN8

#define BSRR_OFFSET		(16u)

int main(void)
{
	fpu_enable();

	// connect clock to PD, PI, PK
	RCC->AHB4ENR |= (GPIOEEN | GPIOIEN | GPIOKEN);

	// set PE3 (external water LED) to output mode (01)
	GPIOE->MODER |=  (1u << 6);
	GPIOE->MODER &= ~(1u << 7);

	// set PI8 (water sensor) as input (00)
	GPIOI->MODER &= ~(1u << 16);
	GPIOI->MODER &= ~(1u << 17);

	// set PK1 (external light LED) to output mode (01)
	GPIOK->MODER |=  (1u << 2);
	GPIOK->MODER &= ~(1u << 3);

	uart3_tx_init();
	systick_init();
	pf8_adc_init();

	// Start temperature conversion
	start_continuous_conversion();

	uint32_t sensor_val;

	while(1)
	{

		// start_single_conversion();
		sensor_val = adc_read();

		// Check if water sensor has detected anything
		char water = (GPIOI->IDR & WATER_SENS_PIN) > 0;
		printf("Sensor value: %d, Water: %d\n\r", (int) sensor_val, water);

		if( sensor_val < 3000 ) {
			// very little light has been detected

			// turn on the lights
			GPIOK->BSRR = (1u << 1);
		}
		else {
			// a decent amount of light has been detected
			// turn off the lights
			GPIOK->BSRR = (1u << 17);

		}

		if( water )
		{
			// enable external LED (on pin 3)
			GPIOE->BSRR = (1u << 3);
		}
		else
		{
			// disable external LED
			GPIOE->BSRR = (1u << 19);
		}

		systick_delay_ms(1000);
	}


	systick_terminate();
}
