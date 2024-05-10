#include "handle_event.h"

using std::cout;
using std::endl;
using std::string;

std::atomic<bool> timeInRange(true); // 原子布尔变量，用于表示时间是否在范围内
std::atomic<bool> shouldStop(true);
std::atomic<bool> is_datausing(false);

enum Step step = RECEIVE_DATA;

SYSTEMTIME local_st;
SYSTEMTIME start_st;
SYSTEMTIME end_st;

Logger logger("..\\data\\logfile.txt");
Stm32 stm32;
map<string, vector<string>> user_data = {};
User_Manager user_manager("..\\data\\data.txt");

void check_file()
{

	while (user_manager.get_stats() == false)
	{
		cout << "open file fail" << endl;
		Sleep(1000);
	}
}

void wait_event()
{
	string data;
	while (shouldStop)
	{
		// cout << "waitting ..." << endl;
		if (stm32.get_message(stm32.get_Post(), data) != GET_DATA)
		{
			Sleep(1000);
			continue;
		}

		logger.log(LogLevel::INFO, std::string("Received data from stm32: ") + data, true);

		// 处理数据
		if (data == "2") // 签到打卡， 放在第一个因为这个操作最频繁
			clock_in();
		else if (data == "1") // 添加用户
			add_user();
		else if (data == "3") // 删除用户
			delete_user();
		else if (data == "connect")
		{
			if (timeInRange)
				stm32.send_message(stm32.get_Post(), std::string("In time"));
			else
				stm32.send_message(stm32.get_Post(), std::string("Out of time"));
		}
	}
}

void clock_in()
{
	string response = "ERROR";
	string data;
	int fail_count = 0;
	while (step != END_STEP)
	{
		switch (step)
		{
		case RECEIVE_DATA:
		{
			if (fail_count > 5)
			{
				logger.log(LogLevel::ERROR_, std::string("Waitting for the ID from stm32 fail"), true);
				step = END_STEP;
				break;
			}
			logger.log(LogLevel::INFO, std::string("Waitting for the ID from stm32 ..."), true);
			if (stm32.get_message(stm32.get_Post(), data) != GET_DATA)
			{
				fail_count++;
				Sleep(1000);
			}
			else
			{
				logger.log(LogLevel::INFO, std::string("Received ID from stm32: ") + data, true);
				step = CHECK_DATA;
			}
		}
		break;
		case CHECK_DATA:
		{

			logger.log(LogLevel::INFO, std::string("Checking the ID is digit or not ..."), true);

			step = CHECK_DATA_EXISTS;
			for (char c : data)
			{
				if (!std::isdigit(c))
				{
					logger.log(LogLevel::ERROR_, std::string("The ID is not digit"), true);
					step = RECEIVE_DATA;
					stm32.send_message(stm32.get_Post(), response);
					break;
				}
			}
		}
		break;

		case CHECK_DATA_EXISTS:
		{

			logger.log(LogLevel::INFO, std::string("Checking the ID is exists or not ..."), true);

			if (user_data[data].size() == 0)
			{
				logger.log(LogLevel::ERROR_, std::string("The ID is not exists"), true);
				stm32.send_message(stm32.get_Post(), response);
				step = RECEIVE_DATA;
			}
			else
			{
				logger.log(LogLevel::INFO, std::string("The ID is exists"), true);
				step = PROCESS_DATA;
			}
		}
		break;
		case PROCESS_DATA:
		{

			logger.log(LogLevel::INFO, std::string("Processing the ID ..."), true);
			string temp;

			if (user_data[data][2] == "0")
			{
				user_data[data][2] = "1";
				user_data[data][1] = incrementCharNumber(user_data[data][1]);
				user_manager.save();
				user_manager.print_data();
				temp = user_data[data][0] + "打卡成功";
				cout << temp << endl;
				stm32.send_message(stm32.get_Post(), temp);
				Sleep(1000);
			}
			else
			{
				temp = user_data[data][0] + "已经打卡成功";
				stm32.send_message(stm32.get_Post(), temp);

				Sleep(1000);
			}
			logger.log(LogLevel::INFO, std::string("Send message to stm32: ") + temp, true);
			step = END_STEP;
		}
		break;
		}
	}
}

