#ifndef __PUBLIC_INCLUDE_H
#define __PUBLIC_INCLUDE_H
#include <stdlib.h>
#include <string.h>
#include <stdarg.h> // 用于处理可变参数
#include "stm32f10x.h"
#include "sys.h"
#include "delay.h"
#include "OLED.h"
#include "Key.h"
#include "AS608.h"

enum ResultCode
{
	COMPLETED = 1,
	FAILURE = 2,
	UNKNOWN = 3

};

enum Key_value
{
	ACK = 1,
	PLUS = 2,
	MINUS = 3,
	UNKONE = 4

};


#endif
