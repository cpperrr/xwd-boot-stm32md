//=======================================Copyright(c)===========================================
// 											Auto Code Making Company
//											Auto Code Department
//												CopyRight Private
//-------------------------------------------------FileMessage------------------------------------
//FileName	 CriticalZoom.c 
//Author	 YHY
//Creation Time
	  
//Description
//-----------------------------------------------当前版本修订----------------------------------
//修改人	
//版	本	 
//修改日期	
//Description	
//=============================================================================================
#ifndef _UARTLOADAPP_H
#define _UARTLOADAPP_H
//=============================================================================================
//头文件
//=============================================================================================
 #include "stm32f10x.h"  
//=============================================================================================
//宏定义
//=============================================================================================


#define FLASH_APP1_ADDR		0x08002000  	//第一个应用程序起始地址(存放在FLASH)
#define APPUPDATE_FLAG    0x5aa5

#define FLASH_IAP_ADDR		0x08000000  	//iap程序起始地址(存放在FLASH)
		
#define APP_CONFIG_ADDR 		0X08001FFC	//配置地址

#define PROTUES_TYPE                (0)         //0Modbus协议
#define MODBUS_BOOTStart_ADDR       (33)    //BOOT段烧录起始地址
#define MODBUS_BOOTSTATE_LEN        (9)
#define MODBUS_BOOTEnd_ADDR         (MODBUS_BOOTStart_ADDR + MODBUS_BOOTSTATE_LEN - 1)

#define MODBUS_CMD_ADDR             (MODBUS_BOOTStart_ADDR + MODBUS_BOOTSTATE_LEN)
#define MODBUS_CMD_LEN              (2)

#define MODBUS_LOAD_ADDR            (MODBUS_CMD_ADDR + MODBUS_CMD_LEN)
#define MODBUS_LOAD_LEN             (1028)


#define RX_BUF_LENGTH           (1200)
#define TX_BUF_LENGTH           (30)
#define SECTION_LENGTH          (1024)

//数据BUF长度
#define ADDR_SELF    			110        //UART2作为MODBUS通讯的从机地址

//功能码
#define READ_CODE    			0x03        //读指令     
#define WRITE_SINGLE_CODE		0x06        //单个写指令
#define WRITE_CODE   			0x10        //多个写指令

#define  BOOT_VERSION               (0x300)            /*Boot软件板本号*/
//#define  PRO_ENTRY_POINT     	    (0x197b)        	/*应用程序入口地址*/
//#define  BOOT_ENRTY_POINT    	    (0x10fb)        	/*Boot程序入口地址*/
//#define  PROGRAME_FLASH_START       (0x1900)    		/*应用程序起始地址*/
//#define  PROGRAME_FLASH_END         (0xdbff)       	    /*应用程序结束地址*/ 
 
//#define  BOOT_PRO_FLAG       		(0x5AA5)         	/*应用程序标志*/
//#define PROGRAME_FLASH_START  (0x1900)     /*应用程序起始地址*/
//#define PROGRAME_FLASH_END    (0xdbff)     /*应用程序结束地址*/   

//#define PROGRAME_CRC_START    (0x1900)      /*应用程序起始校验地址*/
//#define PROGRAME_CRC_END      (0x1D00)      /*应用程序结束校验地址*/  
//#define PROGRAME_CRC_LENGTH (PROGRAME_CRC_END - PROGRAME_CRC_START)     //校验长度

#define BOOT_VERSION        (0x300)         /*Boot软件板本号*/
#define BOOT_TRANS_FLAG     (0x5AA5)        /*转发标志*/
#define BOOT_PRO_FLAG       (0x5AA5)        /*应用程序标志*/
#define BOOT_PRO_TRY_FLAG   (0x5AA5)        /*已经尝试的标志*/

//#define PRO_ENTRY_POINT     (0x197b)        /*应用程序入口地址*/
//#define BOOT_ENRTY_POINT    (0x10fb)        /*Boot程序入口地址*/

