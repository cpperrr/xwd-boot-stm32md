//=======================================Copyright(c)===========================================
// 											欣旺达电子股份有限公司
//											研发三部软件小组
//												软件部 
//-------------------------------------------------文件信息------------------------------------
//文件名		: main.c 
//创建人		: YHY
//创建日期		:  
//描	述		: 
//-----------------------------------------------当前版本修订----------------------------------
//修改人		:
//版	本		: 
//修改日期		:
//描	述		:
//=============================================================================================

//=============================================================================================
//头文件定义
//=============================================================================================
#include "mainInclude.h"
extern void NVIC_Configuration();

/*======================================================================================
//函数名:
//输入参数：
//输出参数：
//注意事项：
//功能：
=======================================================================================*/
int main(void)
{
	//NVIC_Group_Init();//系统默认中断分组
	NVIC_Configuration();
	SCIInit(9600);

//	Delay_Init();	 
	InitLoadParameter();	 
		    SysTick_Config((SystemCoreClock) / 100); // 设置定时器10ms中断	
	while(1)
	{					
			//不能开中断，如果开中断需要做中断跳转,因为中断入口地址只有一个Boot用了以后App 程序便和Boot共用
			UartCanLoaderTask();				 		 
		
	}
}




