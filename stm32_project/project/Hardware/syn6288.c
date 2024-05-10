#include "syn6288.h"
#include "usart3.h"
#include "string.h"
#include "delay.h"
#include "stdio.h"

// Music:ѡ�񱳾����֡�0:�ޱ������֣�1~15��ѡ�񱳾�����
void SYN_FrameInfo(u8 Music, u8 *HZdata)
{
        /****************��Ҫ���͵��ı�**********************************/
        unsigned char Frame_Info[50];
        unsigned char HZ_Length;
        unsigned char ecc = 0; // ����У���ֽ�
        unsigned int i = 0;
        HZ_Length = strlen((char *)HZdata); // ��Ҫ�����ı��ĳ���

        /*****************֡�̶�������Ϣ**************************************/
        Frame_Info[0] = 0xFD;              // ����֡ͷFD
        Frame_Info[1] = 0x00;              // �������������ȵĸ��ֽ�
        Frame_Info[2] = HZ_Length + 3;     // �������������ȵĵ��ֽ�
        Frame_Info[3] = 0x01;              // ���������֣��ϳɲ�������
        Frame_Info[4] = 0x01 | Music << 4; // ����������������������趨

        /*******************У�������***************************************/
        for (i = 0; i < 5; i++) // ���η��͹���õ�5��֡ͷ�ֽ�
        {
                ecc = ecc ^ (Frame_Info[i]); // �Է��͵��ֽڽ������У��
        }

        for (i = 0; i < HZ_Length; i++) // ���η��ʹ��ϳɵ��ı�����
        {
                ecc = ecc ^ (HZdata[i]); // �Է��͵��ֽڽ������У��
        }
        /*******************����֡��Ϣ***************************************/
        memcpy(&Frame_Info[5], HZdata, HZ_Length);
        Frame_Info[5 + HZ_Length] = ecc;
        USART3_SendString(Frame_Info, 5 + HZ_Length + 1);
}

/***********************************************************
 * ��    �ƣ� YS_SYN_Set(u8 *Info_data)
 * ��    �ܣ� ������	�������
 * ��ڲ����� *Info_data:�̶���������Ϣ����
 * ���ڲ�����
 * ˵    �����������������ã�ֹͣ�ϳɡ���ͣ�ϳɵ����� ��Ĭ�ϲ�����9600bps��
 * ���÷�����ͨ�������Ѿ�������������������á�
 **********************************************************/
void YS_SYN_Set(u8 *Info_data)
{
        u8 Com_Len;
        Com_Len = strlen((char *)Info_data);
        USART3_SendString(Info_data, Com_Len);
}

/***********************************************************
 * ��    �ƣ� SYN_Play(u8 background, u8 volumeLevel, u8 backgroundLevel, u8 speechRate, char *text)
 * ��    �ܣ� ������������
 * ��ڲ����� background:��������ѡ��volumeLevel:�����ȼ���backgroundLevel:�������������ȼ���speechRate:���ٵȼ���text:�����ı�
 * ���ڲ�����
 **********************************************************/
void SYN_Play(uint8_t background, uint8_t volumeLevel, uint8_t backgroundLevel, uint8_t speechRate, char *text)
{
        char str[100];
        sprintf(str, "[v%d][m%d][t%d]%s", volumeLevel, backgroundLevel, speechRate, text);
        SYN_FrameInfo(background, (u8 *)str);
}

void SYN_DefaultPlay(char *text)
{
        SYN_Play(0, 15, 0, 5, text);
}
