#include "menu.h"

extern u8 in_time;
;

void show_menu(void)
{
	OLED_Clear();
	OLED_ShowString(1, 7, "1.Add_FR");
	OLED_ShowString(2, 7, "2.Press_FR");
	OLED_ShowString(3, 7, "3.Del_FR");
	OLED_ShowString(4, 7, "4.Get_Num");
}

void show_select(uint8_t column)
{
	show_menu();
	OLED_ShowNum(1, 1, column, 1);
	OLED_ShowString(column, 3, ">>");
}

void handle_functon(uint8_t select_id)
{
	switch (select_id)
	{

	case 1:
		Add_FR(); // 录入
		back_menu(select_id);
		break;
	case 2:

		Press_FR(); // 验证
		back_menu(select_id);
		break;
	case 3:
		Del_FR(); // 删除
		back_menu(select_id);
		break;
	case 4:
		get_Fingerprint_Count(&ValidN);
		sprintf(str, "Finger count: %d", ValidN);
		delay_and_display(str, 3000);
		back_menu(select_id);
		break;
	}
}

__inline void back_menu(uint8_t select_id)
{
	OLED_ShowString(4, 1, "back to menu");
	delay_ms(3000);
	show_select(select_id);
}

void check_time()
{
	char *received_data;
	
	received_data = Serial_GetRxData();
	// Serial_SendString(received_data);
	if (strcmp(received_data, "Out of time") == 0) // 比较 received_data 和 "1234567890"
	{
		// delay_and_display("Out of time", 3000); // 显示 "Out of time"
		in_time = 0;
	}
	else if (strcmp(received_data, "In time") == 0)
	{
		// delay_and_display("In time", 3000); // 显示 received_data
		in_time = 1;
	}
}
