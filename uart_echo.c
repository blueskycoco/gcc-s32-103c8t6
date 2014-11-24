#include <stdint.h>
#include <stdbool.h>
#include <stm32f10x.h>
void delay(int times)
{
	volatile long i,j;
	int m;
	for(m=0;m<times;m++)
	for(i=0;i<100000;i++)
		j=0;
}
//*****************************************************************************
//
// This example demonstrates how to send a string of data to the UART.
//
//*****************************************************************************
int
main(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;

    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC,ENABLE);

    GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_Out_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;

    GPIO_InitStructure.GPIO_Pin   = GPIO_Pin_13;
    GPIO_Init(GPIOC, &GPIO_InitStructure);
    //
    // Loop forever echoing data through the UART.
    //
    while(1)
    {
		GPIO_SetBits(GPIOC, GPIO_Pin_13);
		delay(10);
		GPIO_ResetBits(GPIOC, GPIO_Pin_13);
		delay(10);
    }
}
