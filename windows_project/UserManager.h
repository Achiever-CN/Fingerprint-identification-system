#pragma once
#include <iostream>
#include <fstream>
#include <map>
#include <vector>
#include "./Log/Logger.h"
using namespace std;

using std::string;
extern Logger logger;
extern map<string, vector<string>> user_data;
extern std::atomic<bool> is_datausing;

/*
文件处理类
*/
class User_Manager
{
public:
        User_Manager(const string file_name);
        ~User_Manager();

        void save();

        vector<string> get_value(const string key);
        // map<string, vector<string>>& get_data();

        void add_user(string name, string clock_in_counts, string checked);
        bool delete_user(string key);


        void print_data();
        int get_count();
        bool get_stats();

        void show_clock_in();

        void show_no_clock_in();
        void show_yes_clock_in();

private:
        void wait_file_using();

        string data_path;
        bool stats;
};


