#include "miic.h"
#include "delay.h"

/*
SDA:PB15;
SCL:PC10;
推挽输出
50MHz
初始化高
*/
void IIC_Init(void)
{
    // //库函数初始化
    // GPIO_InitTypeDef  GPIO_InitStructure; 	
    // RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB | RCC_APB2Periph_GPIOC, ENABLE);	 //使能PB,PC端口时钟
        
    // GPIO_InitStructure.GPIO_Pin = GPIO_Pin_15;				 //SDA-->PB15 端口配置
    // GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 		 //推挽输出
    // GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;		 //IO口速度为50MHz
        
    // GPIO_Init(GPIOB, &GPIO_InitStructure);					 //根据设定参数初始化GPIOA8
    // GPIO_SetBits(GPIOB,GPIO_Pin_15);						 //PB15 输出高

    // GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;	    		 //SCL-->PC10 端口配置, 推挽输出
    // GPIO_Init(GPIOC, &GPIO_InitStructure);	  				 //推挽输出 ，IO口速度为50MHz
    // GPIO_SetBits(GPIOC,GPIO_Pin_10);                         //PC10 输出高

    //通过寄存器初始化
    RCC->APB2ENR |= 3 << 3;         //使能 PORT B C时钟
    GPIOB->CRH &= 0x0FFFFFFF;       //PB15清零
    GPIOB->CRH |= 0x30000000;       //PB15赋值 0011 推挽输出
    GPIOB->ODR |= 1 << 15;          //高

    GPIOC->CRH &= 0xFFFFF0FF;
    GPIOC->CRH |= 0x00000300;
    GPIOC->ODR |= 1 << 10;
}

// IIC起始信号
void IIC_Start(void)
{
    SDA_OUT();

    IIC_SDA = 1;
    IIC_SCL = 1;
    delay_us(4);

    IIC_SDA = 0;
    delay_us(4);

    IIC_SCL = 0;    //钳住I2C总线，准备发送或接收数据
}

// IIC结束信号
void IIC_Stop(void)
{
    SDA_OUT();

    IIC_SCL = 0;
    IIC_SDA = 0;
    delay_us(4);

    IIC_SCL = 1;
    delay_us(4);

    IIC_SDA = 1;
}

//IIC等待应答 1:失败;0:成功
u8 IIC_Wait_Ack(void)
{
    u8 ucErrTime = 0;
    SDA_IN();
    IIC_SDA = 1;
    delay_us(1);
    IIC_SCL = 1;
    delay_us(1);

    while(READ_SDA)
    {
        ucErrTime++;

        if(ucErrTime > 250)
        {
            IIC_Stop();
            return 1;
        }
    }

    IIC_SCL = 0;
    return 0;
}

// IIC应答
void IIC_Ack(void)
{
    IIC_SCL = 0;
    SDA_OUT();

    IIC_SDA = 0;
    delay_us(2);
    IIC_SCL = 1;
    delay_us(2);

    IIC_SCL = 0;
}

// IIC非应答
void IIC_NAck(void)
{
    IIC_SCL = 0;
    SDA_OUT();

    IIC_SDA = 1;
    delay_us(2);
    IIC_SCL = 1;
    delay_us(2);

    IIC_SCL = 0;  
}

// IIC发送一个字节
void IIC_Send_Byte(u8 txd)
{
    u8 t;
    SDA_OUT();
    IIC_SCL = 0;    //拉低时钟开始传输数据

    for(t = 0; t < 8; t++)
    {
        IIC_SDA = (txd & 0x80) >> 7;    //从高位到低位传输
        txd <<= 1;

        //从机读取数据
        IIC_SCL = 1;
        delay_us(2);
        IIC_SCL = 0;
        delay_us(2);
    }
}

// IIC读取一个字节  ack = 1时,发送ACK; ack = 0时,发送nACK
u8 IIC_Read_Byte(u8 ack)
{
    u8 i, receive = 0;
    SDA_IN();

    for(i = 0; i < 8; i++)
    {
        IIC_SCL = 0;
        delay_us(2);
        IIC_SCL = 1;
        receive <<= 1;

        if(READ_SDA)
            receive++;
        
        delay_us(1);
    }

    if(!ack)
        IIC_NAck();
    else
        IIC_Ack();

    return receive;
}


