#ifndef _GLOBAL_DEFINE_H_
#define _GLOBAL_DEFINE_H_

#include <string>

namespace Global
{
    // 文件根目录
    static const char* const FileRoot = "../../../data/";
    static const char* const CppRoot = "cpp/";
    static const char* const LuaRoot = "lua/";
};

inline std::string MakePath(const char* file, const char* language)
{
    if (file == NULL)
    {
        return "";
    }

    std::string temp(Global::FileRoot);
    temp.append(language);
    temp.append(file);
    return temp;
}

#endif // _GLOBAL_DEFINE_H_