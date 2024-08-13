#pragma once

#include <string_view>

namespace comics
{

class Coroutine
{
public:
    Coroutine(std::string_view issues, std::string_view sequences);
};

}
