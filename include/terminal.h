#pragma once
#include <string>
#include <set>
#include <variant>
#include <sstream>
#include <iostream>
#include<ranges>
#include <vector>
#include <memory>
#include <filesystem>
#include<limits.h>
#include <unistd.h>
#include <fstream>
#include <algorithm>
#include"database.h"
#include"helper.h"

class terminal{
    using record = std::variant<int,double,std::string>;
    using row= std::vector<record>;
    using table_data = std::set<row>;
    private:
    std::string prompt;//命令行提示符
    std::string current_db;//当前使用的数据库名称
    std::set<std::string> databases;//存储所有数据库的名称
    std::filesystem::path origin_path;
    std::filesystem::path data_path;
    bool switched=false;
    std::set<std::string> keywords = {
        "create", "drop", "use", "insert", "select", "update", "delete","close","ls",
          "exit", "help","database","table","where","into","values","int","double","string","primary key",
    };
    std::set<std::string> command_prefixes = {
        "create database", "drop database", "use","create table",
        "drop table", "select","delete", "insert into","update","help","exit","close","ls"
    };
    std::unique_ptr<database> current_database;//当前使用的数据库指针
    std::string current_command;
    public:
    terminal() : prompt("dblite> "),current_db(""),current_database(nullptr){
        //终端直接切换到data目录下，这样的话可以直接在data目录下创建数据库
        //bin和data是同级目录，bin目录下是可执行文件，data目录下是数据库文件
        origin_path = std::filesystem::current_path();
        std::filesystem::path object = origin_path;
        //只有两种情况需要切换到data目录下：
        //1.当前目录是bin目录
        //2.当前目录是bin的父目录
        if(origin_path.filename() == "bin"){
            object = origin_path.parent_path() / "data";
        }else{
            object = origin_path / "data";
        }
        //如果现在是在bin的父目录下，就要先切换到data目录下
        if(object!=origin_path){
            std::filesystem::current_path(object);
            switched = true;
        }
        data_path = object;
        
        //将data目录下的所有数据库名称加载到databases中
        for(const auto& entry : std::filesystem::directory_iterator(object)){
            if(entry.is_directory()){
                databases.insert(entry.path().filename().string());
            }
        }

    }
    ~terminal(){
        if(current_database){
            //在终端退出时保存当前数据库的状态
            //TODO
            current_database->save_tables();
        }
        // 切换回原来的工作目录

        if(switched){
            std::filesystem::current_path(origin_path);
        }
    }
    bool find_command(const std::string& command);
    void run_command(const std::string& command);

    void set_current_db(const std::string& db){
        current_db = db;
    }

    void create_db(const std::string& db_name,const std::string& password = "");
    void drop_db(const std::string& db_name);
    void use_db(const std::string& db_name);
    void create_table(const std::string& table_name,const std::string& column_defintions);
    void drop_table(const std::string& table_name);
    void insert_into_table(const std::string& table_name,const row& values);
    void select_from_table(const std::string& table_name,const std::vector<std::string>& column_names,
                           const std::string& condition = "");//注意条件是可以有and和or之类连接的,所以交给函数自己判断
    void update_table(const std::string& table_name,const std::string& column_name,
                      const record& value, const std::string& condition = "");
    void delete_from_table(const std::string& table_name, const std::string& condition = "");
    void exit();
    void close_db();
    void ls_table();
    std::string get_prompt() const {
        return prompt;
    }
    void help() const {
        std::cout<< "Available commands:\n"
                    << "create database <db_name> [identified by <password>];\n"
                    << "drop database <db_name>;\n"
                    << "use <db_name>;\n"//使用此语句来进行数据库的切换
                    << "create table <table_name> (<column_definitions>);\n"
                    << "drop table <table_name>;\n"
                    << "select <column_names> from <table_name> [where <condition>];\n"
                    << "insert into <table_name> values (<values>);\n"
                    << "update <table_name> set <column_name> = <value> [where <condition>];\n"
                    << "delete from <table_name> where <condition>;\n"
                    << "close;\n"//关闭当前的数据库
                    << "exit;\n"
                    << "help;\n";
    }
};