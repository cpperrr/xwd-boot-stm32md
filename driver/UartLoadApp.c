//=======================================Copyright(c)===========================================
// 											Auto Code Making Company
//											Auto Code Department
//												CopyRight Private
//-------------------------------------------------FileMessage------------------------------------
// FileName	 CriticalZoom.c
// Author	 YHY
// Creation Time

// Description
//-----------------------------------------------��ǰ�汾�޶�----------------------------------
// �޸���
// ��	��
// �޸�����
// Description
//=============================================================================================

//=============================================================================================
// ͷ�ļ�
//=============================================================================================

#include "BSPTypeDef.h"
#include "UartLoadApp.h"
#include "BSPUART.h"
#include "BSPFLASH.h"

//=============================================================================================
// ȫ�ֱ�������
//=============================================================================================
extern void ResetPro(void);
extern void jump_app(void);
static u16 g_bootload_serialTime = 0;

u16 gCanLoadeReadBuf[e_CANLOAD_BUF_NUM] = {0};
u16 gUpdateLoadSecNum = 0; // �������ܶ���

/*-------------���ڽ��ռ����ʹ���Buf-------------*/
u8 gRxBuf[RX_BUF_LENGTH] = {0};
u16 gRxlength = 0;
u8 gRxFlag = 0;
u8 gTxBuf[TX_BUF_LENGTH] = {0};
static u16 appflash_update = 0xbbbb;

// CRC ѭ�������
const u8 auchCRCHi_exp[] = {
	0x00, 0xc1, 0x81, 0x40, 0x01, 0xc0, 0x80, 0x41, 0x01, 0xc0, 0x80, 0x41, 0x00, 0xc1, 0x81,
	0x40, 0x01, 0xc0, 0x80, 0x41, 0x00, 0xc1, 0x81, 0x40, 0x00, 0xc1, 0x81, 0x40, 0x01, 0xc0,
	0x80, 0x41, 0x01, 0xc0, 0x80, 0x41, 0x00, 0xc1, 0x81, 0x40, 0x00, 0xc1, 0x81, 0x40, 0x01,
	0xc0, 0x80, 0x41, 0x00, 0xc1, 0x81, 0x40, 0x01, 0xc0, 0x80, 0x41, 0x01, 0xc0, 0x80, 0x41,
	0x00, 0xc1, 0x81, 0x40, 0x01, 0xc0, 0x80, 0x41, 0x00, 0xc1, 0x81, 0x40, 0x00, 0xc1, 0x81,
	0x40, 0x01, 0xc0, 0x80, 0x41, 0x00, 0xc1, 0x81, 0x40, 0x01, 0xc0, 0x80, 0x41, 0x01, 0xc0,
	0x80, 0x41, 0x00, 0xc1, 0x81, 0x40, 0x00, 0xc1, 0x81, 0x40, 0x01, 0xc0, 0x80, 0x41, 0x01,
	0xc0, 0x80, 0x41, 0x00, 0xc1, 0x81, 0x40, 0x01, 0xc0, 0x80, 0x41, 0x00, 0xc1, 0x81, 0x40,
	0x00, 0xc1, 0x81, 0x40, 0x01, 0xc0, 0x80, 0x41, 0x01, 0xc0, 0x80, 0x41, 0x00, 0xc1, 0x81,
	0x40, 0x00, 0xc1, 0x81, 0x40, 0x01, 0xc0, 0x80, 0x41, 0x00, 0xc1, 0x81, 0x40, 0x01, 0xc0,
	0x80, 0x41, 0x01, 0xc0, 0x80, 0x41, 0x00, 0xc1, 0x81, 0x40, 0x00, 0xc1, 0x81, 0x40, 0x01,
	0xc0, 0x80, 0x41, 0x01, 0xc0, 0x80, 0x41, 0x00, 0xc1, 0x81, 0x40, 0x01, 0xc0, 0x80, 0x41,
	0x00, 0xc1, 0x81, 0x40, 0x00, 0xc1, 0x81, 0x40, 0x01, 0xc0, 0x80, 0x41, 0x00, 0xc1, 0x81,
	0x40, 0x01, 0xc0, 0x80, 0x41, 0x01, 0xc0, 0x80, 0x41, 0x00, 0xc1, 0x81, 0x40, 0x01, 0xc0,
	0x80, 0x41, 0x00, 0xc1, 0x81, 0x40, 0x00, 0xc1, 0x81, 0x40, 0x01, 0xc0, 0x80, 0x41, 0x01,
	0xc0, 0x80, 0x41, 0x00, 0xc1, 0x81, 0x40, 0x00, 0xc1, 0x81, 0x40, 0x01, 0xc0, 0x80, 0x41,
	0x00, 0xc1, 0x81, 0x40, 0x01, 0xc0, 0x80, 0x41, 0x01, 0xc0, 0x80, 0x41, 0x00, 0xc1, 0x81,
	0x40};

