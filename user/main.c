#include "stdio.h"
#include "string.h"
#include "math.h"

#include "system.h"
#include "SysTick.h"
#include "led.h"
#include "timer.h"
#include "usart.h"

#include "beep.h"
#include "dht11.h"
#include "lsens.h"

#include "tftlcd.h"
#include "img.h"

#include "wifi_config.h"
#include "wifi_function.h"


u8 lsens;  //����ǿ��
struct Dht11Data dhtData1, dhtData2, dhtData;  //��ʪ��	
u8 minLsens=12, alarmTemp=40;

void publicInit(void);
void hardwareInit(void);

//DHT11��ȡ��ʪ��
struct Dht11Data getDht11Data(void);
u8 setAlarmTemp(u8 );
_Bool isTempValid(u8 temp0, u8 temp1);

//����ǿ�Ȼ�ȡ
u8 getLsens(void);
u8 setMinLsens(u8 minLsens);  //��͹�ǿ

void ledBeepOnOrNot(u8 lsens, u8 mlsens, u8 t, u8 alarmT);  //�Ƿ�����LED��BEEP
u8 ledSta=0, beepSta=0;  // 1Ϊ����0Ϊ��


//����ʱ�䡢������ȡ�����ؽṹ��
struct WeatherData getWeatherData(void);
struct WeatherData getWeatherData3(void);
u8 str2int(char s[]);   //�ַ���ת����


/*

void LCD_ShowNum(u16 x,u16 y,u32 num,u8 len,u8 size);//��ʾһ������
void LCD_ShowxNum(u16 x,u16 y,u32 num,u8 len,u8 size,u8 mode);//��ʾ����
void LCD_ShowString(u16 x,u16 y,u16 width,u16 height,u8 size,u8 *p);//��ʾ�ַ���
void LCD_ShowFontHZ(u16 x, u16 y, u8 *cn);//��ʾ����					   						   																			
void LCD_ShowPicture(u16 x, u16 y, u16 wide, u16 high,u8 *pic);//��ʾͼƬ
*/

