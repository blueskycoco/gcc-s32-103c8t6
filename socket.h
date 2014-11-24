#include <stm32f10x.h>
typedef struct {
	unsigned char local_ip[4];
	unsigned char local_port[2];
	unsigned char sub_msk[4];
	unsigned char gw[4];
	unsigned char mac[6];
	unsigned char remote_ip[4];
	unsigned char remote_port[2];
	unsigned char protol;
	unsigned char server_mode;
	unsigned char uart_baud;
}config,*pconfig;
#define CONFIG_LOCAL_IP 	0
#define CONFIG_LOCAL_PORT 	1
#define CONFIG_SUB_MSK 		2
#define CONFIG_GW 			3
#define CONFIG_MAC 			4
#define CONFIG_REMOTE_IP 	5
#define CONFIG_REMOTE_PORT 	6
#define CONFIG_PROTOL 		7
#define CONFIG_SERVER_MODE 	8
#define CONFIG_UART_BAUD 	9
#define NET_PROTOL_TCP		0
#define NET_PROTOL_UDP		1
#define SERVER_MODE			0
#define CLIENT_MODE			1
#define BAUD_115200			0
#define BAUD_460800			1
#define BAUD_921600			2
#define BAUD_1000000		3
#define BAUD_2000000		4
#define BAUD_4000000		5
#define BAUD_6000000		6
#define IND_CONFIG			1
#define IND_SOCKET			0
#define CNN_CONNECT			1
#define CNN_DISCONNECT		0
int uart_send(unsigned char byte);
int config_param(int type,pconfig conf);
void ind_cnn_init();
int cnn_in();
void ind_out(int level);
