#include"terminal.h"
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
    }
}
void terminal::exit(){
    std::exit(0);
}