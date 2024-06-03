#ifndef __USART1_H
#define __USART1_H

#include "public_include.h"

#define PRINTF_BUFFER_SIZE 128  // 串口打印缓冲区大小
#define BUFFER_SIZE 64        // 串口接收缓冲区大小

extern char rx_buffer[BUFFER_SIZE];     // 串口接收缓冲区
extern uint8_t Serial_RxFlag;        // 串口接收标志位

void Serial_Init(void);                                 // 串口初始化
void Serial_SendByte(uint8_t Byte);                     // 发送一个字节
void Serial_SendArray(uint8_t *Array, uint16_t Length); // 发送一个数组
void Serial_SendString(char *String);                   // 发送一个字符串
void Serial_SendNumber(uint32_t Number, uint8_t Length);        // 发送一个数字, 有bug
void Serial_Printf(char *format, ...);

uint8_t Serial_GetRxFlag(void);

char* Serial_GetRxData(void);
void clear_rx_buffer(void);

u8 check_received_data(char * received_data, char * cmp_data);

#endif
