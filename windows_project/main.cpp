#include "handle_event.h"
#include "./Log/Logger.h"
#include <iostream>
#include <direct.h> 

// check_in 函数重启

// -t 8 21
int main(int argc, char *argv[])
{

	logger.log(LogLevel::INFO, "\n\n-----------Start program -----------", true);

	for (int i = 0; i < argc; i++)
	{
		if (strcmp(argv[i], "-t") == 0)
		{
			start_st.wHour = atoi(argv[i + 1]);
			end_st.wHour = atoi(argv[i + 2]);
		}
	}

	// 打开串口
	open_post();

	// // 打开文件
	check_file();

	std::thread timeThread(checkTimeThread);
	std::thread keyboardThread(keyboardThreadFunction);
	Sleep(1000);

	// 处理数据
	wait_event();

	timeThread.join(); // 等待线程结束
	keyboardThread.join();

	logger.log(LogLevel::INFO, "-----------End program -----------\n\n", true);

	return 0;
}
