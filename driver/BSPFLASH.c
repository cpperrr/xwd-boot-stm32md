#include "BSPFLASH.h"
#include "BSPTypeDef.h"

u16 STMFLASH_BUF[STM_SECTOR_SIZE/2];//最多是2K字节


#define ERR_FLASH_WR        1           //flash写出错
#define ERR_FLASH_RD        1           //flash读出错
#define NULL                   ((void *) 0)
	
#define FLASH_WR_BYTE(addr) *(vu8*)addr;

//静态函数申明
//=======================================================================================
//函数名称：void STMFLASH_Write_NoCheck(u32 WriteAddr,u16 *pBuffer,u16 NumToWrite)   
//输入参数：ReadAddr	地址  pBuffer  NumToRead
//输出参数：0-->成功	非0-->失败
//功能描述：  Flash不为检查擦写
//注	意：
//=======================================================================================
static void STMFLASH_Write_NoCheck(u32 WriteAddr,u16 *pBuffer,u16 NumToWrite);
//=======================================================================================
//函数名称：static u8 FlashSectionErase(u32 addr)
//输入参数：addr	地址
//输出参数：0-->成功	非0-->失败
//功能描述：flash擦除操作
//注	意：
//=======================================================================================
static u8 FlashSectionErase(u32 Addr);

//=======================================================================================
//函数名称：void STMFLASH_Write_NoCheck(u32 WriteAddr,u16 *pBuffer,u16 NumToWrite)   
//输入参数：ReadAddr	地址  pBuffer  NumToRead
//输出参数：0-->成功	非0-->失败
//功能描述：  Flash不为检查擦写
//注	意：
//=======================================================================================
static void STMFLASH_Write_NoCheck(u32 WriteAddr,u16 *pBuffer,u16 NumToWrite)   
{ 			 		 
    u16 i;
    for(i=0;i<NumToWrite;i++)
    {
        FLASH_ProgramHalfWord(WriteAddr,pBuffer[i]);
        WriteAddr+=2;//地址增加2.
    }  
}   
//=======================================================================================
//函数名称：static u8 FlashSectionErase(u32 addr)
//输入参数：addr	地址
//输出参数：0-->成功	非0-->失败
//功能描述：flash擦除操作
//注	意：
//=======================================================================================
static u8 FlashSectionErase(u32 Addr)
{
		volatile FLASH_Status FLASHStatus = FLASH_COMPLETE;
	
		if((Addr % FLASH_PAGE_SIZE) != 0)
		{
				return FALSE;
		}
		
		FLASH_UnlockBank1();//解锁BANK1，只有解锁后才能擦除和写入
		FLASHStatus = FLASH_ErasePage(Addr);	//擦除FLASH
		FLASH_LockBank1();//操作完成后加锁
		
		if(FLASH_COMPLETE == FLASHStatus)
		{
				return TRUE;
		}
		else
		{
				return FALSE;
		}	
}
//=======================================================================================
//函数名称：u16 STMFLASH_ReadHalfWord(u32 faddr)
//输入参数：ReadAddr	地址  pBuffer  NumToRead
//输出参数：0-->成功	非0-->失败
//功能描述：  返回Flash地址的数据
//注	意：
//=======================================================================================
u16 STMFLASH_ReadHalfWord(u32 faddr)
{
    return *(vu16*)faddr; 
}
//=======================================================================================
//函数名称：u8 FlashReadByte(u32 addr, u8* pData, u32 len)
//输入参数：addr	地址
//			buf		读缓冲
//          size	缓冲区大小
//输出参数：0-->成功	非0-->失败
//功能描述：flash读操作
//注	意：
//=======================================================================================
void FlashReadByte(u32 addr, u8* pData, u32 len)
{   
	if(addr < FLASH_START_ADDR || addr > FLASH_END_ADDR)
	{
		return; 
	}

	if(pData == NULL)
	{
		return; 
	}

	while(len--)
	{
		*pData = FLASH_WR_BYTE(addr);
		pData++;
		addr++;
	}

}
//=======================================================================================
//函数名称：void STMFLASH_Write(u32 WriteAddr,u16 *pBuffer,u16 NumToWrite)
//输入参数：ReadAddr	地址  pBuffer  NumToRead
//输出参数：0-->成功	非0-->失败
//功能描述：  写Flash的底层函数
//注	意：
//=======================================================================================
void STMFLASH_Write(u32 WriteAddr,u16 *pBuffer,u16 NumToWrite)	
{
    u32 secpos;	   //扇区地址
    u16 secoff;	   //扇区内偏移地址(16位字计算)
    u16 secremain; //扇区内剩余地址(16位字计算)	   
    u16 i;    
    u32 offaddr;   //去掉0X08000000后的地址
    if(WriteAddr<STM32_FLASH_BASE||(WriteAddr>=(STM32_FLASH_BASE+1024*STM32_FLASH_SIZE)))return;//非法地址
    FLASH_Unlock();						//解锁
    offaddr=WriteAddr-STM32_FLASH_BASE;		        //实际偏移地址.
    secpos=offaddr/STM_SECTOR_SIZE;			//扇区地址  0~127 for STM32F103RBT6
    secoff=(offaddr%STM_SECTOR_SIZE)/2;		//在扇区内的偏移(2个字节为基本单位.)
    secremain=STM_SECTOR_SIZE/2-secoff;		//扇区剩余空间大小   
    if(NumToWrite<=secremain)secremain=NumToWrite;//不大于该扇区范围
    while(1) 
    {	
        STMFLASH_Read(secpos*STM_SECTOR_SIZE+STM32_FLASH_BASE,STMFLASH_BUF,STM_SECTOR_SIZE/2);//读出整个扇区的内容
        for(i=0;i<secremain;i++)//校验数据
        {
            if(STMFLASH_BUF[secoff+i]!=0XFFFF)break;//需要擦除  	  
        }
        if(i<secremain)//需要擦除
        {
            FLASH_ErasePage(secpos*STM_SECTOR_SIZE+STM32_FLASH_BASE);//擦除这个扇区
            for(i=0;i<secremain;i++)//复制
            {
                STMFLASH_BUF[i+secoff]=pBuffer[i];	  
            }
            STMFLASH_Write_NoCheck(secpos*STM_SECTOR_SIZE+STM32_FLASH_BASE,STMFLASH_BUF,STM_SECTOR_SIZE/2);//写入整个扇区  
        }else STMFLASH_Write_NoCheck(WriteAddr,pBuffer,secremain);//写已经擦除了的,直接写入扇区剩余区间. 				   
        if(NumToWrite==secremain)break;//写入结束了
        else//写入未结束
        {
            secpos++;				//扇区地址增1
            secoff=0;				//偏移位置为0 	 
            pBuffer+=secremain;  	//指针偏移
            WriteAddr+=secremain;	//写地址偏移	   
            NumToWrite-=secremain;	//字节(16位)数递减
            if(NumToWrite>(STM_SECTOR_SIZE/2))secremain=STM_SECTOR_SIZE/2;//下一个扇区还是写不完
            else secremain=NumToWrite;//下一个扇区可以写完了
        }	 
    };	
    FLASH_Lock();//上锁
}  
//=======================================================================================
//函数名称：void STMFLASH_Read(u32 ReadAddr,u16 *pBuffer,u16 NumToRead) 
//输入参数：ReadAddr	地址  pBuffer  NumToRead
//输出参数：0-->成功	非0-->失败
//功能描述：  读取flash数据
//注	意：
//=======================================================================================
void STMFLASH_Read(u32 ReadAddr,u16 *pBuffer,u16 NumToRead)   	
{
    u16 i;
    for(i=0;i<NumToRead;i++)
    {
        pBuffer[i]=STMFLASH_ReadHalfWord(ReadAddr);//读取2个字节.
        ReadAddr+=2;//偏移2个字节.	
    }
}
 

