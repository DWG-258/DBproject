#include <set>
#include <string>
#include <variant>
#include <vector>
#include <filesystem>
#include <fstream>
#include <map>
#include "type.h"
#pragma once


class table{
    private:
    void read_from_file(const std::filesystem::path& file_path);
    void write_to_file(const std::filesystem::path& file_path) const;
    public:
    using record = std::variant<int,double,std::string>;
    using row = std::vector<record>;
    using table_data = std::set<row>;
    private:
    friend class database;
    std::string table_name;
    table_data data;
    std::vector<std::string> column_names;
    std::map<record,row> primary_key_index_map;
    std::vector<type> column_types;
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
    void insert_row(const row& new_row);
    void delete_row(const row& target_row);
    void update_row(const row& old_row, const row& new_row);
};