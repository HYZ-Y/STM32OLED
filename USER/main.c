#include "delay.h"
#include "led.h"
#include "key.h"
#include "sys.h"
#include "oled.h"
#include "oledfont.h"

// 按键控制LED,OLED显示
void OLED0S(void);
void OLED1S(void);

int main(void)
{
	u8 t;		//接收按键扫描结果

	delay_init();	//缺少导致无法正常显示
	LED_Init();
	KEY_Init();
	OLED_Init();  //OLED显示屏初始化

	OLED_Menu_Display();
	LED0 = 0;	//用于提示已进入程序中
	delay_ms(8000);
	delay_ms(8000);
	delay_ms(8000);
	OLED_Clear();

	OLED_ShowString(0, 0, "LED0:", 16);
	OLED_ShowString(0, 2, "LED1:", 16);
	OLED_ShowCHinese(40, 0, 3);		//开
	OLED_ShowCHinese(40, 2, 4);		//关

	while(1)
	{
		t = KEY_Scan(0);	//得到键值

		switch(t)
		{
			case KEY0_PRES:
					OLED0S();
					break;
			
			case KEY1_PRES:
					OLED1S();
					break;

			case WKUP_PRES:
					OLED0S();
					OLED1S();
					break;

			default:
				delay_ms(10);
		}
	}
 }

 void OLED0S(void)
{
	static u32 k1 = 0;

	LED0 = !LED0;
	k1++;

	if(k1 % 2 == 1)
	{
		OLED_ShowCHinese(40, 0, 4);//关
	
	}
	else if(k1 % 2 == 0)
		OLED_ShowCHinese(40, 0, 3);//开
}

void OLED1S(void)
{
	static u32 k2 = 0;

	LED1 = !LED1;
	k2++;

	if(k2 % 2 == 0)
	{
		OLED_ShowCHinese(40, 2, 4);//关
	
	}
	else if(k2 % 2 == 1)
		OLED_ShowCHinese(40, 2, 3);//开
}
