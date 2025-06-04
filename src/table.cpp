#include"table.h"
void table::read_from_file(const std::filesystem::path& file_path) {
//第一行为列名
//第二行为列类型
//第三行为paimary_key_index
    std::ifstream file(file_path);
    if (!file.is_open()){
        throw std::runtime_error("Could not open file: "+ file_path.string());
    }
    std::string line;
    if(std::getline(file, line)){
        std::istringstream iss(line);
        std::string column_name;
        while(iss >> column_name){
            if(column_name == " ")continue; //跳过空格
            column_names.push_back(column_name);
        }
    }

    if(std::getline(file,line)){
        std::istringstream iss(line);
        std::string column_type;
        while(iss >> column_type){
            if(column_type == " ")continue; //跳过空格
            int type_value=std::stoi(column_type);
            switch(type_value){
                case 0: column_types.push_back(type::INT); break;
                case 1: column_types.push_back(type::DOUBLE); break;
                case 2: column_types.push_back(type::STRING); break;
                default: throw std::runtime_error("Invalid column type in file: " + file_path.string());
            }
        }
    }
    if(std::getline(file,line)){
        primary_key_index = std::stoi(line);
    }
    while(std::getline(file,line)){
        if(line.empty()) continue;
        std::istringstream iss(line);
        auto new_row = std::make_unique<row>();
        std::string value;
        int index = 0;
        while(iss >> value){
            if(value == " ")continue; //跳过空格
            if(column_types[index] == type::INT){
                new_row->push_back(std::stoi(value));
            }else if(column_types[index] == type::DOUBLE){
                new_row->push_back(std::stod(value));
            }else if(column_types[index] == type::STRING){
                new_row->push_back(value);
            }
            index++;
        }
        data.insert(std::move(new_row));
        //主鍵索引不擁有row
        if(primary_key_index != -1){
            primary_key_index_map[(*new_row)[primary_key_index]] = new_row.get();
        }
    }
    file.close();
}

void table::write_to_file(const std::filesystem::path& file_path) const {
    std::ofstream file(file_path);
    if(!file.is_open()){
        throw std::runtime_error("Could not open file for writing: " + file_path.string());
    }
    //写入列名
    for(const auto& column_name : column_names){
        file << column_name << " ";
    }
    //换行
    file << std::endl;
    //写入列类型
    for(const auto& column_type : column_types){
        switch(column_type){
            case type::INT: file << 0 << " "; break;
            case type::DOUBLE: file << 1 << " "; break;
            case type::STRING: file << 2 << " "; break;
        }
    }
    //换行
    file << std::endl;
    //写入paimary_key_index
    file << primary_key_index << std::endl;
    //写入数据
    for(const auto& row : data){
        for(const auto& record : *row){
            std::visit([&file](auto&& arg){
                file << arg << " ";
                std::cout << arg << " ";
            },record);
        }
        file << std::endl;
    }
    file.close();
    
}

void table::insert_row(const row& new_row,std::filesystem::path file_path)
{
    std::unique_ptr<row> new_row_ptr = std::make_unique<row>(new_row);
    //获取主键在行中的具体数据
    auto primary_key_data = (*new_row_ptr)[primary_key_index];
    //首先检查是否有主建,无将则可以重复
    if(primary_key_index == -1){
        data.insert(std::move(new_row_ptr));
    }
   else{
    //如果有重复主建，则报错
    for(auto & record:primary_key_index_map)
    {
        if(record.first == primary_key_data)
        {
            std::cerr << "primary key data already exists" << std::endl;
            return;
        }
    }
    //没有则插入，更新map
    data.insert(std::move(new_row_ptr));
    primary_key_index_map[primary_key_data] = new_row_ptr.get();
   }
    std::cout << "suuccessfully insert" << std::endl;
    //先不保存
    // write_to_file(file_path);
    // std::cout << "suuccess insert to file" << std::endl;
}

void table::update_row(const row& old_row, const row& new_row)
{

}

void table::delete_row(const row& target_row)
{

}
