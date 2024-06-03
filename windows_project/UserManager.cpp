#include "UserManager.h"
#include <sstream>
#include <windows.h>
#include <iomanip>

string UTF8ToGB(const char *str)
{
        string result;
        WCHAR *strSrc;
        LPSTR szRes;

        // 获得临时变量的大小
        int i = MultiByteToWideChar(CP_UTF8, 0, str, -1, NULL, 0);
        strSrc = new WCHAR[i + 1];
        MultiByteToWideChar(CP_UTF8, 0, str, -1, strSrc, i);

        // 获得临时变量的大小
        i = WideCharToMultiByte(CP_ACP, 0, strSrc, -1, NULL, 0, NULL, NULL);
        szRes = new CHAR[i + 1];
        WideCharToMultiByte(CP_ACP, 0, strSrc, -1, szRes, i, NULL, NULL);

        result = szRes;
        delete[] strSrc;
        delete[] szRes;

        return result;
}

string GBToUTF8(const char *str)
{
        string result;
        WCHAR *strSrc;
        LPSTR szRes;

        // 获取临时变量的大小
        int i = MultiByteToWideChar(CP_ACP, 0, str, -1, NULL, 0);
        strSrc = new WCHAR[i + 1];
        MultiByteToWideChar(CP_ACP, 0, str, -1, strSrc, i);

        // 获取临时变量的大小
        i = WideCharToMultiByte(CP_UTF8, 0, strSrc, -1, NULL, 0, NULL, NULL);
        szRes = new CHAR[i + 1];
        WideCharToMultiByte(CP_UTF8, 0, strSrc, -1, szRes, i, NULL, NULL);

        result = szRes;
        delete[] strSrc;
        delete[] szRes;

        return result;
}

// 构造函数
User_Manager::User_Manager(const string data_path) : data_path(data_path)
{

        wait_file_using();

        ifstream in(data_path);

        if (!in)
        {
                logger.log_format(LogLevel::ERROR_, "Open file %s fail [%s]", data_path.c_str(), __FUNCTION__);
                stats = false;
                in.close();
        }
        else
        {
                while (!in.eof())
                {
                        string line;
                        getline(in, line);

                        if (line.empty())
                                continue;

                        istringstream iss(line);

                        string key;
                        iss >> key;

                        cout << key << endl;

                        vector<string> value;
                        string temp;
                        while (iss >> temp)
                        {
                                temp = UTF8ToGB(temp.c_str()).c_str();
                                value.push_back(temp);
                        }
                        user_data[key] = value;
                }
                in.close();
                stats = true;
                logger.log_format(LogLevel::INFO, false, "Open file %s success", data_path.c_str());
                logger.log(LogLevel::INFO, "User_Manager construct success", true);
        }
        is_datausing = false;
        print_data();
}

// 析构函数
User_Manager::~User_Manager()
{
        save();
        logger.log(LogLevel::INFO, "User_Manager destruct", true);
}

// 保存数据
void User_Manager::save()
{
        ofstream out(data_path, ios::trunc);
        // cout << "save" << endl;
        if (!out)
        {
                logger.log_format(LogLevel::ERROR_, true, "Open file %s fail [%s]", data_path.c_str(), __FUNCTION__);
                out.close();
                // cout << "save fail" << endl;
                return;
        }

        string line;

        wait_file_using();
        for (auto it = user_data.begin(); it != user_data.end(); it++)
        {
                line = it->first;
                for (auto i = it->second.begin(); i != it->second.end(); i++)
                {
                        line += " " + GBToUTF8(i->c_str());
                }
                out << line << endl;
        }
        is_datausing = false;
        logger.log_format(LogLevel::INFO, true, "Save data to %s success", data_path.c_str());
        // cout << "save success" << endl;
        out.close();
}

// 打印数据
void User_Manager::print_data()
{
        wait_file_using();
        for (auto it = user_data.begin(); it != user_data.end(); it++)
        {
                cout << it->first;
                for (auto i = it->second.begin(); i != it->second.end(); i++)
                {
                        cout << " " << *i;
                }
                cout << endl;
        }
        is_datausing = false;
}

// 获取用户数据, 通过key 不可以使用
vector<string> User_Manager::get_value(const string key)
{
        wait_file_using();
        if (user_data.find(key) == user_data.end())
        {
                logger.log_format(LogLevel::ERROR_, "Key %s not found [%s]", key.c_str(), __FUNCTION__);
                return user_data["error"];
        }
        return user_data[key];
}

// 添加用户
void User_Manager::add_user(string name, string clock_in_counts, string checked)
{
        vector<string> value = {name, clock_in_counts, checked};
        wait_file_using();
        string id = to_string(user_data.size() + 1);
        user_data[id] = value;
        is_datausing = false;
        save();
}