//=============================================================================================
//定义数据类型
//=============================================================================================
typedef enum _BOOT_STATE_INFO
{
    e_PRO_TYPE     = 0,            //协议类型
    e_DEVICE_TYPE  = 1,            //设备类型
    e_DEVICE_NUM   = 2,            //设备号
    e_CHIP_TYPE    = 3,            //芯片类型
    e_SOFT_VERSION = 4,             //软件版本号
    e_BOOT_VERSION = 5,             //BOOT版本号
    e_CANLOAD_STATE = 6,            //烧录状态
    e_RUN_STATE     = 7,            //运行状态
    e_LOADING_NUM   = 8,            //正在烧写的段号
    e_CANLOAD_BUF_NUM = 9           //数据段长度
}E_BOOT_STATE;

typedef enum _MCU_TYPE_
{
    e_FREESCALE_DZ60 = 0,       //芯片类型
    e_FREESCALE_XEQ512 = 1,
    e_COMTEX_TM4C129ENDPT = 2,
}E_MCU_TYPE;

typedef enum _RUNMODEL_ENUM_
{
    e_TRANSMIT_MODEL = 0,    //转发模式
    e_CANLOAD_MODEL = 1,     //烧录模式
    e_PROGRAME_MODEL = 2,    //应用程序运行模式   
}E_RUNMODEL;

typedef enum _RUNMODEL_STATE_
{
    e_LOADTRANSMIT_STATE    = 0,            /*无应用程序转发状态*/
    e_PROTRANSMIT_STATE     = 1,            /*应用程序转发状态*/
    e_CANLOADREADY_OK       = 2,            /*烧录准备*/
    e_CANLOADREADY_FAIL     = 3,            /*烧录准备失败*/
   // e_CANLOADPRE_OK         = 4,            /*烧录就绪*/
    e_CANLOADTRANSMITING    = 5,            /*正在传输*/   
    e_CANLOADTRANSMIT_FAIL  = 6,            /*传输失败*/
 //   e_CANLOADTRANSMIT_OK    = 7,            /*传输成功*/
    e_CANLOADFLASH_FAIL     = 8,            /*烧录失败*/
    e_CANLOADFLASH_OK       = 9,             /*烧录成功*/
    e_PRORUN_STATE          = 10,           /*应用程序运行状态*/
}E_RUNMODEL_STATE;

typedef enum _MESSAGE_TYPE_
{
    e_Cmd_IntoTransmit      = 0xE0,                 /*进入转发模式*/
    e_Cmd_ExitTransmit      = 0xE1,                 /*退出转发模式*/
    e_Cmd_Reset             = 0xE2,                 /*请求复位*/
    e_Cmd_IntoLoad          = 0xE3,                 /*请求烧录*/
    e_Cmd_ExitLoad          = 0xE4,                 /*退出烧录*/
    e_Cmd_FinishLoad        = 0xE5,                 /*烧录完成*/
}E_MESSAGE_TYPE;

typedef enum _BOOT_FLAG_ADDR_               /*起始地址为0xD600*/
{
    e_BOOT_TRANSMIT_FLAG_ADDR = 6,          /*转发模志存放地址*/
    e_BOOT_CRC_ADDR = 8,                    /*应用程序CRC存放地址*/
    e_BOOT_PRO_FLAG_ADDR = 10,              /*应用程序标志*/
    e_BOOT_PRO_TRY_FLAG_ADDR = 12,          /*尝试运行标志*/
    e_BOOT_PRO_VERSION_ADDR = 14,           /*应用程序版本号*/ 
    e_BOOT_SOTE_VERION_ADDR = 16,           /*Boot软件版本号存储地址*/
    e_BOOT_NETECU_FLAG = 20,               /*设备号及设备类型标志存放地址*/
		e_BOOT_NET_ADDR = 22,                  /*设备类型存放地址*/
    e_BOOT_ECU_ADDR = 24,                  /*设备号存放地址*/

}E_BOOT_FLAG_ADDR;

//=============================================================================================
//声明全局变量
//=============================================================================================

//=============================================================================================
//函数声明
//=============================================================================================

//=======================================================================================
//函数名称	: void InitLoadParameter(void)
//输入参数	: void  
//输出参数	: void
//静态变量	: void
//功	能	:     初始化Load烧录参数
//注	意	:  
//=======================================================================================
void InitLoadParameter(void);

//=============================================================================
//函数名称: void UartRxCheckTask(void)
//输入参数: 
//输出参数:
//返回值  : 
//功能描述: 串口是否接收完成一帧数据检测
//注意事项: void
//=============================================================================
void UartCanLoaderTask(void);


#endif

//-----------------------------------------File END--------------------------------------------
