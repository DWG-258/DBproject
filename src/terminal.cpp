#include"terminal.h"
#include"helper.h"
#include <utility>
bool terminal::find_command(const std::string& command){
    if(command.substr(command.size()-1)!=";"){
        std::cerr << "Error: Command must end with a semicolon." << std::endl;
        return false;
    }
    std::string _command= command.substr(0,command.size()-1);//移除末尾的分号
    std::istringstream iss(_command);
    std::string token;
    std::string command_prefix="";
    while(iss >> token){
        if(keywords.find(token) != keywords.end()){
            command_prefix += token + " ";
        }else{
            break;
        }
    }
    command_prefix = command_prefix.substr(0,command_prefix.size()-1);//移除空格
    if(command_prefixes.find(command_prefix)!= command_prefixes.end()){
        return true;
    }else{
        std::cerr << "Error: Command not found or invalid syntax."<<std::endl;
        return false;
    }
}

void terminal::run_command(const std::string& command){
    if(command.substr(command.size()-1)!=";"){
        std::cerr << "Error: Command must end with a semicolon." << std::endl;
        return;
    }else{
        current_command = command.substr(0,command.size()-1);//移除末尾的分号
        if(current_command=="exit"){
            exit();
            return;
        }else if(current_command=="help"){
            help();
            return;
        }else if(current_command=="close"){
            close_db();
            return;
        }else if(current_command=="ls"){
            ls_table();
            return;
        }
        std::istringstream iss(current_command);
        std::string token;
        iss >> token;
        if(token == "create"){
            iss >> token;
            if(token == "database"){
                std::string db_name;
                iss >> db_name;
                std::string password="";
                if(iss>>token&& token == "identified"&&iss>>token && token == "by"){
                    iss >> password;
                }
                create_db(db_name, password);
            }else if(token == "table"){
                //错误交给各自的函数来处理
                std::string table_name;
                iss >> table_name;
                std::string column_definitions;
                std::getline(iss,column_definitions);
                column_definitions.erase(column_definitions.begin(),std::find_if(column_definitions.begin(),column_definitions.end(),[](unsigned char c){return !std::isspace(c);}));
                if(column_definitions.empty() || column_definitions[0] != '(' || column_definitions.back() != ')'){
                    std::cerr << "Error: Invalid table definition syntax." << std::endl;
                    return;
                }
                column_definitions = column_definitions.substr(1,column_definitions.size()-2);//去掉括号
                create_table(table_name, column_definitions);
            }else{
                std::cerr<<"Error: Invalid command syntax."<<std::endl;
            }
        }else if(token == "drop"){
            iss >> token;
            if(token == "database"){
                std::string db_name;
                iss >> db_name;
                drop_db(db_name);

            }else if(token == "table"){
                std::string table_name;
                iss >> table_name;
                drop_table(table_name);
            }
        }else if(token == "use"){
            std::string db_name;
            iss >> db_name;
            use_db(db_name);
        }else if(token == "insert"){
            iss >> token;
            if(token == "into"){
                std::string table_name;
                iss >> table_name;
                std::string values;
                iss>>values;
                if(values=="values"){
                    row values_row;
                    std::string value;
                    iss>>value;
                    value= value.substr(1,value.size()-2);//去掉括号
                    std::istringstream value_stream(value);
                    std::string _token;
                    while(std::getline(value_stream,_token,',')){
                        
                        if(is_double_(_token)){
                            values_row.push_back(std::stod(_token));
                
                        
                        }else if(is_int_(_token)){
                            values_row.push_back(std::stoi(_token));
                        }else{
                            auto quoted_value = in_quotation(_token);
                            if(quoted_value!= std::nullopt){
                                values_row.push_back(*quoted_value);
                               
                            }else{
                                std::cerr << "Error: Invalid value type."<<std::endl;
                                return;
                            }
                        }
                

                    }
                    insert_into_table(table_name, values_row);
                    return;
                }else{
                    std::cerr << "Error: Invalid command syntax."<<std::endl;
                    return;
                }
            }
        }else if(token == "select"){
            std::vector<std::string> column_names;
            std::string table_name;
            bool where_exist = false;
            std::string condition;
            while(iss>>token){
                if(token == "from"){
                    iss >> table_name;
                    
                }else if(token == "where"){
                    
                    while(iss >> token){
                        condition +=token + " ";
                    }
                    condition = condition.substr(0,condition.size()-1);//移除末尾的空格
                    select_from_table(table_name, column_names, condition);
                    where_exist = true;
                }else if(token == "*"){
                    //选取所有列
                    column_names.push_back("*");
                }
                else{
                    if(token !=","){
                        column_names.push_back(token);
                    }
                }
            }
            //如果无条件
            if(!where_exist){
                select_from_table(table_name, column_names,condition);
            }
        }else if(token == "update"){
            std::string table_name;
            iss >> table_name;
            iss >> token;
            if(token != "set"){
                std::cerr << "Error: Invalid command syntax." << std::endl;
                return;
            }
            std::string column_name;
            iss >> column_name;
         
            
           
            iss>>token;
            if(token != "=")
            {
                std::cerr << "Error: Invalid command syntax." << token<< std::endl;
            }
            std::string value_str;
             iss >> value_str;
            record value;
            if(is_double_(value_str))
            {
                 value = std::stod(value_str);
            }
            else if(is_int_(value_str)){
                value = std::stoi(value_str);
            }
            else if(auto quoted_value = in_quotation(value_str);quoted_value!=std::nullopt){
                value = *quoted_value;
            }else{
                std::cerr << "Error: Invalid value type." << std::endl;
                return;
            }
            std::string condition;
            if(iss >> token && token == "where"){
                while(iss >> token){
                    condition += token + " ";
                }
                condition = condition.substr(0,condition.size()-1);//移除末尾的空格
            }
            update_table(table_name, column_name, value, condition);
        }else if(token == "delete"){
            iss >> token;
            std::string table_name;
            iss >> table_name;
            std::string condition;
            if(iss>>token&& token == "where"){
                while(iss >> token){
                    condition += token + " ";
                }
                condition = condition.substr(0,condition.size()-1);//移除末尾的空格
            }
            delete_from_table(table_name, condition);
        }else{
            std::cerr << "Error: Command not recognized." << std::endl;
        }
    }
}
void terminal::exit(){
    if(current_database){
        close_db(); //确保在退出前关闭当前数据库
    }
    std::exit(0);
}