const u8 auchCRCLo_exp[] = {
	0x00, 0xc0, 0xc1, 0x01, 0xc3, 0x03, 0x02, 0xc2, 0xc6, 0x06, 0x07, 0xc7, 0x05, 0xc5, 0xc4,
	0x04, 0xcc, 0x0c, 0x0d, 0xcd, 0x0f, 0xcf, 0xce, 0x0e, 0x0a, 0xca, 0xcb, 0x0b, 0xc9, 0x09,
	0x08, 0xc8, 0xd8, 0x18, 0x19, 0xd9, 0x1b, 0xdb, 0xda, 0x1a, 0x1e, 0xde, 0xdf, 0x1f, 0xdd,
	0x1d, 0x1c, 0xdc, 0x14, 0xd4, 0xd5, 0x15, 0xd7, 0x17, 0x16, 0xd6, 0xd2, 0x12, 0x13, 0xd3,
	0x11, 0xd1, 0xd0, 0x10, 0xf0, 0x30, 0x31, 0xf1, 0x33, 0xf3, 0xf2, 0x32, 0x36, 0xf6, 0xf7,
	0x37, 0xf5, 0x35, 0x34, 0xf4, 0x3c, 0xfc, 0xfd, 0x3d, 0xff, 0x3f, 0x3e, 0xfe, 0xfa, 0x3a,
	0x3b, 0xfb, 0x39, 0xf9, 0xf8, 0x38, 0x28, 0xe8, 0xe9, 0x29, 0xeb, 0x2b, 0x2a, 0xea, 0xee,
	0x2e, 0x2f, 0xef, 0x2d, 0xed, 0xec, 0x2c, 0xe4, 0x24, 0x25, 0xe5, 0x27, 0xe7, 0xe6, 0x26,
	0x22, 0xe2, 0xe3, 0x23, 0xe1, 0x21, 0x20, 0xe0, 0xa0, 0x60, 0x61, 0xa1, 0x63, 0xa3, 0xa2,
	0x62, 0x66, 0xa6, 0xa7, 0x67, 0xa5, 0x65, 0x64, 0xa4, 0x6c, 0xac, 0xad, 0x6d, 0xaf, 0x6f,
	0x6e, 0xae, 0xaa, 0x6a, 0x6b, 0xab, 0x69, 0xa9, 0xa8, 0x68, 0x78, 0xb8, 0xb9, 0x79, 0xbb,
	0x7b, 0x7a, 0xba, 0xbe, 0x7e, 0x7f, 0xbf, 0x7d, 0xbd, 0xbc, 0x7c, 0xb4, 0x74, 0x75, 0xb5,
	0x77, 0xb7, 0xb6, 0x76, 0x72, 0xb2, 0xb3, 0x73, 0xb1, 0x71, 0x70, 0xb0, 0x50, 0x90, 0x91,
	0x51, 0x93, 0x53, 0x52, 0x92, 0x96, 0x56, 0x57, 0x97, 0x55, 0x95, 0x94, 0x54, 0x9c, 0x5c,
	0x5d, 0x9d, 0x5f, 0x9f, 0x9e, 0x5e, 0x5a, 0x9a, 0x9b, 0x5b, 0x99, 0x59, 0x58, 0x98, 0x88,
	0x48, 0x49, 0x89, 0x4b, 0x8b, 0x8a, 0x4a, 0x4e, 0x8e, 0x8f, 0x4f, 0x8d, 0x4d, 0x4c, 0x8c,
	0x44, 0x84, 0x85, 0x45, 0x87, 0x47, 0x46, 0x86, 0x82, 0x42, 0x43, 0x83, 0x41, 0x81, 0x80,
	0x40};

