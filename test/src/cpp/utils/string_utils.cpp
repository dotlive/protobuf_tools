#include "string_utils.h"
#include <windows.h>


char* StringUtils::UtfToGbk(const char* utf8)
{
    int len = MultiByteToWideChar(CP_UTF8, 0, utf8, -1, NULL, 0);
    wchar_t* wstr = new wchar_t[len+1];
    memset(wstr, 0, len+1);
    MultiByteToWideChar(CP_UTF8, 0, utf8, -1, wstr, len);
    len = WideCharToMultiByte(CP_ACP, 0, wstr, -1, NULL, 0, NULL, NULL);
    char* str = new char[len+1];
    memset(str, 0, len+1);
    WideCharToMultiByte(CP_ACP, 0, wstr, -1, str, len, NULL, NULL);
    if(wstr) delete[] wstr;
    return str;
}

char* StringUtils::GbkToUtf(const char* gb2312)
{
    int len = MultiByteToWideChar(CP_ACP, 0, gb2312, -1, NULL, 0);
    wchar_t* wstr = new wchar_t[len+1];
    memset(wstr, 0, len+1);
    MultiByteToWideChar(CP_ACP, 0, gb2312, -1, wstr, len);
    len = WideCharToMultiByte(CP_UTF8, 0, wstr, -1, NULL, 0, NULL, NULL);
    char* str = new char[len+1];
    memset(str, 0, len+1);
    WideCharToMultiByte(CP_UTF8, 0, wstr, -1, str, len, NULL, NULL);
    if(wstr) delete[] wstr;
    return str;
}

size_t StringUtils::Split(std::vector<std::string>& result, const std::string& origin, const std::string& spliter)
{
    if (origin == "")
    {
        return 0;
    }

    std::string strs(origin);
    strs += spliter;

    size_t pos;
    size_t size = strs.size();

    for (size_t i = 0; i < size; ++i)
    {
        pos = strs.find(spliter, i);
        if (pos < size)
        {
            std::string s = strs.substr(i, pos - i);
            result.push_back(s);
            i = pos + spliter.length() - 1;
        }

    }

    return result.size();
}

int StringUtils::ToInt(const char* value)
{
    return ::atoi(value);
}

bool StringUtils::ToBool(const char* value)
{
    if (0 == strcmp(value, "true"))
        return true;
    else
        return false;
}

void StringUtils::FromInt(int value, std::string& result)
{
    char buff[128] = { 0 };
    result += _itoa(value, buff, 10);
}

void StringUtils::FromBool(bool value, std::string& result)
{
    result += value ? "true" : "false";
}
