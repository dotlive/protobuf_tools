#include "utility.h"

#include <stdlib.h>
#include <string>

#include "string_utils.h"

#ifdef _WIN32
    #include <direct.h>
    #include <io.h>
#elif _LINUX
    #include <stdarg.h>
    #include <sys/stat.h>
#endif

#ifdef _WIN32
    #define ACCESS _access
    #define MKDIR(a) _mkdir((a))
#elif _LINUX
    #define ACCESS access
    #define MKDIR(a) mkdir((a),0755)
#endif


bool Utility::CreateDirectory(const char* path, bool is_file)
{
    if (StringUtils::IsNullOrEmpty(path))
    {
        return false;
    }

    char* temp_path = "";
    if (is_file)
    {
        std::string temp(path);

        // 确保最后一个分割符为 '/'
        size_t pos = temp.rfind('\\');
        if (pos != std::string::npos)
        {
            temp = temp.replace(temp.rfind('\\'), 1, "/");
        }

        // 获取文件所在目录名
        pos = temp.rfind('/');
        if (pos != std::string::npos)
        {
            temp = temp.substr(0, pos);
            temp_path = _strdup(temp.c_str());
        }
    }
    else
    {
        temp_path = _strdup(path);
    }

    if (StringUtils::IsNullOrEmpty(temp_path))
    {
        return false;
    }

    int temp_ret = 0;

    for (int i = 0, len = strlen(temp_path); i < len; ++i)
    {
        if (temp_path[i] == '\\' || temp_path[i] == '/')
        {
            temp_path[i] = '\0';

            temp_ret = ACCESS(temp_path, 0);
            if (temp_ret != 0)
            {
                temp_ret = MKDIR(temp_path);
                if (temp_ret != 0)
                {
                    return -1;
                }
            }
            temp_path[i] = '/';
        }
    }

    temp_ret = MKDIR(temp_path);
    free(temp_path);

    return temp_ret == 0;
}
