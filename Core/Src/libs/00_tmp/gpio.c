// Functions to manage the GPIO peripheral:
//   - configure direction, mode and pull-up/pull-down for a set of GPIO pins
//   - configure output mode for a set of GPIO pins
//   - configure alternative function mapping
//   - configure an EXTI line
#include "sw_mcu_conf.h"

#include "gpio.h"


// Set the direction, mode and pull-up for a set of GPIO pins on a given GPIO port
// input:
//   GPIOx - pointer to a GPIO peripheral handle
//   Mode - pin mode (one of GPIO_Mode_xx values)
//   PUPD - pull-up configuration (one of GPIO_PUPD_xx values)
//   Pins - set of pins (ORed combination of GPIO_Pin_X values)
void GPIO_set_mode(GPIO_TypeDef *GPIOx, GPIOMode_TypeDef Mode, GPIOPUPD_TypeDef PUPD, uint16_t Pins) {
	uint16_t pin = 0;
	uint32_t MODER = GPIOx->MODER;
	uint32_t PUPDR = GPIOx->PUPDR;

	// Set the configuration only for the pins mentioned in Pins parameter
	while (pin < 16) {
		if ((1 << pin) & Pins) {
			// Pin mode
			MODER &= ~GPIO_MODE_MSK(pin);
			MODER |=  GPIO_MODE_SET(pin,Mode);

			// Pull-up configuration
			PUPDR &= ~GPIO_PUPD_MSK(pin);
			PUPDR |=  GPIO_PUPD_SET(pin,PUPD);
		}
		pin++;
	}

	// Write new values of the mode and pull-up/pull-down control registers
	GPIOx->MODER = MODER;
	GPIOx->PUPDR = PUPDR;
}

// Set the type and speed for a set of GPIO pins configured as output on a given GPIO port
// input:
//   GPIOx - pointer to a GPIO peripheral handle
//   OT - output type of the GPIO pin (one of GPIOOT_xx values)
//   Speed - output speed of the GPIO pin (one of GPIOSPD_xx values)
//   Pins - set of pins (ORed combination of GPIO_Pin_X values)
void GPIO_out_cfg(GPIO_TypeDef *GPIOx, GPIOOT_TypeDef OT, GPIOSPD_TypeDef Speed, uint16_t Pins) {
	uint16_t pin = 0;
	uint32_t OSPEEDR = GPIOx->OSPEEDR;

	// Set output type only for the pins mentioned in Pins parameter
	if (OT == GPIO_OT_PP) {
		GPIOx->OTYPER &= ~Pins;
	} else {
		GPIOx->OTYPER |=  Pins;
	}

	// Set the configuration only for the pins mentioned in Pins parameter
	while (pin < 16) {
		if ((1 << pin) & Pins) {
			// Set output speed
			OSPEEDR &= ~GPIO_SPD_MSK(pin);
			OSPEEDR |=  GPIO_SPD_SET(pin,Speed);
		}
		pin++;
	}

	// Write new value of the speed control register
	GPIOx->OSPEEDR = OSPEEDR;
}

// Set the alternative function mapping for a GPIO pin on a given GPIO port
// input:
//   GPIOx - pointer to a GPIO peripheral handle
//   Pin - specified the pin for the alternative function (one of GPIO_PinSourceXX values)
//   AF - pin alternative function number, to know which functions are multiplexed on each GPIO pin,
//        refer to the MCU datasheet (table "Alternate function input/output")
//        can be one of GPIO_AFx values
void GPIO_af_cfg(GPIO_TypeDef *GPIOx, uint16_t Pin, uint8_t AF) {
	uint32_t AFR = GPIOx->AFR[Pin >> 3];

	// Set the alternative function for a GPIO pin
	AFR &= ~GPIO_AF_MSK(Pin & 0x07);
	AFR |=  GPIO_AF_SET(Pin & 0x07,AF);

	// Write new value of the alternate function register
	GPIOx->AFR[Pin >> 3] = AFR;
}
