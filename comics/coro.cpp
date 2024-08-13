#include <comics/coro.h>

#include <stdexcept>

namespace comics
{

Coroutine::Coroutine(DatabasePtr database, std::string_view field, std::string_view name) :
    m_database(database),
    m_field(field),
    m_name(name)
{
}

bool Coroutine::resume() const
{
    if (!m_database)
    {
        return false;
    }

    const auto &dbIssues = m_database->getIssues();
    const auto &dbSeqs = m_database->getSequences();
    if (!dbIssues.is_array() || !dbSeqs.is_array())
    {
        return false;
    }
    const simdjson::dom::array issues{dbIssues.get_array()};
    if (issues.size() == 0)
    {
        return false;
    }
    const simdjson::dom::array seqs{dbSeqs.get_array()};
    return seqs.size() != 0;
}

} // namespace comics