void terminal::create_db(const std::string& db_name,const std::string& password){
    if(current_database){
        std::cerr << "Error: A database is already in use. Please close it before creating another one." << std::endl;
        return;
    }
    if(db_name.empty()){
        std::cerr << "Error: Database name cannot be empty." << std::endl;
        return;
    }
    if(databases.find(db_name) != databases.end()){
        std::cerr << "Error: Database " << db_name << " already exists." << std::endl;
        return;
    }
    //在data目录下创建以数据库名命名的目录
    std::filesystem::path db_path = std::filesystem::current_path() / db_name;
    if(!std::filesystem::exists(db_path)){
        if(std::filesystem::create_directory(db_path)){
            databases.insert(db_name);
            if(!password.empty()){
                std::ofstream pass_file(db_path / "password.txt");
                if(!pass_file){
                    std::cerr <<"Error: Could not create password file for database "<< db_name << "."<<std::endl;
                    return;
                }
                pass_file << password;
            }
            std::cout << "Database "<< db_name << " created successfully." << std::endl;

        }else{
            std::cerr << "Error: Could not create database directory." <<std::endl;
            return;
        }
    }

}

void terminal::drop_db(const std::string& db_name){
    //或许之后添加一个线程操作
    if(current_database){
        std::cerr << "Error: A database is currently in use. Please close it before dropping another one." << std::endl;
        return;
    }
    if(db_name.empty()){
        std::cerr << "Error: Database name cannot be empty." << std::endl;
        return;
    }
    if(databases.find(db_name) == databases.end()){
        std::cerr << "Error: Database " << db_name << " does not exist." << std::endl;
        return;
    }
    std::filesystem::path db_path = std::filesystem::current_path() / db_name;
    std::filesystem::path pass_path= db_path / "password.txt";
    if(std::filesystem::exists(pass_path)){
        std::cout<<"Please enter the password for database "<< db_name <<": "<<std::endl;
        std::string input_password;
        std::cin >> input_password;
        std::ifstream pass_file(pass_path);
        std::string stroed_password;
        if(pass_file){
            std::getline(pass_file,stroed_password);
        }
        if(input_password != stroed_password){
            std::cerr << "Error: Incorrect password." << std::endl;
            return;
        }
    }
    if(std::filesystem::remove_all(db_path)){
        databases.erase(db_name);
        std::cout << "Database "<< db_name << " dropped successfully." << std::endl;
    }else{
        std::cerr << "Error: Could not drop database "<< db_name << "." << std::endl;
    }

}

