#ifndef CCPP2026_SAFE_INPUT_H
#define CCPP2026_SAFE_INPUT_H

#include <charconv>
#include <cerrno>
#include <cmath>
#include <cstdlib>
#include <cctype>
#include <iostream>
#include <limits>
#include <sstream>
#include <string>
#include <type_traits>

namespace safe_input_detail
{
inline std::string trim(const std::string &text)
{
    size_t left = 0;
    size_t right = text.size();
    while (left < right && std::isspace((unsigned char)text[left]))
        ++left;
    while (right > left && std::isspace((unsigned char)text[right - 1]))
        --right;
    return text.substr(left, right - left);
}

template <typename T>
typename std::enable_if<std::is_integral<T>::value &&
                        !std::is_same<T, bool>::value, bool>::type
parse_number(const std::string &text, T &value)
{
    std::string input = trim(text);
    if (input.empty())
        return false;
    if (input[0] == '+')
        input.erase(0, 1);
    if (input.empty())
        return false;

    T result{};
    const char *begin = input.data();
    const char *end = begin + input.size();
    std::from_chars_result result_info = std::from_chars(begin, end, result);
    if (result_info.ec != std::errc() || result_info.ptr != end)
        return false;
    value = result;
    return true;
}

template <typename T>
typename std::enable_if<std::is_floating_point<T>::value, bool>::type
parse_number(const std::string &text, T &value)
{
    std::string input = trim(text);
    if (input.empty())
        return false;

    char *end = nullptr;
    errno = 0;
    long double result = std::strtold(input.c_str(), &end);
    if (end == input.c_str() || *end != '\0' || errno == ERANGE ||
        !std::isfinite(result))
        return false;

    long double min_value = static_cast<long double>(
        std::numeric_limits<T>::lowest());
    long double max_value = static_cast<long double>(
        std::numeric_limits<T>::max());
    if (result < min_value || result > max_value)
        return false;

    T converted = static_cast<T>(result);
    if (!std::isfinite(converted))
        return false;
    value = converted;
    return true;
}
}

template <typename T>
typename std::enable_if<std::is_integral<T>::value&& !std::is_same<T, bool>::value, bool>::type
safe_input(T &value, const std::string &prompt = "")
{
    std::string line;
    while (true)
    {
        std::cout << prompt;
        if (!std::getline(std::cin, line))
        {
            std::cout << '\n';
            return false;
        }
        if (safe_input_detail::parse_number(line, value))
            return true;
        std::cout << "输入无效或超出范围，请重新输入。\n";
    }
}

template <typename T>
typename std::enable_if<std::is_floating_point<T>::value, bool>::type
safe_input(T &value, const std::string &prompt = "")
{
    std::string line;
    while (true)
    {
        std::cout << prompt;
        if (!std::getline(std::cin, line))
        {
            std::cout << '\n';
            return false;
        }
        if (safe_input_detail::parse_number(line, value))
            return true;
        std::cout << "输入无效、超出范围或不是有限浮点数，请重新输入。\n";
    }
}

inline bool safe_input(std::string &value, const std::string &prompt = "")
{
    std::cout << prompt;
    if (!std::getline(std::cin, value))
    {
        std::cout << '\n';
        return false;
    }
    return true;
}

inline bool safe_input(char &value, const std::string &prompt = "")
{
    std::string line;
    while (true)
    {
        std::cout << prompt;
        if (!std::getline(std::cin, line))
        {
            std::cout << '\n';
            return false;
        }
        line = safe_input_detail::trim(line);
        if (line.size() == 1)
        {
            value = line[0];
            return true;
        }
        std::cout << "请输入一个字符，请重新输入。\n";
    }
}

#endif
