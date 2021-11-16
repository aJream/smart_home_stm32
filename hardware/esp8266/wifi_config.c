#include "wifi_config.h"
#include "wifi_function.h"
#include <stdio.h>
#include "string.h"
#include <stdarg.h>
#include "cJSON.h"

#include "tftlcd.h"
#include "key.h"
#include "touch.h"



struct  STRUCT_USARTx_Fram strEsp8266_Fram_Record = { 0 };


/**
  * @brief  WiFi_Config wifi 初始化
  * @param  无
  * @retval 无
  */
void WiFi_Config( void )
{	
	GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
	RCC_APB2PeriphClockCmd( RCC_APB2Periph_GPIOA, ENABLE ); 
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO,ENABLE);
	GPIO_PinRemapConfig(GPIO_Remap_SWJ_JTAGDisable,ENABLE);
	
	/* 配置WiFi模块的片选（CH）引脚	复位重启（RST）引脚*/
	/*选择要控制的PA4（CH）引脚和PA15（RST）引脚*/															   
//	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4 | GPIO_Pin_15;	
//	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;   
//	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz; 
//	GPIO_Init( GPIOA, &GPIO_InitStructure );	 
//	/* 拉低WiFi模块的片选引脚	*/
//	GPIO_ResetBits( GPIOA, GPIO_Pin_4 );		
//	/* 拉高WiFi模块的复位重启引脚	*/
//	GPIO_SetBits( GPIOA, GPIO_Pin_15 );
	
		 		
  	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2; //PA2
  	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;	//复用推挽输出
  	GPIO_Init(GPIOA, &GPIO_InitStructure);
  	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3;//PA3
  	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;//浮空输入
  	GPIO_Init(GPIOA, &GPIO_InitStructure);//初始化GPIOB 11  
  
	

   	//USART2 初始化设置
	USART_InitStructure.USART_BaudRate = 115200;//串口波特率
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;//字长为8位数据格式
	USART_InitStructure.USART_StopBits = USART_StopBits_1;//一个停止位
	USART_InitStructure.USART_Parity = USART_Parity_No;//无奇偶校验位
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;//无硬件数据流控制
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;	//收发模式
  	USART_Init(USART2, &USART_InitStructure); //初始化串口2

	USART_ClearFlag(USART2, USART_FLAG_TC);
	
  	USART_ITConfig(USART2, USART_IT_RXNE, ENABLE);//开启串口接受和总线空闲中断
  	//USART_ITConfig(USART2, USART_IT_IDLE, ENABLE);
	
	
	
	USART_Cmd(USART2, ENABLE);                    //使能串口2 
	
	//USART2 NVIC 配置
  	NVIC_InitStructure.NVIC_IRQChannel = USART2_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=3 ;//抢占优先级3
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;		//子优先级3
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			//IRQ通道使能
	NVIC_Init(&NVIC_InitStructure);	//根据指定的参数初始化VIC寄存器
}


char USART2_RX_BUF[RX_BUF_MAX_LEN];

u16 USART2_RX_STA = 0;       



void USART2_IRQHandler(void){
	u8 r;
//	static struct WeatherData wd;
	
	if(USART_GetITStatus(USART2, USART_IT_RXNE) != RESET)  //接收中断
	{
		r = USART_ReceiveData(USART2);//(USART2->DR);	//读取接收到的数据
		//USART_SendData(USART1, r);
		
		if((USART2_RX_STA&0x8000)==0)//接收未完成
		{
			if(USART2_RX_STA&0x4000)//接收到了0x0d(即'\r')
			{
				if(r!=0x0a)USART2_RX_STA=0;//接收错误(即没有接收到'\n'),重新开始
				
				else {
					
					//USART2_RX_STA|=0x8000;	//接收完成了 (接收到'\r'后接收到'\n')
					USART2_RX_STA = 0;  //接收到\n, 重新等待下一次接收
					//PC_Usart("%s", USART2_RX_BUF);
					
				}
			}
			else //还没收到0X0D(即'\r')
			{	
				if(r==0x0d)USART2_RX_STA|=0x4000; //如果接收到\r, 让USART2_RX_STA的14位置1
				else
				{
					USART2_RX_BUF[USART2_RX_STA&0X3FFF]=r;
					USART2_RX_STA++;
					if(USART2_RX_STA>(RX_BUF_MAX_LEN-1))USART2_RX_STA=0;//接收数据错误,重新开始接收	  
				}		 
			}
		} 		
	} 

}

