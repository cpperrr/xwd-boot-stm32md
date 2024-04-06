//=======================================Copyright(c)===========================================
// 											Auto Code Making Company
//											Auto Code Department
//												CopyRight Private
//-------------------------------------------------FileMessage------------------------------------
//FileName	 CriticalZoom.c 
//Author	 YHY
//Creation Time
	  
//Description
//-----------------------------------------------��ǰ�汾�޶�----------------------------------
//�޸���	
//��	��	 
//�޸�����	
//Description	
//=============================================================================================
#ifndef _UARTLOADAPP_H
#define _UARTLOADAPP_H
//=============================================================================================
//ͷ�ļ�
//=============================================================================================
 #include "stm32f10x.h"  
//=============================================================================================
//�궨��
//=============================================================================================


#define FLASH_APP1_ADDR		0x08002000  	//��һ��Ӧ�ó�����ʼ��ַ(�����FLASH)
#define APPUPDATE_FLAG    0x5aa5

#define FLASH_IAP_ADDR		0x08000000  	//iap������ʼ��ַ(�����FLASH)
		
#define APP_CONFIG_ADDR 		0X08001FFC	//���õ�ַ

#define PROTUES_TYPE                (0)         //0ModbusЭ��
#define MODBUS_BOOTStart_ADDR       (33)    //BOOT����¼��ʼ��ַ
#define MODBUS_BOOTSTATE_LEN        (9)
#define MODBUS_BOOTEnd_ADDR         (MODBUS_BOOTStart_ADDR + MODBUS_BOOTSTATE_LEN - 1)

#define MODBUS_CMD_ADDR             (MODBUS_BOOTStart_ADDR + MODBUS_BOOTSTATE_LEN)
#define MODBUS_CMD_LEN              (2)

#define MODBUS_LOAD_ADDR            (MODBUS_CMD_ADDR + MODBUS_CMD_LEN)
#define MODBUS_LOAD_LEN             (1028)


#define RX_BUF_LENGTH           (1200)
#define TX_BUF_LENGTH           (30)
#define SECTION_LENGTH          (1024)

//����BUF����
#define ADDR_SELF    			110        //UART2��ΪMODBUSͨѶ�Ĵӻ���ַ

//������
#define READ_CODE    			0x03        //��ָ��     
#define WRITE_SINGLE_CODE		0x06        //����дָ��
#define WRITE_CODE   			0x10        //���дָ��

#define  BOOT_VERSION               (0x300)            /*Boot����屾��*/
//#define  PRO_ENTRY_POINT     	    (0x197b)        	/*Ӧ�ó�����ڵ�ַ*/
//#define  BOOT_ENRTY_POINT    	    (0x10fb)        	/*Boot������ڵ�ַ*/
//#define  PROGRAME_FLASH_START       (0x1900)    		/*Ӧ�ó�����ʼ��ַ*/
//#define  PROGRAME_FLASH_END         (0xdbff)       	    /*Ӧ�ó��������ַ*/ 
 
//#define  BOOT_PRO_FLAG       		(0x5AA5)         	/*Ӧ�ó����־*/
//#define PROGRAME_FLASH_START  (0x1900)     /*Ӧ�ó�����ʼ��ַ*/
//#define PROGRAME_FLASH_END    (0xdbff)     /*Ӧ�ó��������ַ*/   

//#define PROGRAME_CRC_START    (0x1900)      /*Ӧ�ó�����ʼУ���ַ*/
//#define PROGRAME_CRC_END      (0x1D00)      /*Ӧ�ó������У���ַ*/  
//#define PROGRAME_CRC_LENGTH (PROGRAME_CRC_END - PROGRAME_CRC_START)     //У�鳤��

#define BOOT_VERSION        (0x300)         /*Boot����屾��*/
#define BOOT_TRANS_FLAG     (0x5AA5)        /*ת����־*/
#define BOOT_PRO_FLAG       (0x5AA5)        /*Ӧ�ó����־*/
#define BOOT_PRO_TRY_FLAG   (0x5AA5)        /*�Ѿ����Եı�־*/

//#define PRO_ENTRY_POINT     (0x197b)        /*Ӧ�ó�����ڵ�ַ*/
//#define BOOT_ENRTY_POINT    (0x10fb)        /*Boot������ڵ�ַ*/

