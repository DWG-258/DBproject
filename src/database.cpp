#include "database.h"

void database::save_tables() const {
    for(const auto& [table_name, tb_ptr] : tables){
        std::filesystem::path file_path = std::filesystem::current_path() / (table_name + ".tbl");
        std::filesystem::path index_path = std::filesystem::current_path() / (table_name + ".idx");

        tb_ptr->write_to_file(file_path);
        
    }
    std::cout << "All tables saved successfully." << std::endl;
}