char minLsens_str[10]={0}, alarmTemp_str[10]={0};
//struct WeatherData{
//	char datetime[20]={0};
//	char city[10];
//	char weather[10];
//	char temp[10];
//	char humi[10];
//	char windpower[10];  //风力等级	
//};


struct WeatherData processWdata(char data[]){
    u8 n=0, i0=0, i=0;
    u8 slen = strlen(data);
    struct WeatherData weather;
	
	strcpy(weather.city, "西安");

    for(i=0; i<slen; i++){
        //printf("%c", *(data+i));
        if(*(data+i)==',') {
            n++;
            switch(n){
                case 1:strncpy(weather.datetime, (data+i0), i-i0);i0=i+1;break;
                case 2:strncpy(weather.weather, (data+i0), i-i0);i0=i+1;break;
                case 3:strncpy(weather.temp, (data+i0), i-i0);i0=i+1;break;
                case 4:strncpy(weather.humi, (data+i0), i-i0);i0=i+1;break;
				case 5:strncpy(weather.windpower, (data+i0), i-i0);i0=i+1;break;
				case 6:strncpy(alarmTemp_str, (data+i0), i-i0);i0=i+1;break;
				case 7:strncpy(minLsens_str, (data+i0), i-i0);i0=i+1;break;
            }

        }

    }

    return weather;

}

struct WeatherData processWdata3(char data[]){
    //char data[] = "2022-22-22 33:33:33,yin,47,48,<=7";
    u8 i=0, j=0, i0=0, k=0;
	u8 ind=0, jnd=0;
	
    int slen = strlen(data);
    struct WeatherData weather;
	
	for(ind=0; ind<8; ind++){
		switch(ind){
			case 0: {
				for(jnd=0; jnd<20; jnd++){
					weather.datetime[jnd]='\0';
				}
			};break;
			case 1: {
				for(jnd=0; jnd<10; jnd++){
					weather.city[jnd]='\0';
				}
			};break;
			case 2: {
				for(jnd=0; jnd<10; jnd++){
					weather.humi[jnd]='\0';
				}
			};break;
			
			case 3: {
				for(jnd=0; jnd<10; jnd++){
					weather.temp[jnd]='\0';
				}
			};break;
			case 4: {
				for(jnd=0; jnd<10; jnd++){
					weather.weather[jnd]='\0';
				}
			};break;
			case 5: {
				for(jnd=0; jnd<10; jnd++){
					weather.windpower[jnd]='\0';
				}
			};break;
			case 6: {
				for(jnd=0; jnd<10; jnd++){
					minLsens_str[jnd]='\0';
				}
			};break;
			case 7: {
				for(jnd=0; jnd<10; jnd++){
					alarmTemp_str[jnd]='\0';
				}
			};break;
			
		}
		
	
	}
	
	

	strcpy(weather.city, "西安");
    for(i=0; i<slen; i++){
        if(data[i]==',') {
            i0++;
            for(j=k; j<i; j++){

                if(i0==1) weather.datetime[j-k]=data[j];
                else if(i0==2) weather.weather[j-k]=data[j];
                else if(i0==3) weather.temp[j-k]=data[j];
                else if(i0==4) weather.humi[j-k]=data[j];
                else if(i0==5) weather.windpower[j-k]=data[j];
				else if(i0==6) alarmTemp_str[j-k]=data[j];
				else if(i0==7) minLsens_str[j-k]=data[j];
            }
            k=i+1;
        }

    }
    return weather;
}

struct WeatherData processWdata2(char data[]){
	struct WeatherData weather;
	int n=0;
    const char *temp = strtok(data,",");
    
	strcpy(weather.city, "西安");
    while(temp)
    {
		n++;
		switch(n){
			case 1: strcpy(weather.datetime, temp);
			case 2: strcpy(weather.weather, temp);
			case 3: strcpy(weather.temp, temp);
			case 4: strcpy(weather.humi, temp);
			case 5: strcpy(weather.windpower, temp);
			
		}
        temp = strtok(NULL,",");
    }
	return weather;
}