//=============================================================================================
//������������
//=============================================================================================
typedef enum _BOOT_STATE_INFO
{
    e_PRO_TYPE     = 0,            //Э������
    e_DEVICE_TYPE  = 1,            //�豸����
    e_DEVICE_NUM   = 2,            //�豸��
    e_CHIP_TYPE    = 3,            //оƬ����
    e_SOFT_VERSION = 4,             //����汾��
    e_BOOT_VERSION = 5,             //BOOT�汾��
    e_CANLOAD_STATE = 6,            //��¼״̬
    e_RUN_STATE     = 7,            //����״̬
    e_LOADING_NUM   = 8,            //������д�Ķκ�
    e_CANLOAD_BUF_NUM = 9           //���ݶγ���
}E_BOOT_STATE;

typedef enum _MCU_TYPE_
{
    e_FREESCALE_DZ60 = 0,       //оƬ����
    e_FREESCALE_XEQ512 = 1,
    e_COMTEX_TM4C129ENDPT = 2,
}E_MCU_TYPE;

typedef enum _RUNMODEL_ENUM_
{
    e_TRANSMIT_MODEL = 0,    //ת��ģʽ
    e_CANLOAD_MODEL = 1,     //��¼ģʽ
    e_PROGRAME_MODEL = 2,    //Ӧ�ó�������ģʽ   
}E_RUNMODEL;

typedef enum _RUNMODEL_STATE_
{
    e_LOADTRANSMIT_STATE    = 0,            /*��Ӧ�ó���ת��״̬*/
    e_PROTRANSMIT_STATE     = 1,            /*Ӧ�ó���ת��״̬*/
    e_CANLOADREADY_OK       = 2,            /*��¼׼��*/
    e_CANLOADREADY_FAIL     = 3,            /*��¼׼��ʧ��*/
   // e_CANLOADPRE_OK         = 4,            /*��¼����*/
    e_CANLOADTRANSMITING    = 5,            /*���ڴ���*/   
    e_CANLOADTRANSMIT_FAIL  = 6,            /*����ʧ��*/
 //   e_CANLOADTRANSMIT_OK    = 7,            /*����ɹ�*/
    e_CANLOADFLASH_FAIL     = 8,            /*��¼ʧ��*/
    e_CANLOADFLASH_OK       = 9,             /*��¼�ɹ�*/
    e_PRORUN_STATE          = 10,           /*Ӧ�ó�������״̬*/
}E_RUNMODEL_STATE;

typedef enum _MESSAGE_TYPE_
{
    e_Cmd_IntoTransmit      = 0xE0,                 /*����ת��ģʽ*/
    e_Cmd_ExitTransmit      = 0xE1,                 /*�˳�ת��ģʽ*/
    e_Cmd_Reset             = 0xE2,                 /*����λ*/
    e_Cmd_IntoLoad          = 0xE3,                 /*������¼*/
    e_Cmd_ExitLoad          = 0xE4,                 /*�˳���¼*/
    e_Cmd_FinishLoad        = 0xE5,                 /*��¼���*/
}E_MESSAGE_TYPE;

typedef enum _BOOT_FLAG_ADDR_               /*��ʼ��ַΪ0xD600*/
{
    e_BOOT_TRANSMIT_FLAG_ADDR = 6,          /*ת��ģ־��ŵ�ַ*/
    e_BOOT_CRC_ADDR = 8,                    /*Ӧ�ó���CRC��ŵ�ַ*/
    e_BOOT_PRO_FLAG_ADDR = 10,              /*Ӧ�ó����־*/
    e_BOOT_PRO_TRY_FLAG_ADDR = 12,          /*�������б�־*/
    e_BOOT_PRO_VERSION_ADDR = 14,           /*Ӧ�ó���汾��*/ 
    e_BOOT_SOTE_VERION_ADDR = 16,           /*Boot����汾�Ŵ洢��ַ*/
    e_BOOT_NETECU_FLAG = 20,               /*�豸�ż��豸���ͱ�־��ŵ�ַ*/
		e_BOOT_NET_ADDR = 22,                  /*�豸���ʹ�ŵ�ַ*/
    e_BOOT_ECU_ADDR = 24,                  /*�豸�Ŵ�ŵ�ַ*/

}E_BOOT_FLAG_ADDR;

//=============================================================================================
//����ȫ�ֱ���
//=============================================================================================

//=============================================================================================
//��������
//=============================================================================================

//=======================================================================================
//��������	: void InitLoadParameter(void)
//�������	: void  
//�������	: void
//��̬����	: void
//��	��	:     ��ʼ��Load��¼����
//ע	��	:  
//=======================================================================================
void InitLoadParameter(void);

//=============================================================================
//��������: void UartRxCheckTask(void)
//�������: 
//�������:
//����ֵ  : 
//��������: �����Ƿ�������һ֡���ݼ��
//ע������: void
//=============================================================================
void UartCanLoaderTask(void);


#endif

//-----------------------------------------File END--------------------------------------------
