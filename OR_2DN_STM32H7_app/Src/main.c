#include "stm32h750xx.h"
#include <stdio.h>
#include "uart.h"
#include "systick.h"
#include "adc.h"

// External LED: 		D8 -> PE3
// Water sensor: 		D7 -> PI8
// Temperature sensor:	A1 -> PF8

#define GPIOEEN		(1u << 4)
#define GPIOIEN		(1u << 8)
#define GPIOJEN		(1u << 9)

#define PIN3		(1u << 3)
#define EXT_LED		PIN3

#define PIN8		(1u << 8)
#define TEMP_PIN	PIN8

#define WATER_PIN	PIN8


uint32_t sensor_val;

int main(void)
{
	// connect clock to PD and PJ
	RCC->AHB4ENR |= (GPIOEEN | GPIOJEN | GPIOIEN);

	// set PE3 (external LED) to output mode (01)
	GPIOE->MODER |=  (1u << 6);
	GPIOE->MODER &= ~(1u << 7);

	// set PJ2 (internal LED) to output mode (01)
	GPIOJ->MODER |=  (1u << 4);
	GPIOJ->MODER &= ~(1u << 5);

	// set PI8 (water sensor) as input (00)
	GPIOI->MODER &= ~(1u << 16);
	GPIOI->MODER &= ~(1u << 17);


	uart3_tx_init();
	systick_init();
	pf8_adc_init();

	// Start temperature conversion
	start_continuous_conversion();

	while(1)
	{
		// start_single_conversion();
		sensor_val = adc_read();
		printf("Temperature: 0x%X (%d), ", (int) sensor_val, (int) sensor_val);


		// Check if water sensor has detected anything
		int water = (GPIOI->IDR & WATER_PIN) > 0;
		printf("Water: %d\n\r", water);
		if( water )
		{
			// enable external LED (on pin 0)
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
