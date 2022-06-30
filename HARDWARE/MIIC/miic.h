#ifndef __MIIC_H
#define __MIIC_H	 
#include "sys.h"

// IO口方向设置
#define SDA_IN() {GPIOB->CRH &= 0X0FFFFFFF; GPIOB->CRH |= (u32)8 << 28;}
#define SDA_OUT() {GPIOB->CRH &= 0X0FFFFFFF; GPIOB->CRH |= (u32)3 << 28;}
// PB15
// IN:上拉/下拉输入模式,输入模式
// OUT:推挽输出模式,速度:50MHz

// IO口操作函数
#define IIC_SCL PCout(10)   //SCL
#define IIC_SDA PBout(15)   //SDA
#define READ_SDA PBin(15)   //输入SDA

void IIC_Init(void);        //IIC初始化
void IIC_Start(void);       //起始信号
void IIC_Stop(void);        //结束信号
u8 IIC_Wait_Ack(void);      //等待应答信号
void IIC_Ack(void);         //产生应答
void IIC_NAck(void);        //不产生应答
void IIC_Send_Byte(u8 txd); //发送一个字节
u8 IIC_Read_Byte(u8 ack);   //读一个字节

// void IIC_Write_One_Byte(u8 daddr, u8 addr, u8 data);
// u8 IIC_Read_One_Byte(u8 daddr, u8 addr);

#endif
