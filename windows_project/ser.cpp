#define UNICODE
#include "ser.h"
#include <iostream>
using std::cout;
using std::endl;

/*
初始化串口
*/
Stm32::Stm32() : dcbSerialParams({0}), timeouts({0})
{

        Post = CreateFile(L"COM5", GENERIC_READ | GENERIC_WRITE, 0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);

        dcbSerialParams.BaudRate = CBR_9600;   // 波特率 9600
        dcbSerialParams.ByteSize = 8;          // 数据位 8
        dcbSerialParams.StopBits = ONESTOPBIT; // 停止位 1
        dcbSerialParams.Parity = NOPARITY;     // 校验位 无

        timeouts.ReadIntervalTimeout = 50;
        timeouts.ReadTotalTimeoutConstant = 50;
        timeouts.ReadTotalTimeoutMultiplier = 10;
        timeouts.WriteTotalTimeoutConstant = 50;
        timeouts.WriteTotalTimeoutMultiplier = 10;

        post_stast = OPEN;

        if (Post == INVALID_HANDLE_VALUE)
        {
                post_stast = ERROR_;
        }


        if (!GetCommState(Post, &dcbSerialParams))
        {
                post_stast = ERROR_;
                CloseHandle(Post);
        }

        if (!SetCommState(Post, &dcbSerialParams))
        {
                cout << "set dcb fail" << endl;
                post_stast = ERROR_;
                CloseHandle(Post);
        }

        if (!SetCommTimeouts(Post, &timeouts))
        {
                cout << "set timeout fail" << endl;
                post_stast = ERROR_;
                CloseHandle(Post);
        }
}

Stm32::~Stm32()
{
        CloseHandle(Post);
}

/*
接收数据
*/
STAST Stm32::get_message(HANDLE com, string &data)
{

        const int MAX_BUFFER_SIZE = 256;
        char buffer[MAX_BUFFER_SIZE];
        DWORD bytesRead;


        if (!ReadFile(com, buffer, MAX_BUFFER_SIZE - 1, &bytesRead, NULL))
        {
                return GET_DATA_ERROR;
        }
        else
        {
                buffer[bytesRead] = '\0';
                data = string(buffer);

                if (bytesRead == 0)
                {
                        return NO_DATA_;
                }
                return GET_DATA;
        }
}

/*
发送数据
*/
STAST Stm32::send_message(HANDLE com, string &data)
{
        DWORD bytes_written;
        data += "\n";

        if (!WriteFile(Post, data.c_str(), strlen(data.c_str()), &bytes_written, NULL))
        {
                return SEND_DATA_ERROR;
        }
        else
        {
                return SEND_DATA;
        }
}


