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
using namespace std;
string trim(const string &text)
{
    size_t left = 0;
    size_t right = text.size();
    while (left < right && isspace((unsigned char)text[left]))
        ++left;
    while (right > left && isspace((unsigned char)text[right - 1]))
        --right;
    return text.substr(left, right - left);
}
template <typename T>
typename enable_if<is_integral<T>::value &&!is_same<T, bool>::value, bool>::type
parse_number(const string &text, T &value)
{
    string input = trim(text);
    if (input.empty())
        return false;
    if (input[0] == '+')
        input.erase(0, 1);
    if (input.empty())
        return false;

    T result{};
    const char *begin = input.data();
    const char *end = begin + input.size();
    from_chars_result result_info = from_chars(begin, end, result);
    if (result_info.ec != errc() || result_info.ptr != end)
        return false;
    value = result;
    return true;
}

template <typename T>
typename enable_if<is_floating_point<T>::value, bool>::type
parse_number(const string &text, T &value)
{
    string input = trim(text);
    if (input.empty())
        return false;

    char *end = nullptr;
    errno = 0;
    long double result = strtold(input.c_str(), &end);
    if (end == input.c_str() || *end != '\0' || errno == ERANGE ||
        !isfinite(result))
        return false;

    long double min_value = static_cast<long double>(
        numeric_limits<T>::lowest());
    long double max_value = static_cast<long double>(
        numeric_limits<T>::max());
    if (result < min_value || result > max_value)
        return false;

    T converted = static_cast<T>(result);
    if (!isfinite(converted))
        return false;
    value = converted;
    return true;
}
template <typename T>
typename enable_if<is_integral<T>::value&& !is_same<T, bool>::value, bool>::type
safe_input(T &value, const string &prompt = "")
{
    string line;
    while (true)
    {
        cout << prompt;
        if (!getline(cin, line))
        {
            cout << '\n';
            return false;
        }
        if (parse_number(line, value))
            return true;
        cout << "输入无效或超出范围，请重新输入。\n";
    }
}

template <typename T>
typename enable_if<is_floating_point<T>::value, bool>::type
safe_input(T &value, const string &prompt = "")
{
    string line;
    while (true)
    {
        cout << prompt;
        if (!getline(cin, line))
        {
            cout << '\n';
            return false;
        }
        if (parse_number(line, value))
            return true;
        cout << "输入无效、超出范围或不是有限浮点数，请重新输入。\n";
    }
}

inline bool safe_input(string &value, const string &prompt = "")
{
    cout << prompt;
    if (!getline(cin, value))
    {
        cout << '\n';
        return false;
    }
    return true;
}

inline bool safe_input(char &value, const string &prompt = "")
{
    string line;
    while (true)
    {
        cout << prompt;
        if (!getline(cin, line))
        {
            cout << '\n';
            return false;
        }
        line = trim(line);
        if (line.size() == 1)
        {
            value = line[0];
            return true;
        }
        cout << "请输入一个字符，请重新输入。\n";
    }
}
#endif
