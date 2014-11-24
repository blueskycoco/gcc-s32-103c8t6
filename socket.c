#include "socket.h"
unsigned char config_local_ip[11]		={0xF5,0x8A,0x00,0xff,0xff,0xff,0xff,0x26,0xfa,0x00,0x00};/*local ip*/
unsigned char config_local_port[9]		={0xF5,0x8A,0x01,0xff,0xff,0x26,0xfa,0x00,0x00};/*local port*/
unsigned char config_sub_msk[11]		={0xF5,0x8A,0x02,0xff,0xff,0xff,0xff,0x26,0xfa,0x00,0x00};/*sub msk*/
unsigned char config_gw[11]				={0xF5,0x8A,0x03,0xff,0xff,0xff,0xff,0x26,0xfa,0x00,0x00};/*gw*/
unsigned char config_mac[13]			={0xF5,0x8A,0x04,0xff,0xff,0xff,0xff,0xff,0xff,0x26,0xfa,0x00,0x00};/*mac*/
unsigned char config_socket0_ip[11]		={0xF5,0x8A,0x05,0xff,0xff,0xff,0xff,0x26,0xfa,0x00,0x00};/*socket 0 ip*/
unsigned char config_socket1_ip[11]		={0xF5,0x8A,0x06,0xff,0xff,0xff,0xff,0x26,0xfa,0x00,0x00};/*socket 1 ip*/
unsigned char config_socket2_ip[11]		={0xF5,0x8A,0x07,0xff,0xff,0xff,0xff,0x26,0xfa,0x00,0x00};/*socket 2 ip*/
unsigned char config_socket3_ip[11]		={0xF5,0x8A,0x08,0xff,0xff,0xff,0xff,0x26,0xfa,0x00,0x00};/*socket 3 ip*/
unsigned char config_socket0_port[9]	={0xF5,0x8A,0x09,0xff,0xff,0x26,0xfa,0x00,0x00};/*socket 0 port*/
unsigned char config_socket1_port[9]	={0xF5,0x8A,0x0a,0xff,0xff,0x26,0xfa,0x00,0x00};/*socket 1 port*/
unsigned char config_socket2_port[9]	={0xF5,0x8A,0x0b,0xff,0xff,0x26,0xfa,0x00,0x00};/*socket 2 port*/
unsigned char config_socket3_port[9]	={0xF5,0x8A,0x0c,0xff,0xff,0x26,0xfa,0x00,0x00};/*socket 3 port*/
unsigned char config_net_protol[11]		={0xF5,0x8A,0x0d,0xff,0xff,0xff,0xff,0x26,0xfa,0x00,0x00};/*protol*/
unsigned char config_socket_mode[11]	={0xF5,0x8A,0x0e,0xff,0xff,0xff,0xff,0x26,0xfa,0x00,0x00};/*server mode*/
unsigned char config_uart_baud[11]		={0xF5,0x8A,0x0f,0xff,0xff,0xff,0xff,0x26,0xfa,0x00,0x00};/*uart baud*/
unsigned char get_config[62];//			={0xF5,0x8B,0x0f,0xff,0xff,0xff,0xff,0x27,0xfa,0x00,0x00};/*get config 0xf5,0x8b,********0x27,0xfa,0x00,0x00*/
void ind_cnn_init()
{
    GPIO_InitTypeDef  GPIO_InitStructure;
    /* Enable the GPIO_LED Clock */
    RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOA, ENABLE);
    /* Configure the GPIO_LED pin */
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;
    GPIO_Init(GPIOA, &GPIO_InitStructure);
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1;
    GPIO_Init(GPIOA, &GPIO_InitStructure);
}
void ind_out(int level)
{
	if(level)
		GPIO_SetBits(GPIOA,GPIO_Pin_0);
	else
		GPIO_ResetBits(GPIOA,GPIO_Pin_0);
}
int cnn_in()
{
	return (GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_1)==SET);
}
int config_param(int type,pconfig conf)
{
	int crc=0,len,i;
	unsigned char *ptr;
	switch(type)
	{
		case CONFIG_LOCAL_IP:
			{
				len=11;
				ptr=config_local_ip;
				for(i=0;i<4;i++)
					ptr[3+i]=conf->local_ip[i];
			}
			break;
		case CONFIG_LOCAL_PORT:
			{
				len=9;
				ptr=config_local_port;
				for(i=0;i<2;i++)
					ptr[3+i]=conf->local_port[i];
			}
			break;
		case CONFIG_SUB_MSK:
			{
				len=11;
				ptr=config_sub_msk;
				for(i=0;i<4;i++)
					ptr[3+i]=conf->sub_msk[i];
			}
			break;
		case CONFIG_GW:
			{
				len=11;
				ptr=config_gw;
				for(i=0;i<4;i++)
					ptr[3+i]=conf->gw[i];
			}
			break;
		case CONFIG_MAC:
			{
				len=13;
				ptr=config_mac;
				for(i=0;i<6;i++)
					ptr[3+i]=conf->mac[i];
			}
			break;
		case CONFIG_REMOTE_IP:
			{
				len=11;
				ptr=config_socket0_ip;
				for(i=0;i<4;i++)
					ptr[3+i]=conf->remote_ip[i];
			}
			break;
		case CONFIG_REMOTE_PORT:
			{
				len=9;
				ptr=config_socket0_port;
				for(i=0;i<2;i++)
					ptr[3+i]=conf->remote_port[i];
			}
			break;
		case CONFIG_PROTOL:
			{
				len=11;
				ptr=config_net_protol;
				ptr[3+0]=conf->protol;
			}
			break;
		case CONFIG_SERVER_MODE:
			{
				len=11;
				ptr=config_socket_mode;
				ptr[3+0]=conf->server_mode;
			}
			break;
		case CONFIG_UART_BAUD:
			{
				len=11;
				ptr=config_uart_baud;
				ptr[3+0]=conf->uart_baud;
			}
			break;
		default:
			return 0;
	}
	for(i=0;i<len;i++)
		crc=crc+ptr[i];
	ptr[len-2]=(crc>>8)&0xff;
	ptr[len-1]=crc&0xff;
	for(i=0;i<len;i++)
		uart_send(ptr[i]);
}
