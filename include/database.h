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
    database(const std::string& name, const std::string& pass)
        : database_name(name),password(pass){
        //尝试从文件中读取数据库的表
        std::filesystem::path db_path = std::filesystem::current_path() / name;
        for(const auto& entry : std::filesystem::directory_iterator(db_path)){
            if(entry.is_regular_file() && entry.path().extension() == ".tbl"){
                std::string table_name = entry.path().stem().string();
            }
        }
        }
};