#pragma once

#include <windows.h>
#include <iostream>
#include "./Log/Logger.h"
using std::string;


extern Logger logger;

enum STAST
{
        OPEN,
        CLOSE,
	ERROR_,
	GET_DATA,
	SEND_DATA,
	GET_DATA_ERROR,
	SEND_DATA_ERROR,
	NO_DATA_
};

/*
串口类
*/
class Stm32
{
public:
	Stm32();
	~Stm32();

	STAST get_post_stast() { return post_stast; }	

	STAST get_message(HANDLE com, string &data);
	STAST send_message(HANDLE com, string &data);
	STAST send_message(HANDLE com, const char * data);

	HANDLE get_Post() { return Post; }

private: 
	HANDLE Post;
	DCB dcbSerialParams;
	COMMTIMEOUTS timeouts;

    STAST post_stast;
};