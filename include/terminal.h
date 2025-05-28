#pragma once
#include <string>
#include <set>
#include <variant>
#include <sstream>
#include <iostream>
#include<ranges>
#include <vector>
#include"database.h"
class terminal{
    using record = std::variant<int,double,std::string>;
    using row= std::vector<record>;
    using table_data = std::set<row>;
    private:
    std::string prompt;
    std::string current_db;
    std::set<std::string> keywords = {
        "create", "drop", "use", "insert", "select", "update", "delete",
          "exit", "help","database","table","where","into","values"
    };
    std::set<std::string> command_prefixes = {
        "create database", "drop database", "use","create table",
        "drop table", "select","delete", "insert into","update","help","exit"
    };
    std::unique_ptr<database> current_database;
    std::string current_command;
    public:
    terminal() : prompt("dblite> "),current_db(""),current_database(nullptr){}
    bool find_command(const std::string& command);
    void run_command(const std::string& command);

    void set_current_db(const std::string& db){
        current_db = db;
    }

    void create_db(const std::string& db_name,const std::string& password = "");
    void drop_db(const std::string& db_name);
    void use_db(const std::string& db_name);
    void create_table(const std::string& table_name);
    void drop_table(const std::string& table_name);
    void insert_into_table(const std::string& table_name,const row& values);
    void select_from_table(const std::string& table_name,const std::vector<std::string>& column_names,
                           const std::string& condition = "");//注意条件是可以有and和or之类连接的
    void update_table(const std::string& table_name,const std::string& column_name,
                      const record& value, const std::string& condition = "");
    void delete_from_table(const std::string& table_name, const std::string& condition = "");
    void exit();
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
                    << "exit;\n"
                    << "help;\n";
    }
};