//=============================================================================================
// ��̬��������
//=============================================================================================
//=======================================================================================

//=======================================================================================
// ��������	: static void ResetPro(void)
// �������	: void
// �������	: void
// ��̬����	: void
// ��	��	: �����λ
// ע	��	:
//=======================================================================================
void ResetPro()
{
	NVIC_SystemReset();
}
//=============================================================================================
// ��������: static u8 IsCommandIntoLoadReady(void)
// �������:
// �������:
// ����ֵ  :
// ��������: �ж��Ƿ��ܹ����ճ�����¼ָ��
// ע������: void
//=============================================================================================
static u8 IsCommandIntoLoadReady(void);

//=============================================================================================
// ��������: static u8 IsCommandStartLoadReady(void)
// �������:
// �������:
// ����ֵ  :
// ��������: �Ƿ��ܹ��������ݴ���ָ��
// ע������: void
//=============================================================================================
static u8 IsCommandStartLoadReady(void);

//=============================================================================================
// ��������: static u8 IsCommandCompleteLoadReady(void)
// �������:
// �������:
// ����ֵ  :
// ��������: ��ѯ��¼�Ƿ������
// ע������: void
//=============================================================================================
static u8 IsCommandCompleteLoadReady(void);

//=============================================================================================
// ��������: static void UartRxMsgDeal(void)
// �������:
// �������:
// ����ֵ  :
// ��������: ���ڽ�����Ϣ����
// ע������: void
//=============================================================================================
static void Command_Hook(u16 Cmd, u16 pdata);

//=============================================================================================
// ��������: static void UartRxMsgDeal(void)
// �������:
// �������:
// ����ֵ  :
// ��������: ���ڽ�����Ϣ����
// ע������: void
//=============================================================================================
static void ResponseToPC(void);

//=============================================================================================
// ��������: static void UartRxMsgDeal(void)
// �������:
// �������:
// ����ֵ  :
// ��������: ���ڽ�����Ϣ����
// ע������: void
//=============================================================================================
static u8 UartRxMsgDeal(void);

//=============================================================================================
// ����ӿں���
//=============================================================================================

//===========================================================================
// ��������	: u16 MbRtuCheckCode(u8 *chkbuf, u8 len)
// �������	: chkbuf: ָ����Ҫ��֤��ModBus��Ϣ֡��ָ��
//			: len   : ҪЧ���Modbus��Ϣ֡�ֽ�����������Ч����CRC
// �������	: UINT16: Ч����CRC
// ��̬����	:
// ��	��	: ���㲢����ModBus RTUģʽ����Ϣ��CRCЧ����
// ע	��	:
//===========================================================================
static u16 CacleCRC(u8 *chkbuf, u16 len)
{
	u8 uchCRCHi = 0xff; // CRC���ֽڳ�ʼ��
	u8 uchCRCLo = 0xff; // CRC���ֽڳ�ʼ��
	u16 uIndex;			// ��ѯ������
	u16 temp_code;

	while (len)
	{
		uIndex = (u16)(uchCRCHi ^ *chkbuf++);
		uchCRCHi = (u8)(uchCRCLo ^ auchCRCHi_exp[uIndex]);
		uchCRCLo = auchCRCLo_exp[uIndex];
		len--;
	}

	temp_code = (u16)uchCRCHi;
	temp_code = (u16)(temp_code << 8);

	return (u16)(temp_code | uchCRCLo);
}