int main()
{
	
	
	struct WeatherData wd;  //����
//	char** wd;
	
	publicInit();
	hardwareInit();

	LCD_DrawRectangle(10, 10, 310, 470);//������
	LCD_ShowFontHZ(LCD_HCENTER - HZSIZE,15,"����",BLUE);
	LCD_ShowFontHZ(20,15+HZSIZE,"����",BLACK);
	LCD_ShowString(20+2*HZSIZE+14*3, 15+HZSIZE+3, 24, 12, 24, "%");
	//LCD_ShowFontHZ(20+2*HZSIZE+14*3+2*HZSIZE,15+HZSIZE, "���",BLACK);
	
	
	LCD_ShowFontHZ(20, 15+2*HZSIZE, "�¶�",BLACK);
	LCD_ShowFontHZ(20+2*HZSIZE+14*3, 15+2*HZSIZE, "��",BLACK);
	
	LCD_ShowFontHZ(20, 15+3*HZSIZE, "ʪ��",BLACK);
	LCD_ShowString(20+2*HZSIZE+14*3, 15+3*HZSIZE+3, tftlcd_data.width, tftlcd_data.height, 24, "%RH");
	
//	LCD_ShowFontHZ(20, 15+3*HZSIZE, "�����¶�",RED);
//	LCD_ShowFontHZ(20+4*HZSIZE+14*3, 15+3*HZSIZE, "��͹���",GREEN);
	
	
	LCD_DrawLine_Color(10, 15+4*HZSIZE+10, 310, 15+4*HZSIZE+10, FRONT_COLOR);//ָ����ɫ����
	
	
	LCD_ShowFontHZ(LCD_HCENTER - 2 * HZSIZE-10, 15+4*HZSIZE+20,"����",BLUE);
	
	LCD_ShowFontHZ(LCD_HCENTER +10, 15+4*HZSIZE+20,"����",BLUE);
	
	//LCD_ShowPicture(20,15+4*HZSIZE, 80,80, imgQing);
	LCD_ShowFontHZ(20,15+6*HZSIZE+20+100,"ʱ��",BLACK);
	LCD_ShowFontHZ(20,15+7*HZSIZE+20+100,"����",BLACK);
	LCD_ShowFontHZ(20,15+8*HZSIZE+20+100,"ʪ��",BLACK);
	LCD_ShowFontHZ(20,15+9*HZSIZE+20+100,"����",BLACK);
	LCD_ShowFontHZ(20+2*HZSIZE+4*14,15+9*HZSIZE+20+100,"��",BLACK);
	//LCD_ShowPicture(20+4*HZSIZE, 15+5*HZSIZE+25, 80,80, (u8 *)gImage_qing);
	
	
	
	while(1)
	{
		if(str2int(minLsens_str)!=0)
			minLsens = setMinLsens(str2int(minLsens_str));
		else
			minLsens = setMinLsens(12);  //Ԥ��ֵ
		
		if(str2int(alarmTemp_str)!=0)
			alarmTemp = setMinLsens(str2int(alarmTemp_str));
		else
			alarmTemp = setMinLsens(40);  //Ԥ��ֵ
		
		lsens = getLsens();
		
		LCD_ShowNum(20+2*HZSIZE, 15+HZSIZE+3, lsens, 3, 24);
		FRONT_COLOR=GRAYBLUE;
		LCD_ShowNum(20+2*HZSIZE+14*3+15*4, 15+HZSIZE+6, minLsens, 3, 16);
		FRONT_COLOR=BLACK;

		dhtData1 = getDht11Data();
		delay_ms(100);
		dhtData2 = getDht11Data();
		if(isTempValid(dhtData1.temp, dhtData2.temp)){
			dhtData.temp = dhtData1.temp;
			dhtData.humi=dhtData1.humi;
		}
		//dhtData.humi=dhtData1.humi;
		LCD_ShowNum(20+2*HZSIZE, 15+2*HZSIZE+3, dhtData.temp, 3, 24);
		LCD_ShowNum(20+2*HZSIZE, 15+3*HZSIZE+3, dhtData.humi, 3, 24);
		FRONT_COLOR=RED;
		LCD_ShowNum(20+2*HZSIZE+14*3+15*4, 15+2*HZSIZE+6, alarmTemp, 3, 16);
		FRONT_COLOR=BLACK;
		
		ledBeepOnOrNot(lsens, minLsens, dhtData.temp, alarmTemp);
		

		wd = getWeatherData3();
		LCD_ShowFontHZ(20+2*HZSIZE,15+4*HZSIZE+20+100+3,(u8*)wd.weather,BLACK);
		//LCD_ShowString(20+2*HZSIZE, 15+4*HZSIZE+20+100+3, tftlcd_data.width, tftlcd_data.height, 24, (u8* )wd.weather);  //����
		LCD_ShowString(20+2*HZSIZE, 15+6*HZSIZE+20+100+8, 14*20, 16, 16, (u8* )wd.datetime);  //ʱ��
		LCD_ShowString(20+2*HZSIZE, 15+7*HZSIZE+20+100+3, 14*5, tftlcd_data.height, 24, (u8* )wd.temp);  //�¶�
		LCD_ShowString(20+2*HZSIZE, 15+8*HZSIZE+20+100+3, 14*5, tftlcd_data.height, 24, (u8* )wd.humi);  //ʪ��
		LCD_ShowString(20+2*HZSIZE, 15+9*HZSIZE+20+100+3, 14*5, tftlcd_data.height, 24, (u8* )wd.windpower);  //����
		
		//LCD_ShowString(110, 205, tftlcd_data.width, tftlcd_data.height, 16, (u8* )wd.windpower);  //ʱ��
		//LCD_ShowFontHZ(100,140,"����");
		//LCD_ShowFontHZ(100,200,(u8*)wd.weather);
		

		//delay_ms(300);
	}
}

void ledBeepOnOrNot(u8 lsens, u8 mlsens, u8 t, u8 alarmT){  //�Ƿ�����LED��BEEP
	if(lsens < mlsens){
		LED1 = 0;  //����
		ledSta = 1;  
	}
		
	else{
		LED1 = 1;  //Ϩ��
		ledSta = 0;
	}
		
	if(t>alarmT){
		BEEP=1;         //��
		beepSta = 1;
	}
	else{
		BEEP=0;			//����
		beepSta = 0;
	}
	
}

u8 setMinLsens(u8 minLsens){
	
	return minLsens;
}


u8 setAlarmTemp(u8 alarmTemp){

	return alarmTemp;
}

u8 str2int(char s[]){
	u8 n=0, i=0, len, sum=0;
	len = strlen(s);
	//PC_Usart("s: %s len: %d", s, len);
	for(i=0; i<len; i++){
		if(47<s[i]<58){
			n = s[i] - 48;   //�õ�һλ����
			sum += (n * pow(10, len-1-i));
		}
	}
	return sum;	
}

void publicInit(void){
	
	SysTick_Init(72);
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);  //�ж����ȼ����� ��2��
	LED_Init();
	USART1_Init(115200);
	TIM2_init(1000, 36000);  //1s����һ���ж�
}