void terminal::use_db(const std::string& db_name){
    if(current_database){
        std::cerr << "Error: A database is already in use. Please close it before using another one." << std::endl;
        return;
    }
    if(db_name.empty()){
        std::cerr << "Error: Database name cannot be empty." << std::endl;
        return;
    }
    if(databases.find(db_name) == databases.end()){
        std::cerr << "Error: Database " << db_name << " does not exist." << std::endl;
        return;
    }
    std::filesystem::path db_path = std::filesystem::current_path() / db_name;
    std::filesystem::path pass_path= db_path / "password.txt";
    std::string input_password;
    if(std::filesystem::exists(pass_path)){
        std::cout<<"Please enter the password for database "<< db_name <<": "<<std::endl;
        std::string input_password;
        std::cin >> input_password;
        std::ifstream pass_file(pass_path);
        std::string stroed_password;
        if(pass_file){
            std::getline(pass_file,stroed_password);
        }
        if(input_password != stroed_password){
            std::cerr << "Error: Incorrect password." << std::endl;
            return;
        }
    }
    current_db = db_name;
    //具体的装入内存操作交给database类来处理
    current_database = std::make_unique<database>(db_name,input_password);
    std::cout << "Using database: " << db_name << std::endl;
    //将当前目录切换到数据库目录下
    std::filesystem::current_path(db_path);
    //更新提示符

    prompt = "dblite("+db_name + ")> ";
    std::cout << "Switched to database: " << db_name << std::endl;
}

void terminal::create_table(const std::string& table_name,const std::string& column_definitions){
    if(!current_database){
        std::cerr<<"Error: No database is currently in use."<<std::endl;
        return;
    }
    if(table_name.empty()){
        std::cerr << "Error: Table name cannot be empty."<<std::endl;
        return;
    }
    if(current_database->tables.find(table_name) != current_database->tables.end()){
        std::cerr << "Error: Table "<< table_name << " already exists."<<std::endl;
        return;
    }
    //解析列定义
    std::vector<std::string> column_names;
    std::vector<type> column_types;
    std::istringstream iss(column_definitions);
    std::string column_definition;
    int index =0;
    int primary_key_index = -1;

    while(std::getline(iss,column_definition,',')){
        std::istringstream col_stream(column_definition);
        std::string column_name;
        std::string token;
        std::vector<std::string> col;
        while(col_stream >> token){
            std::string pk;
            if(token == "primary"){
                col_stream >> pk;
                if(pk != "key"){
                    std::cerr << "Error: Invalid primary key definition."<<std::endl;
                    return;
                }
                if(primary_key_index !=-1){
                    std::cerr << "Error: Multiple primary keys are not allowed." << std::endl;
                    return;
                }
                token+= " "+ pk; //将 primary key 合并为一个 token
                primary_key_index = index; //设置主键索引
                continue; //跳过 primary key 关键字
            }
            col.push_back(token);
        }
        if(col.size() < 2){
            std::cerr << "Error: Invalid column definition: " << column_definition << std::endl;
            return;
        }

        index++;
        for(size_t i =0;i<col.size();++i){
            //表示不是关键字,那么就是列名
            if(keywords.find(col[i])==keywords.end()){
                column_names.push_back(col[i]);
                continue;
            }
            if(col[i] == "int"){
                column_types.push_back(type::INT);
            }else if(col[i] == "double"){
                column_types.push_back(type::DOUBLE);
            }else if(col[i] == "string"){
                column_types.push_back(type::STRING);
            }else{
                std::cerr << "Error: Invalid column type: " << col[i] << std::endl;
                return;
            }
        }
    }

    if(column_names.empty()){
        std::cerr << "Error: No valid columns defined." << std::endl;
        return;
    }
    auto new_table = std::make_shared<table>(table_name, column_names,column_types,primary_key_index);
    //将表定义写入文件

    
    //先创建一个表文件
    std::string file_name= table_name + ".tbl";
    std::ofstream table_file(file_name);
    for(auto& col_name:column_names){
        table_file << col_name << " ";
    }
    table_file << std::endl;
    for(auto& col_type:column_types){
        table_file << static_cast<int>(col_type)<<" ";
    }
    table_file << std::endl;
    table_file<<primary_key_index<<std::endl;
    table_file.close();
    //新创建的表没有数据，所以不需要写入数据
    std::cout << "Table " << table_name << " created successfully." << std::endl;
    //更新当前数据库的状态
    current_database->tables[table_name] = new_table;

   

}
void terminal::drop_table(const std::string& table_name){
    if(!current_database){
        std::cerr << "Error: No database is currently in use." << std::endl;
        return;
    }
    if(table_name.empty()){
        std::cerr << "Error: Table name cannot be empty." << std::endl;
        return;
    }
    if(current_database->tables.find(table_name) == current_database->tables.end()){
        std::cerr << "Error: Table " << table_name << "does not exist." << std::endl;
        return;
    }
    //删除表文件
    std::filesystem::path table_path = std::filesystem::current_path() / (table_name + ".tbl");
    if(std::filesystem::exists(table_path)){
        if(std::filesystem::remove(table_path)){
            current_database->tables.erase(table_name);
            std::cout << "Table "<<table_name << " dropped successfully." << std::endl;
        }else{
            std::cerr << "Error: Could not drop table "<< table_name <<"." <<std::endl;
            return;
        }
    }else{
        std::cerr << "Error: Table " << table_name << " does not exist." << std::endl;
        return;
    }


}

