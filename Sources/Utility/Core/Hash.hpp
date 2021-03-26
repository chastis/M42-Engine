#pragma once

#include <Core/Types.hpp>

namespace details
{
    namespace fnv
    {
        constexpr static U64 FNV_OFFSET_BASIS   = 0xcbf29ce484222325;
        constexpr static U64 FNV_PRIME          = 0x00000100000001B3;

        template <class Iter>
        constexpr U64 fnv1_hash(Iter begin, Iter end)
        {
            U64 hash = FNV_OFFSET_BASIS;
            for (auto head = begin; head != end; ++head)
            {
                hash *= FNV_PRIME;
                hash ^= *head;
            }
            return hash;
        }

        template <class Iter>
        constexpr U64 fnv1_hash(Iter begin, size_t n) noexcept
        {
            U64 hash = FNV_OFFSET_BASIS;
            for (size_t i = 0; i < n; ++i, ++begin)
            {
                hash *= FNV_PRIME;
                hash ^= *begin;
            }
            return hash;
        }

        template <class Iter>
        constexpr U64 fnv1a_hash(Iter begin, Iter end)
        {
            U64 hash = FNV_OFFSET_BASIS;
            for (auto head = begin; head != end; ++head)
            {
                hash ^= *head;
                hash *= FNV_PRIME;
            }
            return hash;
        }

        template <class Iter>
        constexpr U64 fnv1a_hash(Iter begin, size_t n) noexcept
        {
            U64 hash = FNV_OFFSET_BASIS;
            for (size_t i = 0; i < n; ++i, ++begin)
            {
                hash ^= *begin;
                hash *= FNV_PRIME;
            }
            return hash;
        }
    }

    template <class Iter>
    constexpr U64 DefaultHasher(Iter begin, Iter end)
    {
        return fnv::fnv1a_hash(begin, end);
    }

    template <class Iter>
    constexpr U64 DefaultHasher(Iter begin, size_t n)
    {
        return fnv::fnv1a_hash(begin, n);
    }
}

template <class Iter>
static constexpr U64 hash(Iter begin, Iter end) noexcept
{
    return details::DefaultHasher(begin, end);
}

template <class Iter>
static constexpr U64 hash(Iter bytes, size_t n) noexcept
{
    return details::DefaultHasher(bytes, n);
}

static constexpr U64 operator""_sid(const char* const str, const size_t n) noexcept
{
    return hash(str, n);
}