////{"datetime":"2021-10-29 09:09:36","city":"瑗垮畨","weather":"闆"temp":"12","air":"54"}
//struct WeatherData jsonDataProcess(char* jsonData){
//	struct WeatherData weatherData;
//	cJSON* root = NULL;
//	cJSON* datetime = NULL;
//	cJSON* weather = NULL;
//	cJSON* city = NULL;
//	cJSON* temp = NULL;
//	cJSON* air = NULL;
//	
//	root = cJSON_Parse(jsonData);
//	
//	datetime = cJSON_GetObjectItem(root, "datetime");
//	memcpy(weatherData.datetime, datetime->valuestring, strlen(datetime->valuestring));
//	weather = cJSON_GetObjectItem(root, "weather");
//	memcpy(weatherData.weather, weather->valuestring, strlen(weather->valuestring));
//	city = cJSON_GetObjectItem(root, "city");
//	memcpy(weatherData.city, city->valuestring, strlen(city->valuestring));
//	temp = cJSON_GetObjectItem(root, "temp");
//	memcpy(weatherData.temp, temp->valuestring, strlen(temp->valuestring));
//	air = cJSON_GetObjectItem(root, "air");
//	memcpy(weatherData.air, air->valuestring, strlen(air->valuestring));

//	cJSON_Delete(root);
//	return weatherData;
//	
//}

/*
 * 函数名：itoa
 * 描述  ：将整形数据转换成字符串
 * 输入  ：-radix =10 表示10进制，其他结果为0
 *         -value 要转换的整形数
 *         -buf 转换后的字符串
 *         -radix = 10
 * 输出  ：无
 * 返回  ：无
 * 调用  ：被USART2_printf()调用
 */
static char *itoa( int value, char *string, int radix )
{
    int     i, d;
    int     flag = 0;
    char    *ptr = string;

    /* This implementation only works for decimal numbers. */
    if (radix != 10)
    {
        *ptr = 0;
        return string;
    }

    if (!value)
    {
        *ptr++ = 0x30;
        *ptr = 0;
        return string;
    }

    /* if this is a negative value insert the minus sign. */
    if (value < 0)
    {
        *ptr++ = '-';

        /* Make the value positive. */
        value *= -1;
    }

    for (i = 10000; i > 0; i /= 10)
    {
        d = value / i;

        if (d || flag)
        {
            *ptr++ = (char)(d + 0x30);
            value -= (d * i);
            flag = 1;
        }
    }

    /* Null terminate the string. */
    *ptr = 0;

    return string;

} /* NCL_Itoa */


/*
 * 函数名：USART2_printf
 * 描述  ：格式化输出，类似于C库中的printf，但这里没有用到C库
 * 输入  ：-USARTx 串口通道，这里只用到了串口2，即USART2
 *		     -Data   要发送到串口的内容的指针
 *			   -...    其他参数
 * 输出  ：无
 * 返回  ：无 
 * 调用  ：外部调用
 *         典型应用USART2_printf( USART2, "\r\n this is a demo \r\n" );
 *            		 USART2_printf( USART2, "\r\n %d \r\n", i );
 *            		 USART2_printf( USART2, "\r\n %s \r\n", j );
 */
void USART2_printf( USART_TypeDef* USARTx, char *Data, ... )
{
	const char *s;
	int d;   
	char buf[16];

	va_list ap;
	va_start(ap, Data);

	while ( *Data != 0)     // 判断是否到达字符串结束符
	{				                          
		if ( *Data == 0x5c )  //'\'
		{									  
			switch ( *++Data )
			{
				case 'r':							          //回车符
					USART_SendData(USARTx, 0x0d);
					Data ++;
					break;

				case 'n':							          //换行符
					USART_SendData(USARTx, 0x0a);	
					Data ++;
					break;
				
				default:
					Data ++;
				    break;
			}			 
		}
		else if ( *Data == '%')
		{									  //
			switch ( *++Data )
			{				
				case 's':										  //字符串
					s = va_arg(ap, const char *);
          for ( ; *s; s++) 
					{
						USART_SendData(USARTx,*s);
						while( USART_GetFlagStatus(USARTx, USART_FLAG_TXE) == RESET );
          }
					Data++;
          break;

        case 'd':										//十进制
          d = va_arg(ap, int);
          itoa(d, buf, 10);
          for (s = buf; *s; s++) 
					{
						USART_SendData(USARTx,*s);
						while( USART_GetFlagStatus(USARTx, USART_FLAG_TXE) == RESET );
          }
					Data++;
          break;
				 default:
						Data++;
				    break;
			}		 
		} /* end of else if */
		else USART_SendData(USARTx, *Data++);
		while( USART_GetFlagStatus(USARTx, USART_FLAG_TXE) == RESET );
	}
}


