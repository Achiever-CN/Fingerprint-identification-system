#ifndef __OLED_H
#define __OLED_H

void OLED_Init(void);
void OLED_Clear(void);
void show_onemessage(uint8_t, uint8_t, char *);
void OLED_ShowChar(uint8_t Line, uint8_t Column, char Char);
void OLED_ShowString(uint8_t Line, uint8_t Column, char *String);
void OLED_ShowNum(uint8_t Line, uint8_t Column, uint32_t Number, uint8_t Length);
void OLED_ShowSignedNum(uint8_t Line, uint8_t Column, int32_t Number, uint8_t Length);
void OLED_ShowHexNum(uint8_t Line, uint8_t Column, uint32_t Number, uint8_t Length);
void OLED_ShowBinNum(uint8_t Line, uint8_t Column, uint32_t Number, uint8_t Length);


/*
OLED_Init GPIOB SCL B8
OLED_Init GPIOB SCD B9

ʹ������
OLED_ShowChar(1, 1, 'A'); ��ʾһ���ַ� �ڼ��� �ڼ���
OLED_ShowString(1, 3, "HelloWorld!");
OLED_ShowNum(2, 1, 12345, 5); ��ʾ���� �ڼ��� �ڼ��� ���� ����
OLED_ShowSignedNum(2, 7, -66, 2);
OLED_ShowHexNum(3, 1, 0xAA55, 4);
OLED_ShowBinNum(4, 1, 0xAA55, 16);

*/



#endif
