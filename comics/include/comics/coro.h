#pragma once

#include <simdjson.h>

#include <map>
#include <memory>
#include <string_view>

namespace comics
{

class Database
{
public:
    virtual ~Database() = default;
    virtual const simdjson::simdjson_result<simdjson::dom::element> &getIssues() const = 0;
    virtual const simdjson::simdjson_result<simdjson::dom::element> &getSequences() const = 0;
};

using DatabasePtr = std::shared_ptr<Database>;

DatabasePtr createDatabase(std::string_view issues, std::string_view sequences);

class Coroutine
{
public:
    Coroutine(DatabasePtr database, std::string_view field, std::string_view name);

    bool resume() const;

private:
    DatabasePtr m_database;
    std::string m_field;
    std::string m_name;
};

} // namespace comics
