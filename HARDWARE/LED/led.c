#include "led.h"

/*
LED0:PA8
LED1:PD2
推挽输出
50MHz
初始化高
*/
void LED_Init(void)
{
    //库函数初始化
    GPIO_InitTypeDef  GPIO_InitStructure; 	
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | RCC_APB2Periph_GPIOD, ENABLE);	 //使能PA,PD端口时钟
        
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8;				 //LED0-->PA8 端口配置
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 		 //推挽输出
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;		 //IO口速度为50MHz
        
    GPIO_Init(GPIOA, &GPIO_InitStructure);					 //根据设定参数初始化GPIOA8
    GPIO_SetBits(GPIOA,GPIO_Pin_8);						     //PA8 输出高

    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2;	    		 //LED1-->PD2 端口配置, 推挽输出
    GPIO_Init(GPIOD, &GPIO_InitStructure);	  				 //推挽输出 ，IO口速度为50MHz
    GPIO_SetBits(GPIOD,GPIO_Pin_2); 						     //PD2 输出高 

    // //通过寄存器初始化
    // RCC->APB2ENR |= 9 << 2;         //使能 PORT A D时钟
    // GPIOA->CRH &= 0XFFFFFFF0;       //PA8清零
    // GPIOA->CRH |= 0X00000003;       //PB15赋值 0011 推挽输出
    // GPIOA->ODR |= 1 << 8;           //高

    // GPIOD->CRL &= 0XFFFFF0FF;
    // GPIOD->CRL |= 0X00000300;
    // GPIOD->ODR |= 1 << 2;
}


