#include <stdint.h>
#include <stdbool.h>
#include <stm32f10x.h>
#define CLEAR_SCREEN 0x01   //清屏指令：清屏且AC值为00H
#define AC_INIT   0x02   	//将AC设置为00H。且游标移到原点位置
#define CURSE_ADD  0x06   	//设定游标移到方向及图像整体移动方向（默认游标右移，图像整体不动）
#define FUN_MODE  0x30   	//工作模式：8位基本指令集
#define DISPLAY_ON  0x0c   	//显示开,显示游标，且游标位置反白
#define DISPLAY_OFF  0x08   //显示关
#define CURSE_DIR  0x14   	//游标向右移动:AC=AC+1
#define SET_CG_AC  0x40   	//设置AC，范围为：00H~3FH
#define SET_DD_AC  0x80
#define uchar unsigned char
const uchar addr_tab[]={
	0x80,0x81,0x82,0x83,0x84,0x85,0x86,0x87,//第一行汉字位置
	0x90,0x91,0x92,0x93,0x94,0x95,0x96,0x97,//第二行汉字位置
	0x88,0x89,0x8a,0x8b,0x8c,0x8d,0x8e,0x8f,//第三行汉字位置
	0x98,0x99,0x9a,0x9b,0x9c,0x9d,0x9e,0x9f,//第四行汉字位置
};
void delay(int times)
{
	volatile long i,j;
	int m;
	for(m=0;m<times;m++)
		for(i=0;i<100000;i++)
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
		GPIO_ResetBits(GPIOA, GPIO_Pin_5);
		Dbyte=Dbyte<<1;        //左移一位
		//SID = CY;              //移出的位给SID
		if((Dbyte&0x80)==0x80)
			GPIO_SetBits(GPIOA, GPIO_Pin_6);
		else
			GPIO_ResetBits(GPIOA, GPIO_Pin_6);
		// SCLK = 1;
		// SCLK = 0;
		GPIO_SetBits(GPIOA, GPIO_Pin_5);
		GPIO_ResetBits(GPIOA, GPIO_Pin_5);
	}
}

uchar ReceiveByte(void)
{
	uchar i,temp1,temp2;
	GPIO_InitTypeDef GPIO_InitStructure;
	temp1 = 0;
	temp2 = 0;
	GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_IN_FLOATING;
	GPIO_InitStructure.GPIO_Pin   = GPIO_Pin_6;//data
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	for(i=0;i<8;i++)
	{
		temp1=temp1<<1;
		GPIO_ResetBits(GPIOA, GPIO_Pin_5);
		GPIO_SetBits(GPIOA, GPIO_Pin_5);
		GPIO_ResetBits(GPIOA, GPIO_Pin_5);
		//SCLK = 0;
		//SCLK = 1;          
		//SCLK = 0;
		if(GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_6)==Bit_SET) temp1++;
	}
	for(i=0;i<8;i++)
	{
		temp2=temp2<<1;
		GPIO_ResetBits(GPIOA, GPIO_Pin_5);
		GPIO_SetBits(GPIOA, GPIO_Pin_5);
		GPIO_ResetBits(GPIOA, GPIO_Pin_5);
		//SCLK = 0;
		//SCLK = 1;
		//SCLK = 0;
		if(GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_6)==Bit_SET) temp2++;
	}
	GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_Out_PP;
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
	CheckBusy();
	SendByte(0xf8);              //11111,RW(0),RS(0),0
	SendByte(0xf0&Cbyte);        //高四位
	SendByte(0xf0&Cbyte<<4);   //低四位(先执行<<)
	// CS = 0;
	GPIO_ResetBits(GPIOA, GPIO_Pin_4);
}

void Lcd_WriteData(uchar Dbyte )
{
	//CS = 1;
	GPIO_SetBits(GPIOA, GPIO_Pin_4);
	CheckBusy();
	SendByte(0xfa);              //11111,RW(0),RS(1),0
	SendByte(0xf0&Dbyte);        //高四位
	SendByte(0xf0&Dbyte<<4);   //低四位(先执行<<)
	//CS = 0;
	GPIO_ResetBits(GPIOA, GPIO_Pin_4);
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
	delay(10);
	Lcd_WriteCmd(0x30);        //选择基本指令集
	delay(1);
	Lcd_WriteCmd(0x30);          //选择8bit数据流
	delay(1);
	Lcd_WriteCmd(0x0c);          //开显示(无游标、不反白)
	delay(1);
	Lcd_WriteCmd(0x01);          //清除显示，并且设定地址指针为00H
	delay(10);
}

void hanzi_Disp(uchar x,uchar y,uchar *s)
{
	Lcd_WriteCmd(addr_tab[8*x+y]);  //写地址
	while(*s>0)
	{
		Lcd_WriteData(*s);    //写数据
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
	led_init();
	Lcd_Init();
	//
	// Loop forever echoing data through the UART.
	//
	while(1)
	{
		hanzi_Disp(0,0,"你好！世界");
		hanzi_Disp(1,0,"我是科比");
		GPIO_SetBits(GPIOC, GPIO_Pin_13);
		delay(10);
		GPIO_ResetBits(GPIOC, GPIO_Pin_13);
		delay(10);
	}
}
