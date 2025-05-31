#include"terminal.h"
#include"helper.h"
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
        }else if(current_command=="help"){
            help();
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
                        if(is_int_(_token)){
                            values_row.push_back(std::stoi(_token));
                        }else if(is_double_(_token)){
                            values_row.push_back(std::stod(_token));
                        }else{
                            auto quoted_value = in_quotation(_token);
                            if(quoted_value!= std::nullopt){
                                values_row.push_back(*quoted_value);
                            }else{
                                std::cerr << "Error: Invalid value type."<<std::endl;
                                return;
                            }
                        }
                        insert_into_table(table_name, values_row);

                    }
                }else{
                    std::cerr << "Error: Invalid command syntax."<<std::endl;
                    return;
                }
            }
        }else if(token == "select"){
            std::vector<std::string> column_names;
            std::string table_name;
            while(iss>>token){
                if(token == "from"){
                    iss >> table_name;
                }else if(token == "where"){
                    std::string condition;
                    while(iss >> token){
                        condition +=token + " ";
                    }
                    condition = condition.substr(0,condition.size()-1);//移除末尾的空格
                    select_from_table(table_name, column_names, condition);
                }else{
                    if(token !=","){
                        column_names.push_back(token);
                    }

                }
            }
        }else if(token == "update"){
            std::string table_name;
            iss >> table_name;
            std::string column_name;
            iss >> column_name;
            iss >> token;
            if(token != "set"){
                std::cerr << "Error: Invalid command syntax." << std::endl;
                return;
            }
            std::string value_str;
            iss >> value_str;
            record value;
            if(is_int_(value_str)){
                value = std::stoi(value_str);
            }else if(is_double_(value_str)){
                value = std::stod(value_str);
            }else if(auto quoted_value = in_quotation(value_str);quoted_value!=std::nullopt){
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
    std::exit(0);
}

void terminal::create_db(const std::string& db_name,const std::string& password){
    if(!db_test(db_name)){
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
    if(!db_test(db_name)){
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
    if(!db_test(db_name)){
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

    std::cout << "Switched to database: " << db_name << std::endl;
}

void terminal::create_table(const std::string& table_name,const std::string& column_definitions){}
void terminal::drop_table(const std::string& table_name){}
void terminal::insert_into_table(const std::string& table_name,const row& values){}
void terminal::select_from_table(const std::string& table_name,const std::vector<std::string>& column_names,
                                 const std::string& condition){}
void terminal::update_table(const std::string& table_name,const std::string& column_name,
                           const record& value, const std::string& condition){}
void terminal::delete_from_table(const std::string& table_name, const std::string& condition){}
