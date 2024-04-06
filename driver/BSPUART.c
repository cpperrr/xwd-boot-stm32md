#include "BSPUART.h"
#include "stm32f10x.h"


extern   void NVIC_Configuration(void);
//=============================================================================================
//�������� void Debug_Serial_Init(u32 baud)
//������� u32 baud
//������� void
//��̬���� void
//��    �� ��ʼ��SCIģ�� 
//ע    ��  
//=============================================================================================
void SCIInit(u32 baud)
{
//	u32 NVICtemp    = 0;       			 	//����NVIC�������ı���
	GPIO_InitTypeDef GPIO_InitStructure;	//gpio��ʼ������
	USART_InitTypeDef USART_InitStructure;	//���ڳ�ʼ���ṹ��
	//������Щ����һ�������ã����Է����ж�֮��
	USART_InitStructure.USART_BaudRate = baud;//������;
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;	 //8λ����λ
	USART_InitStructure.USART_StopBits = USART_StopBits_1;		 //1λֹͣλ
	USART_InitStructure.USART_Parity = USART_Parity_No;			 //��żУ��ʧ��
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;	//Ӳ������ʧ��
	USART_InitStructure.USART_Mode = USART_Mode_Tx | USART_Mode_Rx;				   //��������ն�ʹ��

	//ʹ�ܴ�������GPIOģ��ʱ��,uartģ��ʱ��,��ʹ��AFIOģ��ʱ�� ,uart1λ��apb2
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA|RCC_APB2Periph_AFIO|RCC_APB2Periph_USART1, ENABLE);
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	//ioģ������pa9
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;		//����TX����
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;		//�����������
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	//pa10
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;		//����RX����
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;		//��������
	GPIO_Init(GPIOA, &GPIO_InitStructure);		//io�������

	//�ж����ȼ���������,��sys�ļ������õĺ궨��
// 	NVICtemp = NVIC_EncodePriority(BOARD_NVIC_GROUP , UART1_PreemptionPriority , UART1_SubPriority);    
//	 NVIC_SetPriority(USART1_IRQn,NVICtemp);          //�����ж����ȼ�
	 
	 
	USART_Init(USART1, &USART_InitStructure);		//��ʼ��USART
	 USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);	 //�����ж�ʹ��
	 NVIC_EnableIRQ(USART1_IRQn);                     //ʹ�ܶ�Ӧ�ж� 
	USART_Cmd(USART1, ENABLE);		//ʹ�ܴ���ģ��	
	USART_GetFlagStatus(USART1, USART_FLAG_TC);
}
 
  void NVIC_Configuration(void)
{
  NVIC_InitTypeDef NVIC_InitStructure;
  
  /* Ƕ�������жϿ�������ѡ�� */
	/* ��ʾ NVIC_PriorityGroupConfig() ����������ֻ��Ҫ����һ�����������ȼ�����*/
  NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
  
  /* ����USARTΪ�ж�Դ ��Ĭ��ʹ�ô���1��Ϊ�ж�Դ*/
  NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;
  /* �������ȼ�*/
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
  /* �����ȼ� */
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
  /* ʹ���ж� */
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  /* ��ʼ������NVIC */
  NVIC_Init(&NVIC_InitStructure);
}

// �����ж����ȼ�����
//	NVIC_Configuration();
//	
//	// ʹ�ܴ��ڽ����жϣ��������ݼĴ����ǿգ���ʾ���յ����ݾͲ����ж�
//	USART_ITConfig(DEBUG_USARTx, USART_IT_RXNE, ENABLE);

//=============================================================================================
//��������u8 SCISendDataOnISR(e_SCINUM SCINum,u8 cmd, u8 *sendbuf, u8 dir,u8 size)
//������� SCINum SCI·�� u8 data:����������  
//������� u8 0:�ɹ�  1�����ɹ�
//��̬���� void
//��    �� ���ʹ������� 
//ע    ��  
//=============================================================================================
u8 SCISendDataOnISR(u8 *sendbuf,u16 size)
{
		u8 timerCount = 0;    
		if(size == 0)
		{
				return FALSE;
		}	
		//��ֹ����
		//PTGD_PTGD5 = 0;     
    for(timerCount = 0;timerCount < 100;timerCount++)
    {
        ;
    }       	
		while(size--)
		{
			while(!USART_GetFlagStatus(USART1, USART_FLAG_TXE));   //���������Ƿ��
			USART_SendData(USART1,*sendbuf);
			while(!USART_GetFlagStatus(USART1, USART_FLAG_TC));			//�Ƿ������
			sendbuf++;
		} 	
    for(timerCount = 0;timerCount < 100;timerCount++)
    {    
			;   
		}    	
		//PTGD_PTGD5 = 1;      
    return (TRUE) ;  
}

 



