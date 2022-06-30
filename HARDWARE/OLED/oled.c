#include "oled.h"
#include "miic.h"
#include "delay.h"
#include "string.h"
#include "oledfont.h"

u8 OLED_GRAM[8][128];/*定义OLED显存数组*/

//OLED初始化
void OLED_Init(void)
{
    IIC_Init();
    delay_ms(200);  //500ms

    OLED_WR_Byte(0xAE, OLED_CMD);   //关闭显示
    OLED_WR_Byte(0x00, OLED_CMD);   //设置显示位置-列低地址
    OLED_WR_Byte(0x10, OLED_CMD);   //设置显示位置-列高地址
    OLED_WR_Byte(0x40, OLED_CMD);   //设置起始行地址,集映射RAM显示起始行(0x00~0x3F)
	OLED_WR_Byte(0xB0,OLED_CMD);    //--set page address

    OLED_WR_Byte(0x81, OLED_CMD);   //设置对比度控制寄存器,本指令不动,对比度由下面一条指令修改
    OLED_WR_Byte(0xFF, OLED_CMD);   //微调对比度的值,可设置范围0x01~0xff(设置SEG输出电流亮度)

    OLED_WR_Byte(0xA1, OLED_CMD);   //**列 段重定义设置,bit0:0,0->0;1,0->127; 0xa0左右反置 0xa1正常

    OLED_WR_Byte(0xA6, OLED_CMD);   //设置正常显示(设置显示方式;A6:正常显示;A7反相显示 1010 0110x 正常:OFF:0 ON:1 反相:ON:0 OFF:1)

    OLED_WR_Byte(0xA8, OLED_CMD);   //*设置驱动路数 设置多路复用比(1比64)
    OLED_WR_Byte(0x3F, OLED_CMD);   //1/64 duty(默认0X3F(1/64))

    OLED_WR_Byte(0xC8, OLED_CMD);   //**设置COM扫描方向;bit3:0,普通模式;1,重定义模式 COM[N-1]->COM0;N:驱动路数   0xc0上下反置 0xc8正常

    OLED_WR_Byte(0xD3, OLED_CMD);   //设置显示偏移位移映射RAM计数器(0x00~0x3F)
    OLED_WR_Byte(0x00, OLED_CMD);   //不偏移

    OLED_WR_Byte(0xD5, OLED_CMD);   //设置显示时钟分频比/振荡器频率
    OLED_WR_Byte(0x80, OLED_CMD);   //设置分频比，设置时钟为100帧/秒

	// OLED_WR_Byte(0xD8,OLED_CMD);    //set area color mode off
	// OLED_WR_Byte(0x05,OLED_CMD);    //没找到

    OLED_WR_Byte(0xD9, OLED_CMD);   //设置预充电周期
    OLED_WR_Byte(0xF1, OLED_CMD);   //设置预充15个时钟，放电1个时钟([3:0],PHASE 1(放电);[7:4],PHASE 2(充电);)

    OLED_WR_Byte(0xDA, OLED_CMD);   //设置COM硬件引脚配置  
    OLED_WR_Byte(0x12, OLED_CMD);   //[5:4]配置 [4]:0 *顺序COM 引脚配置 1:可选COM引脚配置;[5]:0 *禁用 COM 左/右重映射 1 使能COM 左/右重映射 

    OLED_WR_Byte(0xDB, OLED_CMD);   //设置VCOMH 电压倍率
    OLED_WR_Byte(0x30, OLED_CMD);   //Set VCOM 释放电压([5:4] 00,0.65*vcc;01,0.71*vcc;10,0.77*vcc;11,0.83*vcc;)

    OLED_WR_Byte(0x20, OLED_CMD);   //set Memory Addressing Mode
    OLED_WR_Byte(0x02, OLED_CMD);   //A[1:0] = 10b, Page Addressing Mode (RESET)
                                    //A[1:0] = 00b, Horizontal Addressing Mode
                                    //A[1:0] = 01b, Vertical Addressing Mode
                                    //A[1:0] = 11b, Invalid(无效)

    OLED_WR_Byte(0x8D, OLED_CMD);   //设置充电泵启用/禁用
    OLED_WR_Byte(0x14, OLED_CMD);   //设置(0x10禁用,0x14启用) A[7]:0 A[0]:0 Hex code :14h Charge Pump Mode 7.5V(RESET)  

    OLED_WR_Byte(0xA4, OLED_CMD);   // 全局显示开启;bit0:1,开启;0,关闭;(白屏/黑屏) (0xa4/0xa5)
    OLED_WR_Byte(0xA6, OLED_CMD);   //设置正常显示(设置显示方式;A6:正常显示;A7反相显示 1010 0110x 正常:OFF:0 ON:1 反相:ON:0 OFF:1)
    OLED_WR_Byte(0xAF, OLED_CMD);   //开启显示;0xAE关闭显示

    OLED_Clear();
    // OLED_Set_CursorAddress(0, 0);   //原点
}

