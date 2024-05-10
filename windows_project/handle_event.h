#pragma once
#include <iostream>
#include <string>
#include <windows.h>
#include <time.h>
#include <thread>
#include <atomic>

#include "HSerial.h"
#include "UserManager.h"
#include "./Log/Logger.h"



extern SYSTEMTIME local_st;
extern SYSTEMTIME start_st;
extern SYSTEMTIME end_st;

extern Logger logger;
extern Stm32 stm32;
extern User_Manager user_manager;


extern std::map<std::string, std::vector<std::string>> user_data;

enum Step 
{
    RECEIVE_DATA = 0,
    CHECK_DATA,
    CHECK_DATA_EXISTS,
    PROCESS_DATA,
    END_STEP
};

void wait_event();

// 用于处理串口数据
void clock_in();
void add_user();
void delete_user();

// 时间检查线程
void checkTimeThread();

void open_post();
void check_file();
bool check_time();

std::string incrementCharNumber(const std::string& input);
void keyboardThreadFunction();