//=======================================================================================
// ��������	: void InitLoadParameter(void)
// �������	: void
// �������	: void
// ��̬����	: void
// ��	��	:     ��ʼ��Load��¼����
// ע	��	:
//=======================================================================================
void InitLoadParameter(void)
{
	//  u16 flashCRC = 0;
	u16 flashProflag = 0;
	u16 data[2] = {0, 0};
	/*---------------------------��ȡFLASH�е�Ӧ�ó����ʶ---------------------------*/
	// FlashReadByte(FLASH_END_ADDR + e_BOOT_CRC_ADDR,data,4);

	//	STMFLASH_Write(FLASH_END_ADDR+e_BOOT_CRC_ADDR+2,&appflash_update,1);

	data[0] = STMFLASH_ReadHalfWord(FLASH_END_ADDR + e_BOOT_CRC_ADDR);
	data[1] = STMFLASH_ReadHalfWord(FLASH_END_ADDR + e_BOOT_CRC_ADDR + 2);
	// flashCRC = (u16)data[0] | ((u16)data[1] << 8);
	flashProflag = data[0] << 16 | data[1];

	gCanLoadeReadBuf[e_DEVICE_TYPE] = 0xfe;
	gCanLoadeReadBuf[e_DEVICE_NUM] = 0xfe;

	gCanLoadeReadBuf[e_BOOT_VERSION] = BOOT_VERSION;
	gCanLoadeReadBuf[e_SOFT_VERSION] = 0;

	gCanLoadeReadBuf[e_PRO_TYPE] = PROTUES_TYPE;	  // Э������,MODBUSЭ��
	gCanLoadeReadBuf[e_CHIP_TYPE] = e_FREESCALE_DZ60; // оƬ����
	gCanLoadeReadBuf[e_LOADING_NUM] = 0;			  // ������д�Ķκ�
	gCanLoadeReadBuf[e_RUN_STATE] = e_TRANSMIT_MODEL;
	gCanLoadeReadBuf[e_CANLOAD_STATE] = e_LOADTRANSMIT_STATE;

	// ����CRCУ�飬������������
	if (flashProflag == BOOT_PRO_FLAG)
	{
		jump_app(); // ֱ����ת��APP
	}
	/***********
			if(APPUPDATE_FLAG==STMFLASH_ReadHalfWord(APP_CONFIG_ADDR))						//��ѯ�Ƿ����Ӧ�ó����ʶ
			{
				jump_app();//ֱ����ת��APP
			}
		**************/
}

void uart_irq_hook()
{
	// ���ܵ����ݱ�־λ
	if (USART_GetFlagStatus(USART1, USART_FLAG_RXNE))
	{
		g_bootload_serialTime = 10;
		// ��RDRE
		USART_ClearFlag(USART1, USART_FLAG_RXNE);
		gRxBuf[gRxlength] = USART_ReceiveData(USART1);
		gRxlength++;

		if (gRxlength >= RX_BUF_LENGTH)
		{
			gRxlength = 0;
		}
		gRxFlag = 1;
	}
	//	else
	//	{
	//		g_bootload_serialTime++;
	//	}
}
void sysTickSerialTimeHook()
{

	if (g_bootload_serialTime > 0)
		g_bootload_serialTime--;
}
//=============================================================================
// ��������: void UartRxCheckTask(void)
// �������:
// �������:
// ����ֵ  :
// ��������: �����Ƿ�������һ֡���ݼ��
// ע������: void
//=============================================================================
void UartCanLoaderTask(void)
{
	static u16 crc = 0XFF;
	static u16 RxCrc = 0XAA;

	if ((gRxFlag == 1) && (g_bootload_serialTime == 0)) // ��ʾ�������һ֡����
	{
		gRxFlag = 0;
		g_bootload_serialTime = 0;
		// CRC���
		if (gRxlength > 2)
		{
			crc = CacleCRC(gRxBuf, (gRxlength - 2));

			RxCrc = gRxBuf[gRxlength - 2];
			RxCrc = (RxCrc << 8) + gRxBuf[gRxlength - 1];
		}

		if (crc == RxCrc)
		{
			(void)UartRxMsgDeal();
		}
		crc = 0XFF;
		RxCrc = 0XAA;
		gRxlength = 0;
	}
}