// 清屏
void OLED_Clear(void)
{
    u8 i, n;

    for(i = 0; i < 8; i++)
    {
        OLED_WR_Byte(0xB0 + i, OLED_CMD);   //设置页地址(0~7)
        OLED_WR_Byte(0x00, OLED_CMD);       //设置显示位置-列低地址
        OLED_WR_Byte(0x10, OLED_CMD);       //设置显示位置-列高地址

        for(n = 0; n < 128; n++)
        {
            OLED_WR_Byte(0x00, OLED_DATA);
        }
    }
}

//写1:数据 0:命令
void OLED_WR_Byte(u8 dat, u8 cmd)
{
    IIC_Start();
    IIC_Send_Byte(0x78);        //从机地址(OLED)写
    IIC_Wait_Ack();

    if(cmd)
        IIC_Send_Byte(0x40);    //数据
    else
        IIC_Send_Byte(0x00);    //命令
    IIC_Wait_Ack();

    IIC_Send_Byte(dat);
    IIC_Wait_Ack();
    IIC_Stop();
}

//开启OLED显示
void OLED_Display_On(void)
{
    OLED_WR_Byte(0x8D, OLED_CMD);
    OLED_WR_Byte(0x14, OLED_CMD);
    OLED_WR_Byte(0xAF, OLED_CMD);
}

//关闭OLED显示
void OLED_Display_off(void)
{
    OLED_WR_Byte(0x8D, OLED_CMD);
    OLED_WR_Byte(0x10, OLED_CMD);
    OLED_WR_Byte(0xAE, OLED_CMD); 
}

/*
设置OLED光标位置
x:0~127
y:0~7
*/
void OLED_Set_CursorAddress(u8 x, u8 y)
{
    OLED_WR_Byte(0xB0 + y, OLED_CMD);               //Set Page Start Address (y:0~7)
    OLED_WR_Byte((x & 0xF0) >> 4 | 0x10, OLED_CMD); //Set Higher Column Start Address    
    OLED_WR_Byte((x & 0x0F) | 0x00, OLED_CMD);      //Set Lower Column Start Address
}

//待处理
void OLED_Draw_Point(u8 x, u8 y, u8 c)
{
    u8 page, addr;
    page = y / 8;       //每页有8行数据
    addr = y % 8;

    if(c)
        OLED_GRAM[page][x] |= 1 << addr;
    else
        OLED_GRAM[page][x] &= ~(1 << addr);
}

//待处理
void OLED_Refresh_Screen(void)
{
    u8 page, list;      //定义页地址和列地址

    for(page = 0; page < 8; page++)
    {
        OLED_WR_Byte(0xB0 + page, OLED_CMD);    //设置页地址（0~7）
        OLED_WR_Byte(0x00, OLED_CMD);           //设置显示位置―列低地址
        OLED_WR_Byte(0x10, OLED_CMD);           //设置显示位置―列高地址

        for(list = 0; list < 128; list++)
        {
            OLED_WR_Byte(OLED_GRAM[page][list], OLED_DATA);
        }
    }

    memset(OLED_GRAM, 0, sizeof(OLED_GRAM));    //清空显示数组
}

