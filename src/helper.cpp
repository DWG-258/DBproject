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