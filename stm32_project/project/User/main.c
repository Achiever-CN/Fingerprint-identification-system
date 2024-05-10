#include "public_include.h"

#include "ctl_as608.h"
#include "usart2.h"
#include "usart3.h"
#include "usart1.h"
#include "menu.h"
#include "syn6288.h"

u8 res = 0x01;
u8 SYN_StopCom[] = {0xFD, 0X00, 0X02, 0X02, 0XFD};	// ֹͣ�ϳ�
u8 SYN_SuspendCom[] = {0XFD, 0X00, 0X02, 0X03, 0XFC};	// ��ͣ�ϳ�
u8 SYN_RecoverCom[] = {0XFD, 0X00, 0X02, 0X04, 0XFB};	// �ָ��ϳ�
u8 SYN_ChackCom[] = {0XFD, 0X00, 0X02, 0X21, 0XDE};	// ״̬��ѯ
u8 SYN_PowerDownCom[] = {0XFD, 0X00, 0X02, 0X88, 0X77}; // ����POWER DOWNģʽ

int main(void)
{

	enum Key_value key = UNKONE;
	u8 select_id = 1;

	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2); // ����ϵͳ�ж����ȼ�����2

	OLED_Init();
	Key_Init();
	Serial_Init();
	delay_init();
	usart2_init(57600);
	USART3_Init(9600);

	// while(1)
	// {
	// 	// ��ڲ����� background:��������ѡ��volumeLevel:�����ȼ���backgroundLevel:�������������ȼ���speechRate:���ٵȼ���text:�����ı�
	// 	SYN_Play(0, 15, 0, 5, "ȫ������Ҳ���Է���"); // �ϳ�����
	// 	delay_ms(1500);
	// 	delay_ms(1500);
	// 	delay_ms(1500);
	// 	delay_ms(1500);
	// 	delay_ms(1500);
	// 	delay_ms(1500);
	// 	delay_ms(1500);

	// }

	// ����ָ��ģ��
	while (connect_FR() != COMPLETED)
	{
	}

	// ��ȡָ�Ƹ���
	while (get_Fingerprint_Count(&id) != COMPLETED)
	{
		delay_and_display("Get fingerprint count failed", 3000);
	}

	while(connect_PC() != COMPLETED)
	{
	}
	

	show_select(select_id);

	SYN_DefaultPlay("��ӭʹ��ָ��ʶ��ϵͳ");
	// ��ȡ��ť�¼�

	while (1)
	{

		key = Key_GetNum();
		switch (key)
		{
		case PLUS:
			select_id = select_id == 4 ? 4 : select_id + 1;
			show_select(select_id);
			break;
		case MINUS:
			select_id = select_id == 1 ? 1 : select_id - 1;
			show_select(select_id);
			break;
		case ACK:
			// ��ѡ����д���
			handle_functon(select_id);
			break;
		case UNKONE:
			break;
		}

		if (Serial_GetRxFlag())
		{
			check_time();
		}
	}
}