void hardwareInit(void){

	DHT11_Init();
	Lsens_Init();
	BEEP_Init();
	
	TFTLCD_Init();		//LCD��ʼ��
//	KEY_Init();
//	TP_Init();
	LCD_Clear(WHITE);
	FRONT_COLOR=FONTCOLOR;
	WiFi_Config();     //wifi�ӿ����ó�ʼ��

}


struct Dht11Data getDht11Data(){	//��ȡDHT11�ɼ�����ʪ��
	struct Dht11Data tempHumi;
	u8 temp;  	    
	u8 humi;
	
	DHT11_Read_Data(&temp,&humi);
	tempHumi.humi = humi;  		//%RH
	tempHumi.temp = temp;  		//��C
	return tempHumi;
}

_Bool isTempValid(u8 temp0, u8 temp1){
	return ((temp0-temp1<=2) && (temp0-temp1>-2));
}

u8 getLsens(){				//����ǿ��
	return Lsens_Get_Val();
}


//����ʱ�䡢������ȡ�����ؽṹ��
//void getWeatherData2(){
//	
//	char s[100]={0};
//	char *temp;
//	int n=0;
//	//strcpy(*, "����");
//	strcpy(s,USART2_RX_BUF);
//	PC_Usart("data----%s", s);
//    
//	temp = strtok(s,",");
//	
//	
//	
//    while(temp && n<=5)
//    {
//		n++;
//		switch(n){
//			case 1:strcpy(ndtime, temp);
//			case 2:strcpy(nwea, temp);
//			case 3:strcpy(ntemp, temp);
//			case 4:strcpy(nhumi, temp);
//			case 5:strcpy(nwind, temp);
//			
//		}
//		printf("n--->%d\r\n", n);
//        temp = strtok(NULL,",");
//    }

//}
	
	
	
struct WeatherData getWeatherData3(){
	struct WeatherData wd;
	char data[200]={0};
	char* hztq[52] = {"��","����","����","��","�з�","ƽ��","΢��","�ͷ�","���","����","���","�ҷ�","�籩","�񱬷�","쫷�","��","�ж���","�ض���","������","����","������","С��","����","����","����","����","�ش���","��","С��-����","����-����","����-����","��ѩ����","���ѩ","�����ѩ","����","ѩ","��ѩ","Сѩ","��ѩ","��ѩ","��ѩ","����","��ɳ","ɳ����","��","Ũ��","ǿŨ��","����","����","��","��","δ֪"};
	char* pytq[52] = {"qing","shaoyun","duoyun","yin","youfeng","pingjing","weifeng","hefeng","qingfeng","jifeng","dafeng","liefeng","fengbao","kuangbaofeng","jufeng","mai","zhongdumai","zhongdumai","yanzhongmai","zhenyu","leizhenyu","xiaoyu","zhongyu","dayu","baoyu","dabaoyu","tedabaoyu","yu","xiaoyu-zhongyu","zhongyu-dayu","dayu-baoyu","yuxuetianqi","yujiaxue","zhenyujiaxue","dongyu","xue","zhenxue","xiaoxue","zhongxue","daxue","baoxue","fuchen","yangsha","shachenbao","wu","nongwu","qiangnongwu","qingwu","dawu","re","leng","weizhi"};
	u8 i = 0;

	strcpy(data, USART2_RX_BUF);

	PC_Usart("%s\r\n datalen-->%d\r\n", data, strlen(data));
	wd = processWdata3(data);
	PC_Usart("1--%s 2--%s 3--%s 4--%s 5--%s 6--%s 7-alarmtemp--%s 8-minlsens--%s\r\n", wd.city, wd.datetime, wd.weather, wd.temp, wd.humi, wd.windpower, alarmTemp_str, minLsens_str);
	
