#include "public_include.h"

#include "ctl_as608.h"
#include "usart2.h"
#include "usart3.h"
#include "usart1.h"
#include "menu.h"
#include "syn6288.h"

u8 res = 0x01;
u8 SYN_StopCom[] = {0xFD, 0X00, 0X02, 0X02, 0XFD};	// 停止合成
u8 SYN_SuspendCom[] = {0XFD, 0X00, 0X02, 0X03, 0XFC};	// 暂停合成
u8 SYN_RecoverCom[] = {0XFD, 0X00, 0X02, 0X04, 0XFB};	// 恢复合成
u8 SYN_ChackCom[] = {0XFD, 0X00, 0X02, 0X21, 0XDE};	// 状态查询
u8 SYN_PowerDownCom[] = {0XFD, 0X00, 0X02, 0X88, 0X77}; // 进入POWER DOWN模式

int main(void)
{

	enum Key_value key = UNKONE;
	u8 select_id = 1;

	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2); // 设置系统中断优先级分组2

	OLED_Init();
	Key_Init();
	Serial_Init();
	delay_init();
	usart2_init(57600);
	USART3_Init(9600);

	// while(1)
	// {
	// 	// 入口参数： background:背景音乐选择，volumeLevel:音量等级，backgroundLevel:背景音乐音量等级，speechRate:语速等级，text:播报文本
	// 	SYN_Play(0, 15, 0, 5, "全世界我也可以放弃"); // 合成语音
	// 	delay_ms(1500);
	// 	delay_ms(1500);
	// 	delay_ms(1500);
	// 	delay_ms(1500);
	// 	delay_ms(1500);
	// 	delay_ms(1500);
	// 	delay_ms(1500);

	// }

	// 连接指纹模块
	while (connect_FR() != COMPLETED)
	{
	}

	// 获取指纹个数
	while (get_Fingerprint_Count(&id) != COMPLETED)
	{
		delay_and_display("Get fingerprint count failed", 3000);
	}

	while(connect_PC() != COMPLETED)
	{
	}
	

	show_select(select_id);

	SYN_DefaultPlay("欢迎使用指纹识别系统");
	// 读取按钮事件

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
			// 对选择进行处理
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
