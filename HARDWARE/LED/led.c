#include "led.h"

/*
LED0:PA8
LED1:PD2
�������
50MHz
��ʼ����
*/
void LED_Init(void)
{
    //�⺯����ʼ��
    GPIO_InitTypeDef  GPIO_InitStructure; 	
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | RCC_APB2Periph_GPIOD, ENABLE);	 //ʹ��PA,PD�˿�ʱ��
        
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8;				 //LED0-->PA8 �˿�����
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 		 //�������
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;		 //IO���ٶ�Ϊ50MHz
        
    GPIO_Init(GPIOA, &GPIO_InitStructure);					 //�����趨������ʼ��GPIOA8
    GPIO_SetBits(GPIOA,GPIO_Pin_8);						     //PA8 �����

    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2;	    		 //LED1-->PD2 �˿�����, �������
    GPIO_Init(GPIOD, &GPIO_InitStructure);	  				 //������� ��IO���ٶ�Ϊ50MHz
    GPIO_SetBits(GPIOD,GPIO_Pin_2); 						     //PD2 ����� 

    // //ͨ���Ĵ�����ʼ��
    // RCC->APB2ENR |= 9 << 2;         //ʹ�� PORT A Dʱ��
    // GPIOA->CRH &= 0XFFFFFFF0;       //PA8����
    // GPIOA->CRH |= 0X00000003;       //PB15��ֵ 0011 �������
    // GPIOA->ODR |= 1 << 8;           //��

    // GPIOD->CRL &= 0XFFFFF0FF;
    // GPIOD->CRL |= 0X00000300;
    // GPIOD->ODR |= 1 << 2;
}