//=======================================================================================
//函数名称：u8 FlashErase(u32 addr, u8 size)
//输入参数：addr	地址    size  页大小
//输出参数：0-->成功	非0-->失败
//功能描述：flash擦除操作  addr为页的首地址，size是页的大小
//注	意：
//=======================================================================================
u8 FlashErase(u32 addr, u8 size)
{	
	u16 i = 0;

	for(i = 0;i < size;i++)
	{
		(void)FlashSectionErase(addr);
		addr += FLASH_BLOCK_SIZE;
	}

	return OK;
}
//=======================================================================================
//函数名称：u8 FlashWriteSection(u32 addr, u8* Data, u8 Length);
//输入参数：addr	地址
//			buf		写缓冲
//			size	缓冲区大小
//输出参数：0-->成功	非0-->失败
//功能描述：flash写操作
//注	意：STM32每次写入必须是字或者半字 即Length必须为偶数
//=======================================================================================
u8 FlashWriteSection(u32 addr, u8* Data, u16 Length)
{
		u16 i = 0;
		u16 flashHalfWord = 0xffff;
		u32 realAddr = 0;
		volatile FLASH_Status FLASHStatus = FLASH_COMPLETE;

//		if((FALSE == FlashSectionErase(addr)) || (Length % 2 == 1))
//		{
//				return FALSE;
//		}

		realAddr = addr;
		FLASH_UnlockBank1();//解锁BANK1，只有解锁后才能擦除和写入
		for(i = 0; i < Length; i += 2)
		{
				flashHalfWord = Data[i+1];
				flashHalfWord = (flashHalfWord << 8) + Data[i];
				FLASHStatus = FLASH_ProgramHalfWord(realAddr, flashHalfWord);	//写入FLASH
				realAddr += 2;
		}
		FLASH_LockBank1();//操作完成后加锁

		if(FLASHStatus == FLASH_COMPLETE)
		{
				return TRUE;
		}
		else
		{
				return FALSE;
		}	
}








