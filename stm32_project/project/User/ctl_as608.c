#include "ctl_as608.h"
#include "string.h"
#include "menu.h"
#include "syn6288.h"

SysPara AS608Para;
u16 ValidN;
u16 id = 1;
char str[100];
u8 in_time = 1;

enum Steps
{
	PUT_FINGER,
	PUT_AGAIN,
	COMPARE,
	CREATE_MODEL,
	STORE_MODEL,
	QUIT_OUT
};

void delay_and_display(char *message, int delay_time)
{
	show_onemessage(1, 1, message);
	delay_ms(3000);
}


enum ResultCode connect_FR(void)
{
	while (PS_HandShake(&AS608Addr))
	{
		delay_and_display("Connect failed", 3000);
		return FAILURE;
	}
	delay_and_display("Connect success", 3000);
	return COMPLETED;
}

enum ResultCode connect_PC(void)
{
	u8 count = 0;
	Serial_SendString("connect");


	while (Serial_GetRxFlag() == 0)
	{
		count++;
		delay_and_display("Try Connect PC", 3000);
		if (count == 3)
		{
			delay_and_display("PC connect fail", 3000);
			return FAILURE;
		}
	}

	check_time();
	delay_and_display("Connect sucess", 3000);
	return COMPLETED;
}

enum ResultCode Add_FR(void)
{
	u8 times = 0;
	u8 ensure;
	u8 steps = PUT_FINGER;
	SearchResult seach;
	char *received_data;

	while (1)
	{
		switch (steps)
		{
		case PUT_FINGER:
			times++;
			delay_and_display("Put finger", 3000);

			ensure = PS_GetImage(); // 第一次放置手指

			if (ensure != 0x00)
			{
				delay_and_display("Read error", 3000);
				break;
			}

			delay_and_display("Creatting ...", 3000);
			ensure = PS_GenChar(CharBuffer1); // 生成特征
			if (ensure != 0x00)
			{
				delay_and_display("Finger error", 3000);
				return FAILURE;
			}

			ensure = PS_HighSpeedSearch(CharBuffer1, 0, 300, &seach);
			if (ensure == 0x00)
			{
				delay_and_display("Finger exist", 3000);
				times = 0;
				return FAILURE;
			}

			delay_and_display("Finger current", 3000);
			times = 0;
			steps = PUT_AGAIN;
			break;

		case PUT_AGAIN:

			times++;

			delay_and_display("Put again", 3000);

			ensure = PS_GetImage(); // 第二次放置手指

			if (ensure != 0x00)
			{
				delay_and_display("Read error", 3000);
				break;
			}

			delay_and_display("Get finger", 3000);

			ensure = PS_GenChar(CharBuffer2); // 生成特征
			if (ensure != 0x00)
			{
				delay_and_display("finger error", 3000);
				return FAILURE;
			}

			delay_and_display("Finger current", 3000);
			times = 0;
			steps = COMPARE; // 跳到第三步

			break;

		case COMPARE:
			delay_and_display("Compare ing...", 3000);

			ensure = PS_Match(); // 比对两次生成的特征
			if (ensure != 0x00)
			{
				delay_and_display("Different finger", 3000);
				times = 0;
				return FAILURE;
			}

			delay_and_display("Same figer", 3000);
			steps = CREATE_MODEL; // 跳到第四步
			delay_ms(1000);
			break;

		case CREATE_MODEL:
			delay_and_display("Creat model", 3000);

			ensure = PS_RegModel(); // 合成模板
			if (ensure != 0x00)
			{
				steps = QUIT_OUT;
				show_onemessage(1, 1, "Creat model error");
				return FAILURE;
			}

			steps = STORE_MODEL; // 跳到第五步
			delay_and_display("Creat success", 3000);
			break;

		case STORE_MODEL:

			// Serial_Printf("id = %d", id);
			ensure = PS_StoreChar(CharBuffer2, id++); // 储存模板

			if (ensure != 0x00)
			{
				steps = QUIT_OUT;
				delay_and_display("Save erro", 3000);
				return FAILURE;
			}
			// Serial_Printf("id = %d", id);
			
			delay_and_display("Save success", 3000);
			get_Fingerprint_Count(&ValidN); // 读库指纹个数
			sprintf(str, "Total is %d", ValidN);
			delay_and_display(str, 3000);

			Serial_Printf("%d", 1);

			if (check_received_data(received_data, "Down") == 1)
			{
				delay_and_display("Down", 3000);
				SYN_DefaultPlay("用户添加成功"); // 合成语音
			}
			else
			{
				delay_and_display("Wrong", 3000);
				SYN_DefaultPlay("用户添加失败"); // 合成语音
				PS_DeletChar(--id, 1);
				// Serial_Printf("id = %d", id);
			}

			steps = QUIT_OUT;
			break;

		case QUIT_OUT:
			return COMPLETED;
		}
		if (times == 5) // 超过5次没有按手指则退出
		{
			delay_and_display("Time out", 3000);
			return FAILURE;
		}
	}
}