void terminal::close_db(){
    if(!current_database){
        std::cerr << "Error: No database is currently in use." << std::endl;
        return;
    }
    current_database->save_tables(); //保存当前数据库的状态
    current_database.reset();
    current_db = "";
    prompt = "dblite> "; //重置提示符
    std::cout << "Database closed successfully." << std::endl;

    std::filesystem::current_path(data_path); //切换回data目录
}
void terminal::ls_table(){
    if(!current_database){
        std::cerr << "Error: No database is currently in use." << std::endl;
        return;
    }
    std::cout << "Tables in database " << current_db << ":" << std::endl;
    for(const auto& [table_name, table_ptr] : current_database->tables){
        std::cout << "- " << table_name << std::endl;
    }
}









//new 6.2
//通过文件查找数据库中查找表
std::pair<bool,std::filesystem::path> find_table_infile(const std::string& table_name,const std::string& db_name){
    std::string table_name_with_path = table_name+".tbl";
    std::filesystem::path table_path = std::filesystem::current_path()/table_name_with_path ;
    if(std::filesystem::exists(table_path)){
       
        return {true,table_path};
    }
    else{
        
        return {false,table_path};
    }
}



void terminal::insert_into_table(const std::string& table_name,const row& values){
     std::string& db_name = current_db;
     if(db_name.empty())
     {
          std::cerr << "Error: No database is currently in use." << std::endl;
     }
     //在文件中查找表
     std::pair<bool,std::filesystem::path> find_result = find_table_infile(table_name,db_name);
     if(!(find_result.first))
     {
        
        std::cerr << "Table "<< table_name << " not found in systemfile of "<< find_result.second << "." << std::endl;
        return;
     }

     //通过数据结构的数据库查找表
     std::shared_ptr<table> cur_table = current_database->get_table(table_name);
     if(cur_table==nullptr){
        std::cerr << "Table "<< table_name << " not found in database "<< db_name << "." << std::endl;
        return;
     }
     else
     {
        //插入数据
    
        if(values.size()!=cur_table->get_column_names().size())
        {
            std::cerr << "The number of values"<< values.size()<< "does not match the number of columns."<< cur_table->get_column_names().size()<< std::endl;
            return;
        }
        //进入表的层面添加数据，传入数据和了文件路径
        current_database->get_table(table_name)->insert_row(values);
     }
     
}

//TO DO
void terminal::select_from_table(const std::string& table_name,const std::vector<std::string>& column_names,
                                 const std::string& condition)

            {
                  if(current_database.get()==nullptr)
                    {
                        std::cerr << "Error: No database is currently in use." << std::endl;
                    }
            //获取条件
            std::vector<std::string> condition_parts;
            //获取要查询的表
            auto cur_table = current_database->get_table(table_name);
         
            //无条件，则查找全表
            if(condition.empty())
            {
                std::cout << "Selecting all rows from table" << table_name << "..." << std::endl;
                
                if(!current_database)
                {
                    std::cerr << "Error: No database is currently in use." << std::endl;
                    return;
                }
                if(cur_table.get()==nullptr)
                {
                    std::cerr << "Table "<< table_name << " not found in database "<< current_db << "." << std::endl;
                    return;

                }
            
                 cur_table->select_column(column_names,condition_parts);
                
                
            }
            else{
                //有条件，则查找满足条件的数据
                //先解析条件
              
               condition_parts = split_by_space(condition);
               if(condition_parts.size()!=3)
               {
                   std::cerr << "\033[31mError: Invalid condition.\033[0m" << std::endl;
                   return;
               }
               //获取列索引
               int coulmn_index=cur_table->get_column_index(condition_parts[0]);
               auto [condition_type,condition_value] = get_type(condition_parts[2]);
                //判断列类型和条件类型是否相等
               if(cur_table->get_column_types()[coulmn_index]!=condition_type)
               {
                std::cout<<type_to_string(cur_table->get_column_types()[coulmn_index])<<type_to_string(condition_type)<<std::endl;
                std::cerr << "\033[31mError: wrong type in condition.\033[0m" << std::endl;
                return;
               }

                std::cout << "Selecting rows from table " << table_name << " where " << condition << "..." << std::endl;
                // 目前在只支持一个的列名
              
                cur_table->select_column(column_names,condition_parts);
                return;
            }
            }
