#ifndef __DEBUGSERIAL_H_
#define __DEBUGSERIAL_H_


#include "stdio.h"
#include "BSPTypeDef.h"
#include "stm32f10x.h"

//串口中断数据缓冲区
//串口接受缓冲区最大长度,不要超过255
#define SERIAL_MAX_LENGTH	250	


//=============================================================================================
//函数名称 void SCIInit(e_SCINUM sciNum, u32 rate)
//输入参数 SCINum SCI路数 uSCI_Config cfg:SCI配置 
//输出参数 void
//静态变量 void
//功    能 初始化SCI模块 
//注    意  
//=============================================================================================
void SCIInit(u32 baud);

 

//=============================================================================================
//函数名称u8 SCISendDataOnISR(e_SCINUM SCINum,u8 cmd, u8 *sendbuf, u8 dir,u8 size)
//输入参数 SCINum SCI路数 u8 data:待发送数据  
//输出参数 u8 0:成功  1：不成功
//静态变量 void
//功    能 发送串口数据 【查询模式】
//注    意  
//=============================================================================================
u8 SCISendDataOnISR(u8 *sendbuf,u16 size);


#endif










