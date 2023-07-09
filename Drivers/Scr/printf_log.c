#include<stdint.h>
#include"printf_log.h"
#include<stdio.h>
#include<stdarg.h>
#include<string.h>

void UART_init()
{
	//todo: Enable clk GPIO A with alternate function
    uint32_t* RCC_AHB1ENR = (uint32_t*)(0x40023830);
    *RCC_AHB1ENR |= (0b1 << 0);
	uint32_t* GPIOA_Moder = (uint32_t*)(0x40020000 + 0x00);
	*GPIOA_Moder |= (0b10<<4)|(0b10<<6); //alternate function
	//todo: set AFL
	uint32_t* GPIOA_AFRL = (uint32_t*)(0x40020000+0x20);
	*GPIOA_AFRL |= (0b0111<<8) | (0b0111<<12);
	//todo: Enable Clk UART2
	
    uint32_t* RCC_APB1ENR = (uint32_t*)(0x40023840);
    *RCC_APB1ENR |= (0b1 << 17);
	//todo: set baudrate
	uint32_t* UART_BRR = (uint32_t*)(0x40004400+0x08);
	*UART_BRR = (104<<4) | (3<<0);

	//todo: frame data: 8bit data and no parity
	uint32_t* CR1 = (uint32_t*)(0x40004400+0x0C);
	*CR1 &= ~(1<<12); //word length (8bit)
	*CR1 &= ~(1<<10); //parity ctrl enable(non)


	//todo: enable UART enable Tx Rx
	*CR1 |= (1<<13) | (1<<3) |(1<<2);

}

static void UART_Send_Byte(char _data)
{
	//todo: send data
	uint32_t* USART_DR = (uint32_t*)(0x40004400 + 0x04);
	*USART_DR = _data;

	//todo: check status data wait transmittion complete -> read bits 6
	uint32_t* UART_SR = (uint32_t*)(0x40004400+0x00);
	while(((*UART_SR >> 6)&1) == 0);
	*UART_SR &= ~(1<<6); // reset bit;
	while(((*UART_SR >> 7)&1) == 0);

}

void printf_Log(char* format, ...)
{
	va_list arg;
	va_start (arg, format);
	char send_buff [128] = {0};
	vsprintf(send_buff,format, arg);
	va_end(arg);
	int str_len = strlen(send_buff);
	for(int i = 0; i<str_len;i++)
	{
		UART_Send_Byte(send_buff[i]);
	}
}