// 删除用户
bool User_Manager::delete_user(string key)
{
        if (key == "0")
        {
                logger.log(LogLevel::INFO, "delete all user", true);
                wait_file_using();
                user_data.clear();
                is_datausing = false;
                save();
                return 1;
        }
        wait_file_using();
        if (user_data.find(key) == user_data.end())
        {
                logger.log_format(LogLevel::ERROR_, false, "Key %s not found [%s]", key.c_str(), __FUNCTION__);
                return 0;
        }
        user_data.erase(key);
        is_datausing = false;
        save();
        return 1;
}

bool User_Manager:: revoke(void)
{
        wait_file_using();
        if (user_data.size() == 0)
        {
                logger.log_format(LogLevel::ERROR_, false, "No user to revoke [%s]", __FUNCTION__);
                return 0;
        }
        user_data.erase(to_string(user_data.size()));
        is_datausing = false;
        save();
        return 1;
}

// // 获取用户数据
// map<string, vector<string>> &User_Manager::get_data()
// {
//         return user_data;
// }

// 获取用户数量
int User_Manager::get_count()
{
        wait_file_using();
        int count = user_data.size();
        is_datausing = false;
        return count;
}

// 获取文件状态
bool User_Manager::get_stats()
{
        return stats;
}
// 显示打卡信息
void User_Manager::show_clock_in()
{
        // 打印表头
        wait_file_using();
        std::cout << std::left << std::setw(3) << "\n\nID" << " "
                  << std::left << std::setw(20) << "姓名" << " "
                  << std::left << std::setw(8) << "签到次数" << " "
                  << std::left << std::setw(12) << "今天是否签到" << std::endl;
        for (auto it = user_data.begin(); it != user_data.end(); it++)
        {
                std::cout << std::left << std::setw(3) << it->first;
                for (auto i = it->second.begin(); i != it->second.end(); i++)
                {
                        int width;
                        if (i == it->second.begin())
                                width = 20; // 姓名
                        else if (i == it->second.begin() + 1)
                                width = 8; // 签到次数
                        else
                                width = 12; // 今天是否签到

                        if (width == 12)
                        {
                                if (*i == "1")
                                        std::cout << " " << std::left << std::setw(width) << "是";
                                else
                                        std::cout << " " << std::left << std::setw(width) << "否";
                        }
                        else
                                std::cout << " " << std::left << std::setw(width) << *i;
                }
                cout << endl;
        }
        is_datausing = false;
        cout << endl
             << endl
             << endl;
}

void User_Manager::show_no_clock_in()
{
        // 打印表头
        std::cout << std::left << std::setw(3) << "\n\nID" << " "
                  << std::left << std::setw(20) << "姓名" << " "
                  << std::left << std::setw(8) << "签到次数" << " "
                  << std::left << std::setw(12) << "今天是否签到" << std::endl;

        wait_file_using();
        for (auto it = user_data.begin(); it != user_data.end(); it++)
        {
                // 如果用户今天没有签到，打印他们的信息
                if (it->second.back() == "0")
                {
                        std::cout << std::left << std::setw(3) << it->first;
                        for (auto i = it->second.begin(); i != it->second.end(); i++)
                        {
                                int width;
                                if (i == it->second.begin())
                                        width = 20; // 姓名
                                else if (i == it->second.begin() + 1)
                                        width = 8; // 签到次数
                                else
                                        width = 12; // 今天是否签到

                                if (width == 12)
                                        std::cout << " " << std::left << std::setw(width) << "否";
                                else
                                        std::cout << " " << std::left << std::setw(width) << *i;
                        }

                        std::cout << std::endl;
                }
        }
        is_datausing = false;
        std::cout << std::endl
                  << std::endl
                  << std::endl;
}

void User_Manager::show_yes_clock_in()
{
        // 打印表头
        std::cout << std::left << std::setw(3) << "\n\nID" << " "
                  << std::left << std::setw(20) << "姓名" << " "
                  << std::left << std::setw(8) << "签到次数" << " "
                  << std::left << std::setw(12) << "今天是否签到" << std::endl;
        wait_file_using();
        for (auto it = user_data.begin(); it != user_data.end(); it++)
        {
                // 如果用户今天没有签到，打印他们的信息
                if (it->second.back() == "1")
                {
                        std::cout << std::left << std::setw(3) << it->first;
                        for (auto i = it->second.begin(); i != it->second.end(); i++)
                        {
                                int width;
                                if (i == it->second.begin())
                                        width = 20; // 姓名
                                else if (i == it->second.begin() + 1)
                                        width = 8; // 签到次数
                                else
                                        width = 12; // 今天是否签到

                                if (width == 12)
                                        std::cout << " " << std::left << std::setw(width) << "是";
                                else
                                        std::cout << " " << std::left << std::setw(width) << *i;
                        }
                        std::cout << std::endl;
                }
        }
        is_datausing = false;
        std::cout << std::endl
                  << std::endl
                  << std::endl;
}

void User_Manager::wait_file_using()
{
        while (is_datausing)
        {
        }
        is_datausing = true;
}