	for(i=0;i<52; i++){
		if(wd.weather[0]!=0 && strcmp(wd.weather, pytq[i])==0){
//strcmp(wd.weather, pytq[0])==0
			if(strstr(wd.weather, "qing")){
				LCD_ShowPicture(20+4*HZSIZE, 15+5*HZSIZE+25, 80,80, (u8 *)gImage_qing);
			}
			else if(strcmp(wd.weather, pytq[2])==0 || strcmp(wd.weather, pytq[1])==0 ){
				LCD_ShowPicture(20+4*HZSIZE, 15+5*HZSIZE+25, 80,80, (u8 *)gImage_yun);
			}
			else if(strcmp(wd.weather, pytq[3])==0){
				LCD_ShowPicture(20+4*HZSIZE, 15+5*HZSIZE+25, 80,80, (u8 *)gImage_yin);
			}
			
			else if(strstr(wd.weather, "lei")!=NULL){
				LCD_ShowPicture(20+4*HZSIZE, 15+5*HZSIZE+25, 80,80, (u8 *)gImage_lei);
			}
			
			else if(strstr(wd.weather, "shachen")!=NULL){
				LCD_ShowPicture(20+4*HZSIZE, 15+5*HZSIZE+25, 80,80, (u8 *)gImage_shachen);
			}
			
			else if(strstr(wd.weather, "wu")!=NULL){
				LCD_ShowPicture(20+4*HZSIZE, 15+5*HZSIZE+25, 80,80, (u8 *)gImage_wu);
			}
			else if(strstr(wd.weather, "xue")!=NULL){
				LCD_ShowPicture(20+4*HZSIZE, 15+5*HZSIZE+25, 80,80, (u8 *)gImage_xue);
			}
			else if(strstr(wd.weather, "yu")!=NULL){
				LCD_ShowPicture(20+4*HZSIZE, 15+5*HZSIZE+25, 80,80, (u8 *)gImage_yu);
			}
			else if(strstr(wd.weather, "yujia")!=NULL){
				LCD_ShowPicture(20+4*HZSIZE, 15+5*HZSIZE+25, 80,80, (u8 *)gImage_yujiaxue);
			}
			else if(strstr(wd.weather, "zhenyu")!=NULL){
				LCD_ShowPicture(20+4*HZSIZE, 15+5*HZSIZE+25, 80,80, (u8 *)gImage_zhenyu);
			}
			strcpy(wd.weather, hztq[i]);          //ƴ��ת����
			break;
		}
	}
	PC_Usart("1--%s 2--%s 3--%s 4--%s 5--%s 6--%s 7--%s 8--%s\r\n", wd.city, wd.datetime, wd.weather, wd.temp, wd.humi, wd.windpower, alarmTemp_str, minLsens_str);

	return wd;
}


struct WeatherData getWeatherData(){
	struct WeatherData wd;
	char data[200]={0};
//	char* hztq[52] = {"��","����","����","��","�з�","ƽ��","΢��","�ͷ�","���","����","���","�ҷ�","�籩","�񱬷�","쫷�","��","�ж���","�ض���","������","����","������","С��","����","����","����","����","�ش���","��","С��-����","����-����","����-����","��ѩ����","���ѩ","�����ѩ","����","ѩ","��ѩ","Сѩ","��ѩ","��ѩ","��ѩ","����","��ɳ","ɳ����","��","Ũ��","ǿŨ��","����","����","��","��","δ֪"};
//	char* pytq[52] = {"qing","shaoyun","duoyun","yin","youfeng","pingjing","weifeng","hefeng","qingfeng","jifeng","dafeng","liefeng","fengbao","kuangbaofeng","jufeng","mai","zhongdumai","zhongdumai","yanzhongmai","zhenyu","leizhenyu","xiaoyu","zhongyu","dayu","baoyu","dabaoyu","tedabaoyu","yu","xiaoyu-zhongyu","zhongyu-dayu","dayu-baoyu","yuxuetianqi","yujiaxue","zhenyujiaxue","dongyu","xue","zhenxue","xiaoxue","zhongxue","daxue","baoxue","fuchen","yangsha","shachenbao","wu","nongwu","qiangnongwu","qingwu","dawu","re","leng","weizhi"};
//	u8 i = 0;
//	char w[20];

	strcpy(data, USART2_RX_BUF);

	PC_Usart("%s\r\n datalen-->%d\r\n", USART2_RX_BUF, strlen(USART2_RX_BUF));
	wd = processWdata(USART2_RX_BUF);
	PC_Usart("1--%s 2--%s 3--%s 4--%s 5--%s 6--%s 7--%s 8--%s\r\n", wd.city, wd.datetime, wd.weather, wd.temp, wd.humi, wd.windpower, alarmTemp_str, minLsens_str);

//	for(i=0; i<strlen(data);++i){
//		PC_Usart("%c ", data[i]);
//		
//	}
	
	
//	for(i=0;i<52; i++){
//		if(wd.weather[0]!=0 && strcmp(wd.weather, pytq[i])==0){
//			PC_Usart("true--%d\r\n", i);
//			strcpy(wd.weather, hztq[i]);          //ƴ��ת����
//			break;
//		}
//		else{
//			PC_Usart("false--%d\r\n",i);
//		}
//	}
	
	return wd;
	
}


