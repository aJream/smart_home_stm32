#ifndef __WIFI_CONFIG_H
#define	__WIFI_CONFIG_H

#include "system.h"


#if defined ( __CC_ARM   )
#pragma anon_unions
#endif


//* �������� */
typedef enum{
	STA,
	AP,
	STA_AP  
} ENUM_Net_ModeTypeDef;


typedef enum{
	 enumTCP,
	 enumUDP,
} ENUM_NetPro_TypeDef;
	

typedef enum{
	Multiple_ID_0 = 0,
	Multiple_ID_1 = 1,
	Multiple_ID_2 = 2,
	Multiple_ID_3 = 3,
	Multiple_ID_4 = 4,
	Single_ID_0 = 5,
} ENUM_ID_NO_TypeDef;
	

typedef enum{
	OPEN = 0,
	WEP = 1,
	WPA_PSK = 2,
	WPA2_PSK = 3,
	WPA_WPA2_PSK = 4,
} ENUM_AP_PsdMode_TypeDef;


#define RX_BUF_MAX_LEN     1024                                     //�����ջ����ֽ���
extern u16 USART2_RX_STA;   
extern char USART2_RX_BUF[RX_BUF_MAX_LEN]; 

extern struct  STRUCT_USARTx_Fram                                  //��������֡�Ĵ���ṹ��
{
	char  Data_RX_BUF[ RX_BUF_MAX_LEN ];
	
  union {
    __IO u16 InfAll;
    struct {
		  __IO u16 FramLength       :15;                               // 14:0 
		  __IO u16 FramFinishFlag   :1;                                // 15 
	  } InfBit;
  }; 
	
} strPc_Fram_Record, strEsp8266_Fram_Record;


struct WeatherData{
	char datetime[20];
	char city[10];
	char weather[10];  //ԭ��Ϊ10
	char temp[10];
	char humi[10];
	char windpower[10];  //�����ȼ�	
};

extern char minLsens_str[10], alarmTemp_str[10];


void WiFi_Config( void );
struct WeatherData processWdata(char* Data);
struct WeatherData processWdata2(char data[]);
struct WeatherData processWdata3(char data[]);
void USART2_printf( USART_TypeDef* USARTx, char *Data, ... );
#endif    /* __WIFI_CONFIG_H */
