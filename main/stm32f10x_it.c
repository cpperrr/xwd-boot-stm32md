
#include "stm32f10x_it.h"


void NMI_Handler(void)
{
}


void HardFault_Handler(void)
{
  /* Go to infinite loop when Hard Fault exception occurs */
  while (1)
  {
  }
}


void MemManage_Handler(void)
{
  /* Go to infinite loop when Memory Manage exception occurs */
  while (1)
  {
  }
}


void BusFault_Handler(void)
{
  /* Go to infinite loop when Bus Fault exception occurs */
  while (1)
  {
  }
}


void UsageFault_Handler(void)
{
  /* Go to infinite loop when Usage Fault exception occurs */
  while (1)
  {
  }
}


void SVC_Handler(void)
{
}


void DebugMon_Handler(void)
{
}

//ʹ�ò���ϵͳ,ע�͵�����
//void PendSV_Handler(void)
//{
//}

extern void sysTickSerialTimeHook();

void SysTick_Handler(void)
{sysTickSerialTimeHook();
}
extern void uart_irq_hook();
void USART1_IRQHandler()
{
uart_irq_hook();
}