void terminal::update_table(const std::string& table_name,const std::string& column_name,
                           const record& value, const std::string& condition){
                  
        
                if(current_database.get()==nullptr)
                    {
                        std::cerr << "Error: No database is currently in use." << std::endl;
                    }
             //获取条件
            std::vector<std::string> condition_parts;
            //获取要更新的表
            auto cur_table = current_database->get_table(table_name);
         
            //无条件则更新所有
            if(condition.empty())
            {
                std::cout << "update all rows from table " << table_name << "..." << std::endl;
                
                if(!current_database)
                {
                    std::cerr << "Error: No database is currently in use." << std::endl;
                    return;
                }
                if(cur_table.get()==nullptr)
                {
                    std::cerr << "Table "<< table_name << " not found in database "<< current_db << "." << std::endl;
                    return;

                }
            
                 cur_table->update_row(column_name,value,condition_parts);
                
                
            }
            else{
                //有条件，则查找满足条件的数据
                //先解析条件
              
               condition_parts = split_by_space(condition);
               if(condition_parts.size()!=3)
               {
                   std::cerr << "\033[31mError: Invalid condition.\033[0m" << std::endl;
                   return;
               }
               //获取列索引
               int coulmn_index=cur_table->get_column_index(condition_parts[0]);
               auto [condition_type,condition_value] = get_type(condition_parts[2]);
                //判断列类型和条件类型是否相等
               if(cur_table->get_column_types()[coulmn_index]!=condition_type)
               {
                std::cerr<<type_to_string(cur_table->get_column_types()[coulmn_index])<<type_to_string(condition_type)<<std::endl;
                std::cerr << "\033[31mError: wrong type in condition.\033[0m" << std::endl;
                return;
               }

                std::cout << "update row from table " << table_name << " where " << condition << "..." << std::endl;
                // 目前在只支持一个的列名
              
                cur_table->update_row(column_name,value,condition_parts);
                return;
            }
 }
void terminal::delete_from_table(const std::string& table_name, const std::string& condition){

              if(current_database.get()==nullptr)
                    {
                        std::cerr << "Error: No database is currently in use." << std::endl;
                    }
    //获取条件
            std::vector<std::string> condition_parts;
            //获取要查询的表
            auto cur_table = current_database->get_table(table_name);
         
            //无条件，则是删除全表
            if(condition.empty())
            {
                std::cout << "Selecting all rows from table " << table_name << "..." << std::endl;
                
                if(!current_database)
                {
                    std::cerr << "Error: No database is currently in use." << std::endl;
                    return;
                }
                if(cur_table.get()==nullptr)
                {
                    std::cerr << "Table "<< table_name << " not found in database "<< current_db << "." << std::endl;
                    return;

                }
            
                 cur_table->delete_all();
                
                
            }
            else{
                //有条件，则删除满足条件的数据
                //先解析条件
              
               condition_parts = split_by_space(condition);
               if(condition_parts.size()!=3)
               {
                   std::cerr << "Error: Invalid condition." << std::endl;
                   return;
               }
               //获取列索引
               int coulmn_index=cur_table->get_column_index(condition_parts[0]);
               auto [condition_type,condition_value] = get_type(condition_parts[2]);
                //判断列类型和条件类型是否相等
               if(cur_table->get_column_types()[coulmn_index]!=condition_type)
               {
                std::cout<<type_to_string(cur_table->get_column_types()[coulmn_index])<<type_to_string(condition_type)<<std::endl;
                std::cerr << "Error: wrong type in condition." << std::endl;
                return;
               }

                std::cout << "Deleting rows from table " << table_name << " where " << condition << "..." << std::endl;
              
                cur_table->delete_row(condition_parts);
                return;
            }
}
