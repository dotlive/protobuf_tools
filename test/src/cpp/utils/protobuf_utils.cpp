#include <fstream>
#include "protobuf_utils.h"
#include "string_utils.h"

using namespace google::protobuf;


void ProtobufUtils::Encode(const char* type_name, const char* input_data, const char* output_path)
{
    Message* msg_ptr = CreateMessage(type_name);
    if (msg_ptr == NULL)
    {
        return;
    }

    SetFieldAll(input_data, *msg_ptr);

    std::fstream out(output_path, std::ios::out | std::ios::binary | std::ios::trunc);
    msg_ptr->SerializeToOstream(&out);
    out.close();

    delete msg_ptr;
}

void ProtobufUtils::Decode(const char* type_name, const char* input_path, const char* output_path)
{
    Message* msg_ptr = CreateMessage(type_name);
    if (msg_ptr == NULL)
    {
        return;
    }

    std::fstream in(input_path, std::ios::in | std::ios::binary);
    if (!msg_ptr->ParseFromIstream(&in)) {
        std::cerr << "Failed to parse " << type_name << " from " << input_path << std::endl;
        exit(1);
    }

    std::string data;
    GetFieldAll(*msg_ptr, data);
    delete msg_ptr;

    std::cout << data << std::endl;
}

void ProtobufUtils::SetFieldAll(const char* value, google::protobuf::Message& msg)
{
    const Descriptor* descriptor = msg.GetDescriptor();
    const Reflection* reflection = msg.GetReflection();
    if (descriptor == NULL || reflection == NULL)
    {
        return;
    }

    int field_count = descriptor->field_count();

    std::vector<std::string> result;
    StringUtils::Split(result, value, ",");

    for (int idx = 0; idx < field_count; ++idx)
    {
        const FieldDescriptor* field_desc = descriptor->field(idx);
        if (field_desc == NULL)
        {
            continue;
        }

        FieldDescriptor::CppType cpptype = field_desc->cpp_type();
        FieldDescriptor::Label label = field_desc->label();

        switch (cpptype)
        {
        case FieldDescriptor::CPPTYPE_INT32:
            switch (label)
            {
            case google::protobuf::FieldDescriptor::LABEL_OPTIONAL:
            case google::protobuf::FieldDescriptor::LABEL_REQUIRED:
                reflection->SetInt32(&msg, field_desc, StringUtils::ToInt(result[idx].c_str()));
                break;
            case google::protobuf::FieldDescriptor::LABEL_REPEATED:
                //{
                //    int field_size = reflection->FieldSize(msg, field_desc);
                //    for (int idx_field = 0; idx_field < field_size; ++idx_field)
                //    {
                //        reflection->SetRepeatedInt32(&msg, field_desc, idx_field, 0);
                //    }
                //}
                break;
            }
            break;
        case FieldDescriptor::CPPTYPE_INT64:
            reflection->SetInt64(&msg, field_desc, 0);
            break;
        case FieldDescriptor::CPPTYPE_UINT32:
            reflection->SetUInt32(&msg, field_desc, 0);
            break;
        case FieldDescriptor::CPPTYPE_UINT64:
            reflection->SetUInt64(&msg, field_desc, 0);
            break;
        case FieldDescriptor::CPPTYPE_DOUBLE:
            reflection->SetDouble(&msg, field_desc, 0);
            break;
        case FieldDescriptor::CPPTYPE_FLOAT:
            reflection->SetFloat(&msg, field_desc, 0.0f);
            break;
        case FieldDescriptor::CPPTYPE_BOOL:
            reflection->SetBool(&msg, field_desc, StringUtils::ToBool(result[idx].c_str()));
            break;
        case FieldDescriptor::CPPTYPE_ENUM:
            reflection->SetEnumValue(&msg, field_desc, 0);
            break;
        case FieldDescriptor::CPPTYPE_STRING:
            reflection->SetString(&msg, field_desc, result[idx]);
            break;
        case FieldDescriptor::CPPTYPE_MESSAGE:
            break;
        default:
            break;
        }
    }
}

void ProtobufUtils::GetFieldAll(const google::protobuf::Message& msg, std::string& value)
{
    const Descriptor* descriptor = msg.GetDescriptor();
    const Reflection* reflection = msg.GetReflection();
    if (descriptor == NULL || reflection == NULL)
    {
        return;
    }

    int field_count = descriptor->field_count();

    for (int idx = 0; idx < field_count; ++idx)
    {
        const FieldDescriptor* field_desc = descriptor->field(idx);
        if (field_desc == NULL)
        {
            continue;
        }

        FieldDescriptor::CppType cpptype = field_desc->cpp_type();
        FieldDescriptor::Label label = field_desc->label();

        if (!value.empty())
        {
            value += ",";
        }

        switch (cpptype)
        {
        case FieldDescriptor::CPPTYPE_INT32:
            switch (label)
            {
            case google::protobuf::FieldDescriptor::LABEL_OPTIONAL:
            case google::protobuf::FieldDescriptor::LABEL_REQUIRED:
                StringUtils::FromInt(reflection->GetInt32(msg, field_desc), value);
                break;
            case google::protobuf::FieldDescriptor::LABEL_REPEATED:
                break;
            }
            break;
        case FieldDescriptor::CPPTYPE_INT64:
            reflection->GetInt64(msg, field_desc);
            break;
        case FieldDescriptor::CPPTYPE_UINT32:
            reflection->GetUInt32(msg, field_desc);
            break;
        case FieldDescriptor::CPPTYPE_UINT64:
            reflection->GetUInt64(msg, field_desc);
            break;
        case FieldDescriptor::CPPTYPE_DOUBLE:
            reflection->GetDouble(msg, field_desc);
            break;
        case FieldDescriptor::CPPTYPE_FLOAT:
            reflection->GetFloat(msg, field_desc);
            break;
        case FieldDescriptor::CPPTYPE_BOOL:
            StringUtils::FromBool(reflection->GetBool(msg, field_desc), value);
            break;
        case FieldDescriptor::CPPTYPE_ENUM:
            reflection->GetEnumValue(msg, field_desc);
            break;
        case FieldDescriptor::CPPTYPE_STRING:
            value += reflection->GetString(msg, field_desc).c_str();
            break;
        case FieldDescriptor::CPPTYPE_MESSAGE:
            break;
        default:
            break;
        }
    }
}

google::protobuf::Message* ProtobufUtils::CreateMessage(const char* type_name)
{
    Message* message = NULL;

    const Descriptor* descriptor = DescriptorPool::generated_pool()->FindMessageTypeByName(type_name);
    if (descriptor)
    {
        const Message* prototype = MessageFactory::generated_factory()->GetPrototype(descriptor);
        if (prototype)
        {
            message = prototype->New();
        }
    }

    return message;
}

void ProtobufUtils::PrintDescriptor(const google::protobuf::Message& msg)
{
    const Descriptor* descriptor = msg.GetDescriptor();
    if (descriptor == NULL)
    {
        return;
    }

    for (int idx = 0, max = descriptor->field_count(); idx < max; ++idx)
    {
        const FieldDescriptor* field_desc = descriptor->field(idx);
        if (field_desc == NULL)
        {
            continue;
        }

        FieldDescriptor::Label label = field_desc->label();
        const std::string& name = field_desc->name();
        int index = field_desc->index();
        FieldDescriptor::Type pbtype = field_desc->type();
        FieldDescriptor::CppType cpptype = field_desc->cpp_type();
        std::cout << "label = " << label
                  << ", name = " << name
                  << ", index = " << index
                  << ", pbType = " << pbtype
                  << ", cppType = " << cpptype
                  << std::endl;
    }
}
