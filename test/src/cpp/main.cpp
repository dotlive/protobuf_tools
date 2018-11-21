#include <iostream>
#include <fstream>
#include <string>

#include "rapidxml/rapidxml.hpp"
#include "rapidxml/rapidxml_utils.hpp"

#include "global.h"
#include "string_utils.h"
#include "protobuf_utils.h"


int main(int argc, char* argv[])
{
    GOOGLE_PROTOBUF_VERIFY_VERSION;

    std::string file_name("message.pb");
    std::string path = MakePath(file_name.c_str(), Global::CppRoot);

    ProtobufUtils::Encode(
        "MESSAGE",
        "1,2,3,4,5,6,abc,8,9,10,true,12,13,中国",
        path.c_str());

    ProtobufUtils::Decode(
        "MESSAGE",
        path.c_str(),
        "");

    ProtobufUtils::Decode(
        "MESSAGE",
        MakePath(file_name.c_str(), Global::LuaRoot).c_str(),
        "");

    getchar();
    return 0;
}
