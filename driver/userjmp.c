#include "BSPTypeDef.h"
#include "UartLoadApp.h"
#include "BSPUART.h"
#include "BSPFLASH.h"

//函数名称	: static void ResetPro(void)
//输入参数	: addr:栈顶地址
//输出参数	: void
//静态变量	: void
//功	能	: 设置栈顶地址
//注	意	:  
//=======================================================================================
__asm void MSR_MSP(u32 addr) 
{
    MSR MSP, r0 			//set Main Stack value
    BX r14
}
//=============================================================================================
//函数名称: void iap_jump_app_s(void)
//输入参数: appxaddr:用户代码起始地址.
//输出参数: 
//返回值  : 
//功能描述: //跳转到app区域运行
//注意事项: void
//=============================================================================================
void jump_app(void)
{ 		
	typedef  void (*boot_type)(void);				//定义一个函数类型的参数.
	boot_type pro_entry;

	if(((*(vu32*)FLASH_APP1_ADDR)&0x2FFE0000)==0x20000000)	//检查栈顶地址是否合法.0x20000000是sram的起始地址,也是程序的栈顶地址
	{ 
		pro_entry=(boot_type)*(vu32*)(FLASH_APP1_ADDR+4);		//用户代码区第二个字为程序开始地址(复位地址)		
		MSR_MSP(*(vu32*)FLASH_APP1_ADDR);					//初始化APP堆栈指针(用户代码区的第一个字用于存放栈顶地址)
		pro_entry();									//跳转到APP.
	}
	else
	{		
		return;
	}
}



//=======================================================================================
//函数名称	: static void ResetPro(void)
//输入参数	: void
//输出参数	: void
//静态变量	: void
//功	能	: 软件复位
//注	意	:  
//=======================================================================================
//  void ResetPro(void)
//{  	 
////	typedef  void (*boot_type)(void);				//定义一个函数类型的参数.
////	boot_type boot_entry;
////	if(((*(vu32*)FLASH_IAP_ADDR)&0x2FFE0000)==0x20000000)	//检查栈顶地址是否合法.0x20000000是sram的起始地址,也是程序的栈顶地址
////	{ 
////		boot_entry=(boot_type)*(vu32*)(FLASH_IAP_ADDR+4);		//用户代码区第二个字为程序开始地址(复位地址)		
////		MSR_MSP(*(vu32*)FLASH_IAP_ADDR);					//初始化APP堆栈指针(用户代码区的第一个字用于存放栈顶地址)
////		boot_entry();									//跳转到APP.
////	}
////	else
////	{
////			return;
////	}
//}


