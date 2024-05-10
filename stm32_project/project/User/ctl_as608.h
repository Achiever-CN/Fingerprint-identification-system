#ifndef __ctl_as608_H
#define __ctl_as608_H
#include "public_include.h"
#include "key.h"
#include "usart1.h"


extern SysPara AS608Para;
extern u16 ValidN;
extern u16 id;
extern char str[100];



enum ResultCode connect_FR(void);
enum ResultCode connect_PC(void);


enum ResultCode Add_FR(void);
enum ResultCode Press_FR(void);
enum ResultCode Del_FR(void);


u16 get_input_id(void);
enum ResultCode get_Fingerprint_Count(u16 *count);
void delay_and_display(char *message, int delay_time);

#endif


