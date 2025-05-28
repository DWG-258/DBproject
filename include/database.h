#include"table.h"
#include<string>
#include<set>
#include<memory>
#include<map>
#include<iostream>
#include <variant>
#pragma once

class database {
    private:
        std::string database_name;
        std::map<std::string,std::shared_ptr<table>> tables;
        std::string password;
    public:
    database(const std::string& name, const std::string& pass)
        : database_name(name),password(pass){}
};