#include"ADC_temperature.h"
#include<stdint.h>


void adc_init()
{
	
    uint32_t* RCC_APB2ENR = (uint32_t*)(0x40023844);
    *RCC_APB2ENR |= (0b1 << 8);
	uint32_t* SMR1 = (uint32_t*)0x4001200c;

	uint32_t* CR2 = (uint32_t*)0x40012008;
	uint32_t* ADC_JSQR = (uint32_t*)0x40012038;
	uint32_t* ADC_Common = (uint32_t*)0x40012304;


	*SMR1 |= (0b111 << 18);
	*ADC_JSQR &= ~(0b11 << 20);			// select 1 convertion
	*ADC_JSQR |= (16 << 15);			// select channel assigned(temperature) for jsq4
	*ADC_Common |= (0b1 << 23);			// enable temp sensor
	*CR2 |= (0b1 << 0);					// Enable ADC
}

static float adc_measure()
{
	uint32_t* CR2 = (uint32_t*)0x40012008;
	uint32_t* SR = (uint32_t*)0x40012000;

	*CR2 |= (1 << 22);		//Start conversion of injected channels
	while(((*SR >> 2)&1) !=1);
	*SR &= ~(1 << 2);
	uint32_t* ADC_JDR1 = (uint32_t*)0x4001203c;
	float read_sensor = (3.0*(*ADC_JDR1)/4095.0);
	return read_sensor;
}

float temp_measure()
{
    float Vsenso = adc_measure();
    const float v25 = 0.76;
    const float avg_slope = 2.5/1000;
    float temp = ((Vsenso - v25)/avg_slope) + 25;
    return temp;
}