//待处理
void OLED_DisplayString(u8 x, u8 y, u8 width, u8 height, u8 *str)
{
    // u8 addr = 0, i;
    // u16 font = 0;

    // while(*str != '\0')     //连续显示
    // {      
    //     if(*str >= ' ' && *str <= '~')      //显示英文
    //     {
    //         addr = *str - ' ';      //取模从空格开始的,计算下标
    //         //写8*16 ASCII字符的上半部分
    //         OLED_Set_CursorAddress(x, y);   //设置光标的位置

    //         for(i = 0; i < width / 2; i++)  //横向写width列
    //         {
    //             OLED_WR_Byte(ASCII_8_16[addr][i], OLED_DATA);
    //         }
    //         //写8*16 ASCII字符的下半部分
    //         OLED_Set_CursorAddress(x, y + 1);   //设置光标的位置
    //         for(i = 0; i < width / 2; i++)      //横向写width列
    //         {
    //             OLED_WR_Byte(ASCII_8_16[addr][i + width / 2], OLED_DATA);
    //         }

    //         str++;          //继续显示下一个字符
    //         x += width / 2; //在下一个位置继续显示数据
    //     }
    //     else    //显示中文
    //     {
    //         OLED_Set_CursorAddress(x, y);   //设置光标的位置
    //         font = ((*str) << 8) + (*(str + 1));

    //         switch(font)
    //         {
    //             case 0xCEC2:addr = 0; break;    //温
    //             default:break;
    //         }

    //         for(i = 0; i < width; i++)      //横向写width列
    //         {
    //             OLED_WR_Byte(ChineseFont_16_16[addr][i], OLED_DATA);
    //         }
    //         // 写8*16 ASCII字符的下半部分
    //         OLED_Set_CursorAddress(x, y + 1);

    //         for(i = 0; i < width; i++)
    //         {
    //             OLED_WR_Byte(ChineseFont_16_16[addr][i + width], OLED_DATA);
    //         }
            
    //         str += 2;   //继续显示下一个字符
    //         x += width; //在下一个位置继续显示数据
    //     }
    // }
}

// 待处理
void OLED_Menu_Display(void)
{
	//OLED_DisplayString(0,0,16,16,"黄月宗");
    // OLED_ShowCHinese(0,0,0);	//黄
	// OLED_ShowCHinese(16,0,1);	//月
	// OLED_ShowCHinese(32,0,2);	//宗

    OLED_DrawBMP(0, 0, 128, 8, BMP1);
}

/*
显示单个字符
x:
y:
Char_Size:8 16
未完
*/
void OLED_ShowChar(u8 x, u8 y, u8 chr, u8 Char_Size)
{
    u8 c = 0, i = 0;
    c = chr - ' ';      //得到偏移后的值

    if(x > Max_Column - 1)
    {
        x = 0;
        y += 2;
    }

    if(Char_Size == 16)
    {
        OLED_Set_CursorAddress(x, y);

        for(i = 0; i < 8; i++)
        {
            OLED_WR_Byte(F8X16[c * 16 + i], OLED_DATA);            
        }

        OLED_Set_CursorAddress(x, y + 1);
        for(i = 0; i < 8; i++)
        {
            OLED_WR_Byte(F8X16[c * 16 + i + 8], OLED_DATA);            
        }
    }
    else
    {
        OLED_Set_CursorAddress(x, y);
        for(i = 0; i < 6; i++)
        {
            OLED_WR_Byte(F6x8[c][i], OLED_DATA);
        }
    }
}

/*
显示字符串
x:
y:
Char_Size:
未完
*/
void OLED_ShowString(u8 x, u8 y, u8 *chr, u8 Char_Size)
{
    u8 j = 0;

    while(chr[j] != '\0')
    {
        OLED_ShowChar(x, y, chr[j], Char_Size);
        x += 8;

        if(x > 120)
        {
            x = 0;
            y += 2;
        }

        j++;
    }
}

/*
显示中文
x:
y:
no:所在字的编号
未完
*/
void OLED_ShowCHinese(u8 x,u8 y,u8 no)
{      			    
	u8 t,adder=0;
	OLED_Set_CursorAddress(x,y);
	
    for(t=0;t<16;t++)
	{
		OLED_WR_Byte(Hzk[2*no][t],OLED_DATA);
		adder+=1;   //没用
    }

    // 文字下半部分	
	OLED_Set_CursorAddress(x,y+1);	
    for(t=0;t<16;t++)
	{	
		OLED_WR_Byte(Hzk[2*no+1][t],OLED_DATA);
		adder+=1;   //没用
  }
}

// 待处理
/***********功能描述：
 * 显示显示BMP图片128×64
 * 起始点坐标(x,y)
 * x的范围0～127
 * y为页的范围0～7
 * *****************/
void OLED_DrawBMP(u8 x0, u8 y0, u8 x1, u8 y1,const u8 BMP[])
{
    u32 j = 0;
    u8 x, y;

    if(y1 % 8 == 0)
        y = y1 / 8;
    else
        y = y1 / 8 + 1;

    for(y = y0; y < y1; y++)
    {
        OLED_Set_CursorAddress(x0, y);

        for(x = x0; x < x1; x++)
        {
            OLED_WR_Byte(BMP[j++], OLED_DATA);
        }
    }
}


