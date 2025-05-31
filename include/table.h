#include <set>
#include <string>
#include <variant>
#include <vector>
#include <filesystem>
#include "type.h"
#pragma once


class table{
    private:
    void read_from_file(const std::filesystem::path& file_path);
    void write_to_file(const std::string& file_path) const;
    public:
    using record = std::variant<int,double,std::string>;
    using row = std::vector<record>;
    using table_data = std::set<row>;
    private:
    std::string table_name;
    table_data data;
    std::vector<std::string> column_names;
    int primary_key_index = -1;
    std::vector<type> column_types;
    public:
    table(const std::string& name, const std::vector<std::string>& column_names,
          const std::vector<type>& column_types,std::filesystem::path file_path)
        : table_name(name), column_names(column_names), column_types(column_types) {
            read_from_file(file_path);
        }
    void rename(const std::string& new_name);
    void add_column(const std::string& column_name, type column_type);
    void remove_column(const std::string& column_name);
    void insert_row(const row& new_row);
    void delete_row(const row& target_row);
    void update_row(const row& old_row, const row& new_row);
};