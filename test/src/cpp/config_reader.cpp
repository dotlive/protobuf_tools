#include "config_reader.h"

#include "rapidxml/rapidxml.hpp"
#include "rapidxml/rapidxml_utils.hpp"

#include "string_utils.h"


//////////////////////////////////////////////////////////////////////////

bool Options::s_initialized = false;
Options::OptionData Options::m_data;

const std::string& Options::Query(const char* key)
{
    LoadOption();

    OptionDataCiter citer = m_data.find(key);
    if (citer == m_data.end())
    {
        return "";
    }

    return citer->second;
}

void Options::LoadOption()
{
    if (s_initialized)
    {
        return;
    }

    rapidxml::file<> fdoc("../option.xml");
    if (fdoc.size() == 0)
    {
        return;
    }

    rapidxml::xml_document<> doc;
    doc.parse<0>(fdoc.data());

    rapidxml::xml_node<> *root_node = doc.first_node();
    if (root_node != NULL)
    {
        for (rapidxml::xml_node<> *option_node = root_node->first_node();
            option_node != NULL;
            option_node = option_node->next_sibling())
        {
            const char* opt_key = option_node->first_attribute("key")->value();
            const char* opt_val = option_node->first_attribute("value")->value();
            m_data[opt_key] = opt_val;
        }
    }

    s_initialized = true;
}

//////////////////////////////////////////////////////////////////////////

DataUnit::DataUnit(const char* key, const char* value)
: m_key(key)
, m_value(value)
{
}

DataUnit::~DataUnit()
{
}

const char* DataUnit::Key() const
{
    return m_key.c_str();
}

const char* DataUnit::Value() const
{
    return m_value.c_str();
}

//////////////////////////////////////////////////////////////////////////

MessageData::MessageData()
{
    Clear();
}

MessageData::~MessageData()
{
}

void MessageData::Add(const DataUnit& data)
{
    m_dataList.push_back(data);
}

const DataUnit* MessageData::Get(size_t index) const
{
    if (index >= Count())
    {
        return NULL;
    }

    return &(m_dataList[index]);
}

size_t MessageData::Count() const
{
    return m_dataList.size();
}

void MessageData::Clear()
{
    m_dataList.clear();
}

bool MessageData::Exist(const char* key) const
{
    for (DataListCiter citer = m_dataList.begin();
        citer != m_dataList.end();
        ++citer)
    {
        if (0 == strcmp(key, citer->Key()))
        {
            return true;
        }
    }

    return false;
}

//////////////////////////////////////////////////////////////////////////

ConfigReader::ConfigReader(const char* path /*= ""*/)
: m_isInitialized(false)
{
    if (StringUtils::IsNullOrEmpty(path))
    {
        m_path = Options::outputDir();
        m_path.append("config.xml");
    }
    else
    {
        m_path = path;
    }

    // 延迟加载会破坏const限制, 所以还是预先加载了
    LazyInit();
}

ConfigReader::~ConfigReader()
{
}

const char* ConfigReader::FirstMesage()
{
    m_traversalIter = m_data.begin();
    if (m_traversalIter == m_data.end())
    {
        return NULL;
    }

    return m_traversalIter->first.c_str();
}

const char* ConfigReader::NextMessage()
{
    if (m_traversalIter == m_data.end())
    {
        return NULL;
    }

    ++m_traversalIter;
    if (m_traversalIter == m_data.end())
    {
        return NULL;
    }

    return m_traversalIter->first.c_str();
}

const MessageData* ConfigReader::GetMessageData(const char* message_type_name) const
{
    MessageDataGroupCiter citer = m_data.find(message_type_name);
    if (citer == m_data.end())
    {
        return NULL;
    }

    return &(citer->second);
}

void ConfigReader::Clear()
{
    m_isInitialized = false;
    m_path.clear();
    m_data.clear();
}

void ConfigReader::LazyInit()
{
    if (m_isInitialized)
    {
        return;
    }

    rapidxml::file<> fdoc(m_path.c_str());
    if (fdoc.size() == 0)
    {
        return;
    }

    rapidxml::xml_document<> doc;
    doc.parse<0>(fdoc.data());

    rapidxml::xml_node<> *root_node = doc.first_node();
    if (root_node != NULL)
    {
        // message
        for (rapidxml::xml_node<> *msg_node = root_node->first_node();
            msg_node != NULL;
            msg_node = msg_node->next_sibling())
        {
            const char* msg_type = msg_node->first_attribute("type_name")->value();
            if (m_data.find(msg_type) != m_data.end())
            {
                std::cout << "msg_type repeated, type_name = " << msg_type << std::endl;
                continue;
            }

            // data
            MessageData msg_data;
            for (rapidxml::xml_node<> *data_node = msg_node->first_node();
                data_node != NULL;
                data_node = data_node->next_sibling())
            {
                const char* data_key = data_node->first_attribute("key")->value();
                if (msg_data.Exist(data_key))
                {
                    std::cout << "data_key repeated, type_name = " << data_key << std::endl;
                    continue;
                }

                const char* data_value = data_node->first_attribute("value")->value();

                DataUnit item(data_key, data_value);
                msg_data.Add(item);
            }

            m_data[msg_type] = msg_data;
        }
    }

    m_isInitialized = true;
}