//=============================================================================================
// ��������: static u8 IsCommandIntoLoadReady(void)
// �������:
// �������:
// ����ֵ  :
// ��������: �ж��Ƿ��ܹ����ճ�����¼ָ��
// ע������: void
//=============================================================================================
static u8 IsCommandIntoLoadReady(void)
{
	// ����ת��ģʽ
	if (gCanLoadeReadBuf[e_RUN_STATE] == e_TRANSMIT_MODEL)
	{
		return (TRUE);
	}

	if (gCanLoadeReadBuf[e_RUN_STATE] == e_CANLOAD_MODEL && gCanLoadeReadBuf[e_CANLOAD_STATE] == e_CANLOADREADY_OK)
	{
		return (TRUE);
	}

	if (gCanLoadeReadBuf[e_RUN_STATE] == e_CANLOAD_MODEL && gCanLoadeReadBuf[e_CANLOAD_STATE] == e_CANLOADREADY_FAIL)
	{
		return (TRUE);
	}

	return (FALSE);
}
//=============================================================================================
// ��������: static u8 IsCommandStartLoadReady(void)
// �������:
// �������:
// ����ֵ  :
// ��������: �Ƿ��ܹ��������ݴ���ָ��
// ע������: void
//=============================================================================================
static u8 IsCommandStartLoadReady(void)
{
	// ��������ڳ�����¼ģʽ���򲻽��봦��
	if (gCanLoadeReadBuf[e_RUN_STATE] != e_CANLOAD_MODEL)
	{
		return (FALSE);
	}

	if ((gCanLoadeReadBuf[e_CANLOAD_STATE] == e_CANLOADREADY_OK) || (gCanLoadeReadBuf[e_CANLOAD_STATE] == e_CANLOADTRANSMITING) || (gCanLoadeReadBuf[e_CANLOAD_STATE] == e_CANLOADTRANSMIT_FAIL))
	{
		return (TRUE);
	}

	return (FALSE);
}

