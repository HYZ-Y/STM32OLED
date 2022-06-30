#ifndef __OLED_H
#define __OLED_H	 
#include "sys.h"

#define Max_Column	128

#define OLED_CMD  0           //写命令
#define OLED_DATA 1           //写数据

extern u8 OLED_GRAM[8][128];         //显存数组(存储OLED屏幕信息)

void OLED_Init(void);                       //初始化
void OLED_Clear(void);                      //清屏
void OLED_WR_Byte(u8 dat, u8 cmd);          //写/读位   dat:数据 cmd:命令
void OLED_Display_On(void);                 //开启显示
void OLED_Display_off(void);                //关闭显示
void OLED_Set_CursorAddress(u8 x, u8 y);    //设置光标地址
void OLED_Draw_Point(u8 x, u8 y, u8 c);     //画点(x:0~127;y:0~63;c:1 亮, 0 灭;数据:低前高后)
void OLED_Refresh_Screen(void);             //更新显存到OLED
void OLED_DisplayString(u8 x, u8 y, u8 width, u8 height, u8 *str);  //字符串显示
void OLED_Menu_Display(void);               //初始界面
void OLED_ShowCHinese(u8 x,u8 y,u8 no);     //显示中文
void OLED_ShowChar(u8 x, u8 y, u8 chr, u8 Char_Size);       //显示一个字符
void OLED_ShowString(u8 x, u8 y, u8 *chr, u8 Char_Size);    //显示一个字符串
void OLED_DrawBMP(u8 x0, u8 y0, u8 x1, u8 y1, const u8 BMP[]);    //画图

#endif


