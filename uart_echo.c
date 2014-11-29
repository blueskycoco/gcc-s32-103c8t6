#include <stdint.h>
#include <stdbool.h>
#include <stm32f10x.h>
#define CLEAR_SCREEN 0x01   
#define AC_INIT   0x02   	
#define CURSE_ADD  0x06   	
#define FUN_MODE  0x30   	
#define DISPLAY_ON  0x0c   
#define DISPLAY_OFF  0x08  
#define SET_CG_AC  0x40   	
#define SET_DD_AC  0x80
#define uchar unsigned char
const uchar addr_tab[]={
	0x80,0x81,0x82,0x83,0x84,0x85,0x86,0x87,
	0x90,0x91,0x92,0x93,0x94,0x95,0x96,0x97,
	0x88,0x89,0x8a,0x8b,0x8c,0x8d,0x8e,0x8f,
	0x98,0x99,0x9a,0x9b,0x9c,0x9d,0x9e,0x9f,
	};
void delay(int times)
{
	volatile long i,j;
	int m;
	for(m=0;m<times;m++)
		for(i=0;i<100000;i++)
			j=0;
}
void delay_ms(int times)
{
	volatile long i,j;
	int m;
	for(m=0;m<times;m++)
	for(i=0;i<300;i++)
			j=0;
}
void led_init()
{
	GPIO_InitTypeDef GPIO_InitStructure;

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC,ENABLE);

	GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;

	GPIO_InitStructure.GPIO_Pin   = GPIO_Pin_13;
	GPIO_Init(GPIOC, &GPIO_InitStructure);
}
void SendByte(uchar Dbyte)
{
	uchar i;
	for(i=0;i<8;i++)
	{
		//SCLK = 0;
		GPIO_ResetBits(GPIOA, GPIO_Pin_5);delay_ms(2); 
		
		GPIO_SetBits(GPIOA, GPIO_Pin_5);
		if((Dbyte&0x80)==0x80)
			GPIO_SetBits(GPIOA, GPIO_Pin_6);
		else
			GPIO_ResetBits(GPIOA, GPIO_Pin_6);
		// SCLK = 1;
		// SCLK = 0;
		//delay_ms(1);
		GPIO_SetBits(GPIOA, GPIO_Pin_5);		
		Dbyte=Dbyte<<1; 
		delay_ms(3);
	}
	GPIO_ResetBits(GPIOA, GPIO_Pin_5);
}

uchar ReceiveByte(void)
{
	uchar i,temp1,temp2;
	GPIO_InitTypeDef GPIO_InitStructure;
	temp1 = 0;
	temp2 = 0;
	GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_IPU;
	GPIO_InitStructure.GPIO_Pin   = GPIO_Pin_6;//data
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	for(i=0;i<8;i++)
	{
		temp1=temp1<<1;
		GPIO_ResetBits(GPIOA, GPIO_Pin_5);delay_ms(1);
		GPIO_SetBits(GPIOA, GPIO_Pin_5);delay_ms(1);
		GPIO_ResetBits(GPIOA, GPIO_Pin_5);//delay_ms(1);
		//SCLK = 0;
		//SCLK = 1;          
		//SCLK = 0;
		if(GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_6)==Bit_SET) temp1++;
	}
	for(i=0;i<8;i++)
	{
		temp2=temp2<<1;
		GPIO_ResetBits(GPIOA, GPIO_Pin_5);delay_ms(1);
		GPIO_SetBits(GPIOA, GPIO_Pin_5);delay_ms(1);
		GPIO_ResetBits(GPIOA, GPIO_Pin_5);//delay_ms(1);
		//SCLK = 0;
		//SCLK = 1;
		//SCLK = 0;
		if(GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_6)==Bit_SET) temp2++;
	}
	GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	return ((0xf0&temp1)+(0x0f&temp2));
}

void CheckBusy( void )
{
	do   SendByte(0xfc);         //11111,RW(1),RS(0),0
	while(0x80&ReceiveByte());       //BF(.7)=1 Busy
}

void Lcd_WriteCmd(uchar Cbyte )
{
	//CS = 1;
	GPIO_SetBits(GPIOA, GPIO_Pin_4);
	delay_ms(1);
	//CheckBusy();
	SendByte(0xf8);            
	SendByte(0xf0&Cbyte);        
	SendByte(0xf0&(Cbyte<<4));  
	// CS = 0;
	GPIO_ResetBits(GPIOA, GPIO_Pin_4);
	delay_ms(1);
}

void Lcd_WriteData(uchar Dbyte )
{
	//CS = 1;
	GPIO_SetBits(GPIOA, GPIO_Pin_4);
	delay_ms(1);
	//CheckBusy();
	SendByte(0xfa);              //11111,RW(0),RS(1),0
	SendByte(0xf0&Dbyte);        
	SendByte(0xf0&(Dbyte<<4));   
	//CS = 0;
	GPIO_ResetBits(GPIOA, GPIO_Pin_4);
	delay_ms(1);
}

void Lcd_Init(void)
{
	//    PSB = 0;
	GPIO_InitTypeDef GPIO_InitStructure;

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);

	GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;

	GPIO_InitStructure.GPIO_Pin   = GPIO_Pin_4;//cs
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	GPIO_InitStructure.GPIO_Pin   = GPIO_Pin_5;//clk
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	GPIO_InitStructure.GPIO_Pin   = GPIO_Pin_6;//data
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	GPIO_ResetBits(GPIOA, GPIO_Pin_4);
	GPIO_ResetBits(GPIOA, GPIO_Pin_5);
	GPIO_ResetBits(GPIOA, GPIO_Pin_6);
	delay(10);
	Lcd_WriteCmd(0x30);       
	delay(1);
	Lcd_WriteCmd(0x30);          
	delay(10);
	Lcd_WriteCmd(0x0c);         
	delay(10);
	Lcd_WriteCmd(0x01);         
	//delay(10);
	//Lcd_WriteCmd(0x02);         
	//delay(10);
	//Lcd_WriteCmd(0x80);         
	delay(10);
}

void hanzi_Disp(uchar x,uchar y,uchar *s)
{
	Lcd_WriteCmd(addr_tab[8*x+y]);  
	while(*s>0)
	{
		Lcd_WriteData(*s);    
		s++;   
	}
}
//*****************************************************************************
//
// This example demonstrates how to send a string of data to the UART.
//
//*****************************************************************************
	int
main(void)
{
	//led_init();
	//unsigned char buf[20]={0xb0,0xa1,0xb0,0xa2,0xb0,0xa3,0xb0,0xa4,0xb0,0xa5,0xb0,0xa6,0xb0,0xa7,0xb0,0xa8,0xb0,0xa9,0xb0,0xaa};
	//unsigned char buf1[20]={'�?,'�?};
	Lcd_Init();
	hanzi_Disp(0,0,"adbcde");
	//
	// Loop forever echoing data through the UART.
	//
	while(1)
	{
		//Lcd_Init();
		hanzi_Disp(0,0,"ffffgg");
		//GPIO_SetBits(GPIOC, GPIO_Pin_13);
		delay(10);
		hanzi_Disp(0,0,"abcdefghijkm");
		//GPIO_ResetBits(GPIOC, GPIO_Pin_13);
		delay(10);
	}
}
