#ifndef __DEBUGSERIAL_H_
#define __DEBUGSERIAL_H_


#include "stdio.h"
#include "BSPTypeDef.h"
#include "stm32f10x.h"

//�����ж����ݻ�����
//���ڽ��ܻ�������󳤶�,��Ҫ����255
#define SERIAL_MAX_LENGTH	250	


//=============================================================================================
//�������� void SCIInit(e_SCINUM sciNum, u32 rate)
//������� SCINum SCI·�� uSCI_Config cfg:SCI���� 
//������� void
//��̬���� void
//��    �� ��ʼ��SCIģ�� 
//ע    ��  
//=============================================================================================
void SCIInit(u32 baud);

 

//=============================================================================================
//��������u8 SCISendDataOnISR(e_SCINUM SCINum,u8 cmd, u8 *sendbuf, u8 dir,u8 size)
//������� SCINum SCI·�� u8 data:����������  
//������� u8 0:�ɹ�  1�����ɹ�
//��̬���� void
//��    �� ���ʹ������� ����ѯģʽ��
//ע    ��  
//=============================================================================================
u8 SCISendDataOnISR(u8 *sendbuf,u16 size);


#endif










