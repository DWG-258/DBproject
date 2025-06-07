#include <set>
#include <string>
#include <variant>
#include <vector>
#include <filesystem>
#include <fstream>
#include <map>
#include <memory>
#include "type.h"
#include <iostream>
#include "helper.h"
#pragma once


class table{
    private:
    void read_from_file(const std::filesystem::path& file_path);
    void write_to_file(const std::filesystem::path& file_path) const;
    public:
    using record = std::variant<int,double,std::string>;
    using row = std::vector<record>;
    using table_data = std::set<std::unique_ptr<row>>;
    private:
    friend class database;
    std::string table_name;
    table_data data;
    std::vector<std::string> column_names;
    //该map为主建索引，即主键的的数据和行
    std::map<record,row*> primary_key_index_map;
    std::vector<type> column_types;
    std::map<std::string,type> column_type_map;
    int primary_key_index = -1;
    public:
    //这个构造函数是从文件中读取表数据
    table(const std::string& name, std::filesystem::path file_path)
        : table_name(name) {
            read_from_file(file_path);
        }
    //这个构造函数是创建一个新的表
    table(const std::string& name, const std::vector<std::string>& columns, 
    const std::vector<type>& types, int primary_key = -1)
        :table_name(name),column_names(columns),column_types(types),primary_key_index(primary_key){
            //如果没有主键，则 primary_key_index 设置为 -1，不设置主键
            //如果有主键，则设置 primary_key_index 为主键所在的列索引
        }
    void rename(const std::string& new_name);
    void add_column(const std::string& column_name, type column_type);
    void remove_column(const std::string& column_name);
    void insert_row(const row& new_row,std::filesystem::path file_path);
    void delete_row(const std::vector<std::string>& condition);
    void update_row(const row& old_row, const row& new_row);
    void select_all();
    void select_column(const std::vector<std::string>& column_names,const std::vector<std::string>& condition);
    void print_row_in_condition(const std::vector<std::string>& column_names,const std::vector<std::string>& condition);
    void print_row_without_condition(const std::vector<std::string>& column_names);
    void delete_all();
    template<typename variable_type>
    void print_record(const variable_type& v);
    void update_row(const std::string& column_name,const record& value,const std::vector<std::string>& condition);


    std::vector<std::string> get_column_names() const
    {
        return column_names;
    }
    std::vector<type> get_column_types() const
    {
        return column_types;

    }

    int get_column_index(const std::string& column_name) const
    {
        //如果是*则返回-1
        if(column_name=="*")
        {
            return -1;
        }
        for(std::size_t i = 0; i < column_names.size(); i++)
        {
            if(column_names[i] == column_name)
            {
                return i;
            }
        }
        //都不是则返回-2
        return -2;
    }

 
};