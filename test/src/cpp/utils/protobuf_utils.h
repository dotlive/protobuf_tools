#ifndef _PROTOBUF_UTILS_H_
#define _PROTOBUF_UTILS_H_

#include <google/protobuf/message.h>
#include <google/protobuf/descriptor.h>

// Protobuf 辅助类

class ProtobufUtils
{
public:
    static void Encode(const char* type_name, const char* input_data, const char* output_path);
    static void Decode(const char* type_name, const char* input_path, const char* output_path);

    static void SetFieldAll(const char* value, google::protobuf::Message& msg);
    static void GetFieldAll(const google::protobuf::Message& msg, std::string& value);

private:
    // 调用方需要释放内存
    static google::protobuf::Message* CreateMessage(const char* type_name);
    static void PrintDescriptor(const google::protobuf::Message& msg);
};

#endif // _PROTOBUF_UTILS_H_
