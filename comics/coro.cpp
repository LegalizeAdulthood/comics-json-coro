#include <comics/coro.h>

#include <coroutine>
#include <map>
#include <stdexcept>

namespace comics
{

simdjson::dom::object find_issue(DatabasePtr db, int issue)
{
    simdjson::dom::array issues = db->getIssues().get_array();
    const auto it = std::find_if(issues.begin(), issues.end(),
        [=](const simdjson::dom::element &item)
        {
            if (!item.is_object())
            {
                throw std::runtime_error("Issue array element is not an object");
            }
            const simdjson::dom::object &obj = item.get_object().value();
            if (!obj.at_key("id").is_string())
            {
                std::ostringstream typeName;
                typeName << obj.at_key("id").type();
                throw std::runtime_error("Expected string value for key 'id', got " + typeName.str());
            }
            const int issueId = std::stoi(std::string{obj.at_key("id").get_string().value()});
            return issueId == issue;
        });
    if (it == issues.end())
    {
        throw std::runtime_error("Couldn't find issue with id " + std::to_string(issue));
    }
    return (*it).get_object().value();
}

static std::string_view to_string(CreditField field)
{
    switch (field)
    {
    case CreditField::NONE:
        return "none";
    case CreditField::SCRIPT:
        return "script";
    case CreditField::PENCIL:
        return "pencils";
    case CreditField::INK:
        return "inks";
    case CreditField::COLOR:
        return "colors";
    case CreditField::LETTER:
        return "letters";
    }
    return "?";
}

MatchGenerator matches(DatabasePtr database, CreditField creditField, std::string_view name)
{
    if (!database)
    {
        co_return;
    }

    std::map<int, simdjson::dom::object> issues;
    std::string_view fieldName{to_string(creditField)};

    for (const simdjson::dom::element record : database->getSequences().get_array())
    {
        if (!record.is_object())
        {
            throw std::runtime_error("Sequence array element should be an object");
        }

        const simdjson::dom::object sequence{record.get_object()};
        for (const simdjson::dom::key_value_pair field : sequence)
        {
            if (field.key == fieldName)
            {
                const simdjson::dom::element &value = field.value;
                if (!value.is_string())
                {
                    throw std::runtime_error("Value of script field should be a string");
                }
                if (value.get_string().value().find(name) != std::string::npos)
                {
                    simdjson::dom::object obj = record.get_object().value();
                    const int issue = std::stoi(std::string{obj.at_key("issue").get_string().value()});
                    if (auto it = issues.find(issue); it == issues.end())
                    {
                        issues[issue] = find_issue(database, issue);
                    }
                    co_yield SequenceMatch{issues[issue], sequence};
                }
                break;
            }
        }
    }

}
} // namespace comics
