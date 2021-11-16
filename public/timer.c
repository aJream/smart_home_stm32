#include "timer.h"
#include "system.h"
#include "wifi_function.h"
#include "wifi_config.h"

extern u8 lsens;  //光照强度
extern struct Dht11Data dhtData;  //温湿度	
extern u8 ledSta, beepSta;

//初始化TIM2
void TIM2_init(u16 arr, u16 psc){
	TIM_TimeBaseInitTypeDef TIM_Structure;
	NVIC_InitTypeDef NVIC_Structure;
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);
	
	//延时时间（单位:s）：arr/(72000000/psc)
	TIM_Structure.TIM_Period = arr - 1;  //装载值
	TIM_Structure.TIM_Prescaler = psc-1;  //分频系数
	TIM_Structure.TIM_ClockDivision = 0;
	TIM_Structure.TIM_CounterMode = TIM_CounterMode_Up;
	TIM_TimeBaseInit(TIM2, &TIM_Structure);
	
	TIM_ITConfig(TIM2, TIM_IT_Update, ENABLE); //开启定时器TIM2中断
	TIM_ClearITPendingBit(TIM2, TIM_IT_Update);
	NVIC_Structure.NVIC_IRQChannel = TIM2_IRQn;
	NVIC_Structure.NVIC_IRQChannelPreemptionPriority = 6; //抢占优先级
	NVIC_Structure.NVIC_IRQChannelSubPriority = 4;  //子优先级
	NVIC_Structure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_Structure);

	TIM_ClearFlag(TIM2, TIM_FLAG_Update);
	TIM_Cmd(TIM2, ENABLE);
}


void TIM2_IRQHandler(void){
	
	if(TIM_GetITStatus(TIM2, TIM_IT_Update) != RESET){

		ESP8266_Usart("{\"temp\":\"%d\",\"humi\":\"%d\",\"light\":\"%d\",\"ledsta\":\"%d\",\"beepsta\":\"%d\"}\r\n", dhtData.temp, dhtData.humi, lsens, ledSta, beepSta);//输出【光照、温度、湿度、led、beep状态】到esp8266，再发送给手机
		
		TIM_ClearITPendingBit(TIM2, TIM_IT_Update);
	}
}




