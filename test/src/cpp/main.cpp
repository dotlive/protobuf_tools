#include <iostream>
#include <fstream>
#include <string>

#include "utility.h"
#include "string_utils.h"
#include "protobuf_utils.h"

#include "config_reader.h"


static std::string GetEncodeFile(const char* msg_type, const char* data_key)
{
    std::string full_name(Options::outputDir());
    full_name.append(Options::cppDir());
    full_name.append(msg_type);
    full_name.append(Options::nameSpliter());
    full_name.append(data_key);
    full_name.append(Options::extensionName());
    return full_name;
}

int main(int argc, char* argv[])
{
    GOOGLE_PROTOBUF_VERIFY_VERSION;

    ConfigReader cr;
    std::string output_file;

    for (const char* msg_type = cr.FirstMesage(); !StringUtils::IsNullOrEmpty(msg_type); msg_type = cr.NextMessage())
    {
        const MessageData* msg_data = cr.GetMessageData(msg_type);
        if (msg_data == NULL)
        {
            continue;
        }

        for (int idx = 0, max = msg_data->Count(); idx < max; ++idx)
        {
            const DataUnit* du = msg_data->Get(idx);
            if (du == NULL)
            {
                continue;
            }

            output_file = GetEncodeFile(msg_type, du->Key());
            Utility::CreateDirectory(output_file.c_str(), true);

            ProtobufUtils::Encode(msg_type, du->Value(), output_file.c_str());
        }
    }

    getchar();
    return 0;
}
