#include <Utility/Core/StringId.hpp>

StringId::StringId(const std::string& str)
    : m_string(str)
    , m_hash(hash(str.data(), str.size()))
{
}

bool StringId::operator==(const StringId& rhs) const
{
    if (m_hash != rhs.m_hash)
    {
        return false;
    }
    return m_string == rhs.m_string;
}

bool StringId::operator!=(const StringId& rhs) const
{
    if (m_hash != rhs.m_hash)
    {
        return true;
    }
    return m_string != rhs.m_string;
}

const std::string& StringId::Str() const
{
    return m_string;
}

U64 StringId::Id() const
{
    return m_hash;
}
