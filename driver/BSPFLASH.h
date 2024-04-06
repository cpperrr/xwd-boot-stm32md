#ifndef __FLASH_H__
#define __FLASH_H__
#include "stm32f10x.h"  
//#include "delay.h"

#define FLASH_BLOCK_SIZE	(1024)
#define FLASH_PAGE_SIZE    ((uint16_t)0x400)//定义页的大小，对于中等容量器件页的大小为0X400字节
#define STM_SECTOR_SIZE 1024 //字节
#define FLASH_START_ADDR	0x8002000      //flash起始地址
#define FLASH_END_ADDR		0x8001C00      //flash结束地址

//////////////////////////////////////////////////////////////////////////////////////////////////////
//用户根据自己的需要设置
#define STM32_FLASH_SIZE 128 	 		//所选STM32的FLASH容量大小(单位为K)
#define STM32_FLASH_WREN 1                      //使能FLASH写入(0，不是能;1，使能)
//////////////////////////////////////////////////////////////////////////////////////////////////////

//FLASH起始地址
#define STM32_FLASH_BASE 0x08000000 	//STM32 FLASH的起始地址
//FLASH解锁键值
//#define RDP_Key                  ((uint16_t)0x00A5)
//#define FLASH_KEY1               ((uint32_t)0x45670123)
//#define FLASH_KEY2               ((uint32_t)0xCDEF89AB)
 
//=======================================================================================
//函数名称：u16 STMFLASH_ReadHalfWord(u32 faddr)
//输入参数：ReadAddr	地址  pBuffer  NumToRead
//输出参数：0-->成功	非0-->失败
//功能描述：  返回Flash地址的数据
//注	意：
//=======================================================================================
u16 STMFLASH_ReadHalfWord(u32 faddr);		
 
//=======================================================================================
//函数名称：void STMFLASH_Write(u32 WriteAddr,u16 *pBuffer,u16 NumToWrite)
//输入参数：ReadAddr	地址  pBuffer  NumToRead
//输出参数：0-->成功	非0-->失败
//功能描述：  写Flash的底层函数
//注	意：
//=======================================================================================
void STMFLASH_Write(u32 WriteAddr,u16 *pBuffer,u16 NumToWrite);

//=======================================================================================
//函数名称：void STMFLASH_Read(u32 ReadAddr,u16 *pBuffer,u16 NumToRead) 
//输入参数：ReadAddr	地址  pBuffer  NumToRead
//输出参数：0-->成功	非0-->失败
//功能描述：  读取flash数据
//注	意：
//=======================================================================================
void STMFLASH_Read(u32 ReadAddr,u16 *pBuffer,u16 NumToRead);

//=======================================================================================
//函数名称：u8 FlashReadByte(u32 addr, u8* pData, u32 len)
//输入参数：addr	地址
//			buf		读缓冲
//          size	缓冲区大小
//输出参数：0-->成功	非0-->失败
//功能描述：flash读操作
//注	意：
//=======================================================================================
void FlashReadByte(u32 addr, u8* pData, u32 len); 

//=======================================================================================
//函数名称：u8 FlashErase(u32 addr, u8 size)
//输入参数：addr	地址    size  页大小
//输出参数：0-->成功	非0-->失败
//功能描述：flash擦除操作  addr为页的首地址，size是页的大小
//注	意：
//=======================================================================================
u8 FlashErase(u32 addr, u8 size);

//=======================================================================================
//函数名称：u8 FlashWriteSection(u32 addr, u8* Data, u8 Length);
//输入参数：addr	地址
//			buf		写缓冲
//			size	缓冲区大小
//输出参数：0-->成功	非0-->失败
//功能描述：flash写操作
//注	意：STM32每次写入必须是字或者半字 即Length必须为偶数
//=======================================================================================
u8 FlashWriteSection(u32 addr, u8* Data, u16 Length);

 

#endif

