enum ResultCode Press_FR(void)
{
	SearchResult seach;
	u8 ensure;
	char *received_data;
	u8 count = 0;

	char str_match[50];

	if (in_time == 0)
	{
		delay_and_display("Out of time", 3000);
		return FAILURE;
	}

	delay_and_display("Put finger", 3000);

	ensure = PS_GetImage();
	if (ensure != 0x00) // 获取图像成功
		return FAILURE;
	delay_and_display("Creat features", 3000);

	ensure = PS_GenChar(CharBuffer1);
	if (ensure != 0x00) // 生成特征成功
		delay_and_display("Creat fail", 3000);

	delay_and_display("Searching ...", 3000);
	ensure = PS_HighSpeedSearch(CharBuffer1, 0, 300, &seach);

	if (ensure != 0x00) // 搜索成功
		delay_and_display("Search fail", 3000);

	if (seach.pageID == 0)
	{
		delay_and_display("No match", 3000);
		return FAILURE;
	
	}
	delay_and_display("Search success", 3000);

	sprintf(str_match, "Match ID:%d", seach.pageID);       // 显示匹配指纹的ID和分数
	Serial_SendString("2");
	delay_and_display(str_match, 3000);

	Serial_Printf("%d", seach.pageID);

	while(Serial_GetRxFlag() != 1)
	{
		count++;
		delay_and_display("waitting for data", 3000);
		if(count == 5){
			delay_and_display("clock in fail", 3000);
			return FAILURE;
		}
	}

	received_data = Serial_GetRxData();

	SYN_DefaultPlay(received_data); // 合成语音

	Serial_SendString(received_data);

	delay_and_display("clock in success", 10000);

	return COMPLETED;
}

// 删除指纹功能

enum ResultCode Del_FR(void)
{
	u16 id = get_input_id();
	char str[50];
	char *received_data;
	u8 ensure = (id == 0) ? PS_Empty() : PS_DeletChar(id, 1);

	Serial_SendString("3");

	if (id == 0)
		strcpy(str, "Deleting all");
	else
		sprintf(str, "Delete ID %d", id);

	delay_and_display(str, 3000);

	if (ensure == 0)
		delay_and_display("Delete success", 3000);

	else
		delay_and_display("Delete failed", 3000);

	Serial_Printf("%d", id);

	if (check_received_data(received_data, "Down") == 1)
		delay_and_display("Down", 3000);
	else
		delay_and_display("Wrong", 3000);

	return COMPLETED;
}
u16 get_input_id(void)
{
	u16 t_id = 0;
	enum Key_value key = UNKONE;

	show_onemessage(1, 1, "select id");
	OLED_ShowString(2, 1, "0 to del all");
	OLED_ShowString(4, 1, "id: ");
	OLED_ShowNum(4, 5, t_id, 2);

	while (key != ACK)
	{
		key = Key_GetNum();
		switch (key)
		{
		case PLUS:
			t_id = t_id == 300 ? 300 : t_id + 1;
			OLED_ShowNum(4, 5, t_id, 2);
			break;
		case MINUS:
			t_id = t_id == 0 ? 0 : t_id - 1;
			OLED_ShowNum(4, 5, t_id, 2);
			break;
		case ACK:
			break;
		case UNKONE:
			break;
		}
	}
	delay_and_display("ID selected", 3000);
	return t_id;
}
enum ResultCode get_Fingerprint_Count(u16 *count)
{
	u8 result;
	result = PS_ValidTempleteNum(count); // 读库指纹个数

	if (result)
		return FAILURE;

	return COMPLETED;
}
