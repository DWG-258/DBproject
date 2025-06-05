#include"table.h"
#include<string>
#include<set>
#include<memory>
#include<map>
#include<iostream>
#include <variant>
#include <filesystem>
#pragma once

class database {
    private:
        std::string database_name;
        std::map<std::string,std::shared_ptr<table>> tables;
        std::string password;
    public:
    friend class terminal; // 允许 terminal 类访问私有成员
    database(const std::string& name, const std::string& pass)
        : database_name(name),password(pass){
        //尝试从文件中读取数据库的表
        std::filesystem::path db_path = std::filesystem::current_path() / name;
        for(const auto& entry : std::filesystem::directory_iterator(db_path)){
            if(entry.is_regular_file() && entry.path().extension() == ".tbl"){
                std::string table_name = entry.path().stem().string();//获取表名
               
              
                auto new_table = std::make_shared<table>(table_name,entry.path());
                tables[table_name] = new_table;
               
            }
        }
        }
    void save_tables() const;


     //new 5.31
     std::string get_name()
     {
         return database_name;
     }
     std::shared_ptr<table>  get_table(std::string table_name)
     {
          if(tables.find(table_name) == tables.end())
         {
             std::cout << "Table " << table_name << " does not exist." << std::endl;
             return nullptr;
         }
         else
         {
             return tables[ table_name];
         }
        
     }
};