#include<stdint.h>
#include"led.h"

void Leds_init(){
	
    uint32_t* RCC_AHB1ENR = (uint32_t*)(0x40023830);
	*RCC_AHB1ENR |= (0b1 << 3);
	
	uint32_t* GPIOD_MODER = (uint32_t*)(0x40020C00 + 0x00);
	*GPIOD_MODER &= ~(0xFF << 24); // reset Pod_D
	*GPIOD_MODER |= (0b01 << 24) |(0b01 << 26) |(0b01 << 28) | (0b01 << 30); // set GPOM
	uint32_t* GPIO_OTYPER = (uint32_t*)(0x40020C00 + 0x04);
	*GPIO_OTYPER &= ~(0b1111 << 12);
}


void Leds_ctrl(int led_num, int led_state){
	uint32_t* GPIO_ODR = (uint32_t*)(0x40020C00 +0x14);
	if(led_state == 1){
		*GPIO_ODR |= (1 << (led_num));
	}
	else
	{
		*GPIO_ODR &= ~(1 << (led_num));
	}
}