void add_user()
{
	string name;
	string clock_in_counts;
	string checked;

	logger.log(LogLevel::INFO, std::string("Waitting for input user message ..."), true);
	std::cout << "Please input the user: name  clock_in_counts  checked" << std::endl;

	std::cin >> name >> clock_in_counts >> checked;

	user_manager.add_user(name, clock_in_counts, checked);

	logger.log(LogLevel::INFO, std::string("Add user success"), true);

	if (stm32.send_message(stm32.get_Post(), std::string("Down")) == SEND_DATA_ERROR)
	{
		logger.log(LogLevel::ERROR_, std::string("Send message to stm32 fail"), true);
	}
	else
	{
		logger.log(LogLevel::INFO, std::string("Send message to stm32 success"), true);
	}
}

void delete_user()
{
	string id;
	string response = "ERROR";

	logger.log(LogLevel::INFO, std::string("Waitting for user ID ..."), true);

	while (stm32.get_message(stm32.get_Post(), id) != GET_DATA)
	{
		Sleep(1000);
	}

	logger.log(LogLevel::INFO, std::string("Received ID from stm32: ") + id, true);

	if (user_manager.delete_user(id) == false)
	{
		logger.log(LogLevel::ERROR_, std::string("Delete user fail"), true);
		stm32.send_message(stm32.get_Post(), response);
		return;
	}

	stm32.send_message(stm32.get_Post(), std::string("Down"));
	logger.log(LogLevel::INFO, std::string("Delete user success"), true);
}

bool check_time()
{
	GetLocalTime(&local_st);

	if (local_st.wHour < start_st.wHour || local_st.wHour > end_st.wHour)
	{

		return false;
	}
	return true;
}

void checkTimeThread()
{
	GetLocalTime(&local_st);
	logger.log_format(LogLevel::INFO, true, "The local time : %d:%d:%d", local_st.wHour, local_st.wMinute, local_st.wSecond);
	logger.log_format(LogLevel::INFO, true, "The start time : %d:%d:%d", start_st.wHour, start_st.wMinute, start_st.wSecond);
	logger.log_format(LogLevel::INFO, true, "The end time : %d:%d:%d", end_st.wHour, end_st.wMinute, end_st.wSecond);

	while (shouldStop)
	{
		if (step == RECEIVE_DATA)
		{
			if (check_time() == false)
			{
				if (timeInRange == true)
				{
					logger.log(LogLevel::INFO, std::string("Out of time"), true);
					stm32.send_message(stm32.get_Post(), std::string("Out of time"));
					timeInRange = false;
				}
			}
			else
			{
				if (timeInRange == false)
				{
					logger.log(LogLevel::INFO, std::string("In time"), true);
					stm32.send_message(stm32.get_Post(), std::string("In time"));
					timeInRange = true;
				}
			}
		}
		std::this_thread::sleep_for(std::chrono::seconds(30)); // 每30秒检查一次
	}
}

std::string incrementCharNumber(const std::string &input)
{
	int number = std::stoi(input); // 将字符串转换为整数
	number += 1;		       // 增加1
	return std::to_string(number); // 将整数转换回字符串并返回
}

void keyboardThreadFunction()
{
	while (true)
	{
		if ((GetAsyncKeyState(VK_CONTROL) & 0x8000) && (GetAsyncKeyState('W') & 0x8000))
		{
			user_manager.show_clock_in();
		}

		if ((GetAsyncKeyState(VK_CONTROL) & 0x8000) && (GetAsyncKeyState('T') & 0x8000))
		{
			shouldStop = false;
			cout << "Exit program" << endl;
			break;
		}
		if ((GetAsyncKeyState(VK_CONTROL) & 0x8000) && (GetAsyncKeyState('Y') & 0x8000))
		{
			user_manager.show_no_clock_in();
		}
		if ((GetAsyncKeyState(VK_CONTROL) & 0x8000) && (GetAsyncKeyState('U') & 0x8000))
		{
			user_manager.show_yes_clock_in();
		}
		std::this_thread::sleep_for(std::chrono::milliseconds(100)); // Prevent CPU overuse
	}
}

void open_post()
{
	while (stm32.get_post_stast() == ERROR_)
	{
		cout << "open post fail" << endl;
		Sleep(1000);
	}
}
