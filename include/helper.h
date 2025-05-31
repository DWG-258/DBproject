#pragma once
#include <string>
#include <stdexcept>
#include <optional>

bool is_double_(const std::string& str);

bool is_int_(const std::string& str);
//查看是否在引号内，如果在的话，将引号内的内容返回，如果不在的话返回空字符串
std::optional<std::string> in_quotation(const std::string& str);
