#include "BSPUART.h"
#include "stm32f10x.h"


extern   void NVIC_Configuration(void);
//=============================================================================================
//函数名称 void Debug_Serial_Init(u32 baud)
//输入参数 u32 baud
//输出参数 void
//静态变量 void
//功    能 初始化SCI模块 
//注    意  
//=============================================================================================
void SCIInit(u32 baud)
{
//	u32 NVICtemp    = 0;       			 	//用于NVIC控制器的变量
	GPIO_InitTypeDef GPIO_InitStructure;	//gpio初始化变量
	USART_InitTypeDef USART_InitStructure;	//串口初始化结构体
	//下面这些都是一样的配置，所以放在判断之外
	USART_InitStructure.USART_BaudRate = baud;//波特率;
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;	 //8位数据位
	USART_InitStructure.USART_StopBits = USART_StopBits_1;		 //1位停止位
	USART_InitStructure.USART_Parity = USART_Parity_No;			 //奇偶校验失能
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;	//硬件流控失能
	USART_InitStructure.USART_Mode = USART_Mode_Tx | USART_Mode_Rx;				   //发送与接收都使能

	//使能串口所有GPIO模块时钟,uart模块时钟,并使能AFIO模块时钟 ,uart1位于apb2
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA|RCC_APB2Periph_AFIO|RCC_APB2Periph_USART1, ENABLE);
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	//io模块配置pa9
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;		//设置TX引脚
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;		//复用推挽输出
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	//pa10
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;		//设置RX引脚
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;		//浮空输入
	GPIO_Init(GPIOA, &GPIO_InitStructure);		//io配置完成

	//中断优先级变量解码,在sys文件中设置的宏定义
// 	NVICtemp = NVIC_EncodePriority(BOARD_NVIC_GROUP , UART1_PreemptionPriority , UART1_SubPriority);    
//	 NVIC_SetPriority(USART1_IRQn,NVICtemp);          //设置中断优先级
	 
	 
	USART_Init(USART1, &USART_InitStructure);		//初始化USART
	 USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);	 //接收中断使能
	 NVIC_EnableIRQ(USART1_IRQn);                     //使能对应中断 
	USART_Cmd(USART1, ENABLE);		//使能串口模块	
	USART_GetFlagStatus(USART1, USART_FLAG_TC);
}
 
  void NVIC_Configuration(void)
{
  NVIC_InitTypeDef NVIC_InitStructure;
  
  /* 嵌套向量中断控制器组选择 */
	/* 提示 NVIC_PriorityGroupConfig() 在整个工程只需要调用一次来配置优先级分组*/
  NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
  
  /* 配置USART为中断源 ，默认使用串口1作为中断源*/
  NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;
  /* 抢断优先级*/
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
  /* 子优先级 */
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
  /* 使能中断 */
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  /* 初始化配置NVIC */
  NVIC_Init(&NVIC_InitStructure);
}

// 串口中断优先级配置
//	NVIC_Configuration();
//	
//	// 使能串口接收中断，接收数据寄存器非空，表示接收到数据就产生中断
//	USART_ITConfig(DEBUG_USARTx, USART_IT_RXNE, ENABLE);

//=============================================================================================
//函数名称u8 SCISendDataOnISR(e_SCINUM SCINum,u8 cmd, u8 *sendbuf, u8 dir,u8 size)
//输入参数 SCINum SCI路数 u8 data:待发送数据  
//输出参数 u8 0:成功  1：不成功
//静态变量 void
//功    能 发送串口数据 
//注    意  
//=============================================================================================
u8 SCISendDataOnISR(u8 *sendbuf,u16 size)
{
		u8 timerCount = 0;    
		if(size == 0)
		{
				return FALSE;
		}	
		//禁止接收
		//PTGD_PTGD5 = 0;     
    for(timerCount = 0;timerCount < 100;timerCount++)
    {
        ;
    }       	
		while(size--)
		{
			while(!USART_GetFlagStatus(USART1, USART_FLAG_TXE));   //发送数据是否空
			USART_SendData(USART1,*sendbuf);
			while(!USART_GetFlagStatus(USART1, USART_FLAG_TC));			//是否发送完成
			sendbuf++;
		} 	
    for(timerCount = 0;timerCount < 100;timerCount++)
    {    
			;   
		}    	
		//PTGD_PTGD5 = 1;      
    return (TRUE) ;  
}

 



