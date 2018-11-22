#ifndef _STRING_UTILS_H_
#define _STRING_UTILS_H_

#include <vector>
#include <string>

// 字符串操作相关辅助接口类

class StringUtils
{
public:
    static char* UtfToGbk(const char* utf8);
    static char* GbkToUtf(const char* gb2312);

    static size_t Split(std::vector<std::string>& result, const std::string& origin, const std::string& spliter);
    static bool IsNullOrEmpty(const char* value);

    static int ToInt(const char* value);
    static bool ToBool(const char* value);

    static void FromInt(int value, std::string& result);
    static void FromBool(bool value, std::string& result);
};

#endif // _STRING_UTILS_H_
