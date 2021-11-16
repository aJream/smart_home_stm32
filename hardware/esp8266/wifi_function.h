#ifndef __WIFI_FUNCTION_H
#define	__WIFI_FUNCTION_H 


#include "system.h"
#include "wifi_config.h"
#include <stdbool.h>


#define     ESP8266_Usart( fmt, ... )           USART2_printf (USART2, fmt, ##__VA_ARGS__ ) 
#define     PC_Usart( fmt, ... )                printf ( fmt, ##__VA_ARGS__ )

#define     ESP8266_CH_HIGH_LEVEL()             GPIO_SetBits( GPIOA, GPIO_Pin_4 )
#define     ESP8266_CH_LOW_LEVEL()              GPIO_ResetBits( GPIOA, GPIO_Pin_4 )

#define     ESP8266_RST_HIGH_LEVEL()            GPIO_SetBits( GPIOA, GPIO_Pin_15 )
#define     ESP8266_RST_LOW_LEVEL()             GPIO_ResetBits( GPIOA, GPIO_Pin_15 )

void send2Esp8266(char *data);

char* getDataFromEsp8266(void);

// bool        ESP8266_Cmd                         ( char * cmd, char * reply1, char * reply2, u32 waittime );
// bool        ESP8266_SendString                  ( FunctionalState enumEnUnvarnishTx, char * pStr, u32 ulStrLength, ENUM_ID_NO_TypeDef ucId );
// char *      ESP8266_ReceiveString               ( FunctionalState enumEnUnvarnishTx );


#endif    /* __WIFI_FUNCTION_H */





