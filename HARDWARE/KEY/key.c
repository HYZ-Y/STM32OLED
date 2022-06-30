#include "key.h"
#include "delay.h"

void KEY_Init(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | RCC_APB2Periph_GPIOC, ENABLE);    //使能PORTA, PORTC时钟
    GPIO_PinRemapConfig(GPIO_Remap_SWJ_JTAGDisable, ENABLE);    //关闭jtag,使能SWD,可以用SWD模式调试

    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_15;      //(PA)15
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;   //设置成上拉输入
    GPIO_Init(GPIOA, &GPIO_InitStructure);          //初始化GPIOA15

    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5;       //(PC)5
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;   //设置成上拉输入
    GPIO_Init(GPIOC, &GPIO_InitStructure);          //初始化GPIOC5

    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;       //(PA)0
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPD;   //设置成下拉输入
    GPIO_Init(GPIOA, &GPIO_InitStructure);          //初始化GPIOA0
}

// mode:0,不支持连续按; 1,支持连续按
u8 KEY_Scan(u8 mode)
{
    static u8 key_up = 1;   //按键可按下(1)/松开(0)标志
    if(mode)  key_up = 1;   //支持连按
         
    if(key_up && (KEY0 == 0 || KEY1 == 0 || WK_UP == 1)) 
    {
        delay_ms(10);   //去抖动
        key_up = 0;

        // 响应优先级KEY0>KEY1>WK_UP
        if(KEY0 == 0)       return KEY0_PRES;
        else if(KEY1 == 0)  return KEY1_PRES;
        else if(WK_UP == 1) return WKUP_PRES;
    }
    else if(KEY0 == 1 && KEY1 == 1 && WK_UP == 0)  
        key_up = 1;

    return 0;       //无按键按下   
}


