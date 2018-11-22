#ifndef _CONFIG_READER_H_
#define _CONFIG_READER_H_

#include <iostream>
#include <string>
#include <vector>
#include <map>

//////////////////////////////////////////////////////////////////////////

class Options
{
    typedef std::map<std::string, std::string> OptionData;
    typedef OptionData::iterator OptionDataIter;
    typedef OptionData::const_iterator OptionDataCiter;

public:
    static const std::string& outputDir() { return Query("output_dir"); }
    static const std::string& cppDir() { return Query("cpp_dir"); }
    static const std::string& luaDir() { return Query("lua_dir"); }
    static const std::string& nameSpliter() { return Query("name_spliter"); }
    static const std::string& extensionName() { return Query("extension_name"); }
    static const std::string& Query(const char* key);

private:
    Options() {}
    static void LoadOption();

    static bool s_initialized;
    static OptionData m_data;
};

//////////////////////////////////////////////////////////////////////////

class DataUnit
{
public:
    DataUnit(const char* key, const char* value);
    ~DataUnit();

    const char* Key() const;
    const char* Value() const;

private:
    std::string m_key;
    std::string m_value;
};

//////////////////////////////////////////////////////////////////////////

class MessageData
{
    typedef std::vector<DataUnit> DataList;
    typedef DataList::iterator DataListIter;
    typedef DataList::const_iterator DataListCiter;

public:
    MessageData();
    ~MessageData();

    void Add(const DataUnit& data);
    const DataUnit* Get(size_t index) const;
    size_t Count() const;
    void Clear();
    bool Exist(const char* key) const;

private:
    DataList m_dataList;
};

//////////////////////////////////////////////////////////////////////////

class ConfigReader
{
    typedef std::map<std::string, MessageData> MessageDataGroup;
    typedef MessageDataGroup::iterator MessageDataGroupIter;
    typedef MessageDataGroup::const_iterator MessageDataGroupCiter;

public:
    ConfigReader(const char* path = "");
    ~ConfigReader();

    const char* FirstMesage();
    const char* NextMessage();
    const MessageData* GetMessageData(const char* message_type_name) const;

    void Clear();

protected:
    void LazyInit();

private:
    bool m_isInitialized;
    std::string m_path;
    MessageDataGroup m_data;
    MessageDataGroupIter m_traversalIter;
};

#endif // _CONFIG_READER_H_
