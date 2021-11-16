#include "wifi_function.h"
#include "wifi_config.h"
#include "SysTick.h"
#include <string.h>
#include <stdio.h>
#include <stdbool.h>




void send2Esp8266(char data[]){
	char cStr[1000];

	sprintf(cStr, "%s", data);

	ESP8266_Usart("%s\r\n", cStr);
}

char* getDataFromEsp8266(void){
	
	return USART2_RX_BUF;
}


// bool ESP8266_Cmd(char *cmd, char *reply1, char *reply2, u32 waittime)
// {
// 	strEsp8266_Fram_Record.InfBit.FramLength = 0; //从新开始接收新的数据包

// 	ESP8266_Usart("%s\r\n", cmd);

// 	delay_ms(waittime); //延时

// 	strEsp8266_Fram_Record.Data_RX_BUF[strEsp8266_Fram_Record.InfBit.FramLength] = '\0';

// 	PC_Usart("%s", strEsp8266_Fram_Record.Data_RX_BUF);

// 	if ((reply1 == 0) && (reply2 == 0)) //不需要接收数据
// 		return true;

// 	if ((reply1 != 0) && (reply2 != 0))
// 		return ((bool)strstr(strEsp8266_Fram_Record.Data_RX_BUF, reply1) ||
// 				(bool)strstr(strEsp8266_Fram_Record.Data_RX_BUF, reply2));

// 	else if (reply1 != 0)
// 		return ((bool)strstr(strEsp8266_Fram_Record.Data_RX_BUF, reply1));

// 	else
// 		return ((bool)strstr(strEsp8266_Fram_Record.Data_RX_BUF, reply2));
// }

// //发送命令返回数据
// char * ESP8266_CmdGetString(char *cmd, u32 waittime){
// 	strEsp8266_Fram_Record.InfBit.FramLength = 0; //从新开始接收新的数据包

// 	ESP8266_Usart("%s\r\n", cmd);


// 	delay_ms(waittime); //延时

// 	strEsp8266_Fram_Record.Data_RX_BUF[strEsp8266_Fram_Record.InfBit.FramLength] = '\0';

// 	//PC_Usart("%s", strEsp8266_Fram_Record.Data_RX_BUF);
	
// 	return strEsp8266_Fram_Record.Data_RX_BUF;
// }




/* 
 * 输入  ：enumEnUnvarnishTx，声明是否已使能了透传模式
 *       ：pStr，要发送的字符串
 *       ：ulStrLength，要发送的字符串的字节数
 *       ：ucId，哪个ID发送的字符串
 */
// bool ESP8266_SendString(FunctionalState enumEnUnvarnishTx, char *pStr, u32 ulStrLength, ENUM_ID_NO_TypeDef ucId)
// {
// 	char cStr[20];
// 	bool bRet = false;

// 	if (enumEnUnvarnishTx)
// 		ESP8266_Usart("%s", pStr);

// 	else
// 	{
// 		if (ucId < 5)
// 			sprintf(cStr, "AT+CIPSEND=%d,%d", ucId, ulStrLength + 2);

// 		else
// 			sprintf(cStr, "AT+CIPSEND=%d", ulStrLength + 2);

// 		ESP8266_Cmd(cStr, "> ", 0, 1000);

// 		bRet = ESP8266_Cmd(pStr, "SEND OK", 0, 1000);
// 	}

// 	return bRet;
// }

/*
 * 输入  ：enumEnUnvarnishTx，声明是否已使能了透传模式
 * 返回  : 接收到的字符串首地址
 * 调用  ：被外部调用
 */
// char *ESP8266_ReceiveString(FunctionalState enumEnUnvarnishTx)
// {
// 	char *pRecStr = 0;

// 	strEsp8266_Fram_Record.InfBit.FramLength = 0;
// 	strEsp8266_Fram_Record.InfBit.FramFinishFlag = 0;
// 	while (!strEsp8266_Fram_Record.InfBit.FramFinishFlag);
	
// 	strEsp8266_Fram_Record.Data_RX_BUF[strEsp8266_Fram_Record.InfBit.FramLength] = '\0';

// 	if (enumEnUnvarnishTx)
// 	{
// 		if (strstr(strEsp8266_Fram_Record.Data_RX_BUF, ">"))
// 			pRecStr = strEsp8266_Fram_Record.Data_RX_BUF;
// 	}

// 	else
// 	{
// 		if (strstr(strEsp8266_Fram_Record.Data_RX_BUF, "+IPD"))
// 			pRecStr = strEsp8266_Fram_Record.Data_RX_BUF;
// 	}

// 	return pRecStr;
// }
