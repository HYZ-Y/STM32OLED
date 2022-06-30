#ifndef __KEY_H
#define __KEY_H	 
#include "sys.h"

#define KEY0 GPIO_ReadInputDataBit(GPIOC, GPIO_Pin_5)  //读取按键0
#define KEY1 GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_15)  //读取按键1
#define WK_UP GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_0)  //读取按键WK_UP

// 位带操作
// #define KEY0 PCin(5)    //读取按键0
// #define KEY1 PAin(15)    //读取按键1
// #define WK_UP PAin(0)    //读取按键WK_UP

//按键按下
#define KEY0_PRES   1
#define KEY1_PRES   2
#define WKUP_PRES   3

void KEY_Init(void);    //IO初始化
u8 KEY_Scan(u8 mode);   //按键扫描

#endif
