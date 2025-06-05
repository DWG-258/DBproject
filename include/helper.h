#pragma once
#include <string>
#include <stdexcept>
#include <optional>
#include <iostream>
#include <sstream>
#include <vector>
#include <string>
#include "type.h"
#include <variant>
bool is_double_(const std::string& str);

using value = std::variant<int, double,std::string>;

bool is_int_(const std::string& str);
//查看是否在引号内，如果在的话，将引号内的内容返回，如果不在的话返回空字符串
std::optional<std::string> in_quotation(const std::string& str);

std::vector<std::string> split_by_space(const std::string& str);
 
std::pair<type,value> get_type(const std::string& str);