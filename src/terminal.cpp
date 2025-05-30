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
                        if(is_int(_token)){
                            values_row.push_back(std::stoi(_token));
                        }else if(is_double(_token)){
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

                    }
                }else{
                    std::cerr << "Error: Invalid command syntax."<<std::endl;
                    return;
                }
            }
        }
    }
}
void terminal::exit(){
    std::exit(0);
}