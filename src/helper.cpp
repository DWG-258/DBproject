#include "helper.h"
bool is_double_(const std::string& str){
    try{
        std::stod(str);
        return true;
    }catch(const std::invalid_argument&){
        return false;
    }catch(const std::out_of_range&){
        return false;
    }
}

bool is_int_(const std::string& str){
    try{
        std::stoi(str);
        return true;
    }catch(const std::invalid_argument&){
        return false;
    }catch(const std::out_of_range&){
        return false;
    }
}
//查看是否在引号内，如果在的话，将引号内的内容返回，如果不在的话返回空字符串
std::optional<std::string> in_quotation(const std::string& str){
    size_t start=str.find("\"");
    size_t end=str.find("\"",start + 1);
    if(start !=std::string::npos && end!=std::string::npos && end > start){
        return str.substr(start + 1,end-start -1);
    }
    return std::nullopt;
}

std::vector<std::string> split_by_space(const std::string& str)
{
    std::vector<std::string> result;
    std::istringstream iss(str);
    std::string word;

    while (iss >> word) {  // 自动跳过连续空格
        result.push_back(word);
    }

    return result;
}

std::pair<type,value> get_type(const std::string& str)
{
     if(str.find('.') != std::string::npos)
     {
         return {type::DOUBLE,std::stod(str)};
     }
     else if(is_int_(str))
     {
         return {type::INT,std::stoi(str)};
     }
     else{
        return {type::STRING,str};
     }
     
}

std::string type_to_string(type type) {
    switch (type) {
        case type::INT:    return "Int";
        case type::DOUBLE: return "Double";
        case type::STRING: return "String";
        default:                return "Unknown";
    }
}