//=============================================================================================
// ��������: static u8 IsCommandCompleteLoadReady(void)
// �������:
// �������:
// ����ֵ  :
// ��������: ��ѯ��¼�Ƿ������
// ע������: void
//=============================================================================================
static u8 IsCommandCompleteLoadReady(void)
{
	u16 crc = 0;
	u16 data[2] = {0, 0}; // ����
	// ��������ڳ�����¼ģʽ���򲻽��봦��
	if (gCanLoadeReadBuf[e_RUN_STATE] != e_CANLOAD_MODEL)
	{
		return (FALSE);
	}

	if ((gCanLoadeReadBuf[e_CANLOAD_STATE] != e_CANLOADTRANSMITING))
	{
		return (FALSE);
	}

	if (gUpdateLoadSecNum != gCanLoadeReadBuf[e_LOADING_NUM])
	{
		return (FALSE);
	}
	//   (void)FlashErase(0xD600,1);
	//  (void)FlashWriteByte(0xD600 + e_BOOT_CRC_ADDR,data,4);

	data[0] = crc;
	data[1] = BOOT_PRO_FLAG;

	//   (void)FlashErase(0xD600,1);
	(void)STMFLASH_Write(FLASH_END_ADDR + e_BOOT_CRC_ADDR, data, 2);

	//	STMFLASH_Write(APP_CONFIG_ADDR,&appflash_update,1);

	return (TRUE);
}
//=============================================================================================
// ��������: static void UartRxMsgDeal(void)
// �������:
// �������:
// ����ֵ  :
// ��������: ���ڽ�����Ϣ����
// ע������: void
//=============================================================================================
static void Command_Hook(u16 Cmd, u16 pdata)
{
	switch (Cmd)
	{
	case e_Cmd_IntoTransmit: // �������ת��״̬

		/*���������¼ģʽ���߱����Ѵ���ת��ģʽ����ִ�������¼ģʽʱ��Ҫ���յ��˳���¼
		�����ٽ��뵽ת��ģʽ*/
		if ((gCanLoadeReadBuf[e_RUN_STATE] == e_TRANSMIT_MODEL) || (gCanLoadeReadBuf[e_RUN_STATE] == e_CANLOAD_MODEL))
		{
			return;
		}
		gCanLoadeReadBuf[e_LOADING_NUM] = 0;
		gCanLoadeReadBuf[e_RUN_STATE] = e_TRANSMIT_MODEL;		  // ����ת��ģʽ
		gCanLoadeReadBuf[e_CANLOAD_STATE] = e_LOADTRANSMIT_STATE; // ��Ӧ�ó���ת��״̬

		break;
	case e_Cmd_ExitTransmit: // �����˳�ת��״̬
		if (gCanLoadeReadBuf[e_RUN_STATE] != e_TRANSMIT_MODEL)
		{
			return;
		}
		gCanLoadeReadBuf[e_LOADING_NUM] = 0;
		ResetPro();
		break;
	case e_Cmd_Reset: // ����λ
		ResetPro();
		break;
	case e_Cmd_IntoLoad: // ���������¼״̬
		// ���ϵͳ����ת��״̬�����͵�ָ���а�������¼���ܶγ�������Ӧ����ָ��
		if (IsCommandIntoLoadReady() == TRUE && pdata > 0)
		{
			if (FlashErase(FLASH_APP1_ADDR, 100) == 0) // ���FLASH�ɹ�
			{
				gCanLoadeReadBuf[e_RUN_STATE] = e_CANLOAD_MODEL;	   // ���������¼ģʽ
				gCanLoadeReadBuf[e_CANLOAD_STATE] = e_CANLOADREADY_OK; // ��¼׼��OK
				gCanLoadeReadBuf[e_LOADING_NUM] = 0;
				gUpdateLoadSecNum = pdata;
			}
			else
			{
				gCanLoadeReadBuf[e_RUN_STATE] = e_CANLOAD_MODEL;		 // ���������¼ģʽ
				gCanLoadeReadBuf[e_CANLOAD_STATE] = e_CANLOADREADY_FAIL; // ��¼׼��ʧ��
			}
		}
		break;
	case e_Cmd_ExitLoad: // �����˳���¼״̬
		if (gCanLoadeReadBuf[e_RUN_STATE] == e_CANLOAD_MODEL)
		{
			gCanLoadeReadBuf[e_LOADING_NUM] = 0;
			gCanLoadeReadBuf[e_RUN_STATE] = e_TRANSMIT_MODEL;
			gCanLoadeReadBuf[e_CANLOAD_STATE] = e_LOADTRANSMIT_STATE; // ��Ӧ�ó���ת��״̬
		}

		break;
	case e_Cmd_FinishLoad: // �����¼

		if (IsCommandCompleteLoadReady() == TRUE)
		{
			gCanLoadeReadBuf[e_LOADING_NUM] = 0;
			gCanLoadeReadBuf[e_CANLOAD_STATE] = e_CANLOADFLASH_OK;
		}

		break;
	default:
		break;
	}
}
//=============================================================================================
// ��������: static void UartRxMsgDeal(void)
// �������:
// �������:
// ����ֵ  :
// ��������: ���ڽ�����Ϣ����
// ע������: void
//=============================================================================================
static void ResponseToPC(void)
{
	u16 txlength = 0;
	u16 i = 0;
	u16 crc = 0;

	gTxBuf[txlength++] = ADDR_SELF; // ��վ��ַ
	gTxBuf[txlength++] = READ_CODE; // ������
	gTxBuf[txlength++] = 18;		// ����Byte

	for (i = 0; i < 9; i++)
	{
		gTxBuf[txlength++] = (u8)((gCanLoadeReadBuf[i] >> 8) & 0xff);
		gTxBuf[txlength++] = (u8)(gCanLoadeReadBuf[i] & 0xff);
	}

	crc = CacleCRC(gTxBuf, txlength);
	gTxBuf[txlength++] = (u8)((crc >> 8) & 0xff);
	gTxBuf[txlength++] = (u8)(crc & 0xff);

	(void)SCISendDataOnISR(gTxBuf, txlength);
}
//=============================================================================================
// ��������: static void UartRxMsgDeal(void)
// �������:
// �������:
// ����ֵ  :
// ��������: ���ڽ�����Ϣ����
// ע������: void
//=============================================================================================
static u8 UartRxMsgDeal(void)
{
	u8 slave = 0;	// ��վ��ַ
	u8 funcode = 0; // ������
	u16 addr = 0;	// ��ʼ��ַ
	u16 length = 0; // ����
					//  u16 limitlength = 0;    //�����Գ���
	u8 i = 0;
	// u16 startIndex = 0;
	// u16 crc = 0;
	// u16 txlength = 0;
	u16 CommandBuf[2] = {0, 0};		   // ����ָ��
	static u16 ReadytoUploadNum = 0;   // ׼����¼�Ķκ�
	static u32 ReadytoUploadAddr = 0;  // ׼����¼����ʼ��ַ
	static u16 ReadytoUploadlenth = 0; // ׼����¼�ĳ���
	u8 flashResult = 0;

	slave = gRxBuf[0];	 // ��վ��ַ
	funcode = gRxBuf[1]; // ������

	if (slave != ADDR_SELF && slave != 0xff) // Ŀ���ַ��0xff����Ϊ�㲥��ַ������д���
	{
		return FALSE;
	}

	switch (funcode)
	{
	case READ_CODE: // ��������
		addr = (u16)gRxBuf[3] | ((u16)gRxBuf[2] << 8);
		length = (u16)gRxBuf[5] | ((u16)gRxBuf[4] << 8);

		// ��ȡ��ַ�Ϸ�
		if (addr == MODBUS_BOOTStart_ADDR && length == MODBUS_BOOTSTATE_LEN)
		{
			ResponseToPC();
			return (TRUE);
		}

		break;
	case WRITE_CODE: // д������

		addr = (u16)gRxBuf[3] | ((u16)gRxBuf[2] << 8);
		length = (u16)gRxBuf[5] | ((u16)gRxBuf[4] << 8);

		if (addr == MODBUS_CMD_ADDR && length == MODBUS_CMD_LEN) // ����ָ���󳤶�Ϊ2
		{
			for (i = 0; i < length; i++)
			{
				CommandBuf[i] = ((u16)gRxBuf[7 + i * 2] << 8) | ((u16)gRxBuf[8 + i * 2]);
			}

			Command_Hook(CommandBuf[0], CommandBuf[1]);
			ResponseToPC();
			return (TRUE);
		}
		else if (addr == MODBUS_LOAD_ADDR) // ����ָ��
		{
			if (IsCommandStartLoadReady() == TRUE)
			{
				ReadytoUploadNum = ((u16)gRxBuf[7] << 8) | ((u16)gRxBuf[8]); // ׼����¼�Ķκ�
				ReadytoUploadAddr = ((u32)gRxBuf[9] << 24) | ((u32)gRxBuf[10] << 16) | ((u32)gRxBuf[11] << 8) | ((u32)gRxBuf[12]);
				ReadytoUploadlenth = ((u16)gRxBuf[13] << 8) | ((u16)gRxBuf[14]);

				if ((ReadytoUploadNum != gCanLoadeReadBuf[e_LOADING_NUM] + 1) || (ReadytoUploadNum > gUpdateLoadSecNum))
				{
					gCanLoadeReadBuf[e_CANLOAD_STATE] = e_CANLOADTRANSMIT_FAIL;
				}
				else
				{
					/***
						if(ReadytoUploadAddr >= 0xFFC0)    //�ж�������ַ�洢����ת�Ƶ�F9C0->F9FF֮��
					{
						 flashResult = FlashWriteByte(ReadytoUploadAddr -0x600,&gRxBuf[15],ReadytoUploadlenth);
					}
						****/
					if (ReadytoUploadAddr + ReadytoUploadlenth >= 0x08020000)
					{
						flashResult = 0;
					}
					else
					{
						flashResult = FlashWriteSection(ReadytoUploadAddr, &gRxBuf[15], ReadytoUploadlenth);
					}

					if (flashResult == 1)
					{
						gCanLoadeReadBuf[e_LOADING_NUM]++;
						gCanLoadeReadBuf[e_CANLOAD_STATE] = e_CANLOADTRANSMITING;
					}
					else
					{
						gCanLoadeReadBuf[e_CANLOAD_STATE] = e_CANLOADFLASH_FAIL;
					}
				}

				ResponseToPC();
				return (TRUE);
			}
		}

		break;
	default:
		break;
	}

	return (FALSE);
}

//-----------------------------------------File END--------------------------------------------
