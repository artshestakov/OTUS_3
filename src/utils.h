#pragma once
//-----------------------------------------------------------------------------
#include "custom_list.h"
#include "custom_allocator.h"
//-----------------------------------------------------------------------------
inline unsigned int Factorial(unsigned int n)
{
    if (n == 0)
    {
        return 1;
    }

    return n * Factorial(n - 1);
}
//-----------------------------------------------------------------------------
template <typename T> struct type_false : std::false_type {};
//-----------------------------------------------------------------------------
template<typename T>
void Fill(T& t)
{
    if constexpr (std::is_same_v<std::map<int, int>, T> ||
                  std::is_same_v< std::map<int, int, std::less<int>, CustomAllocator<std::pair<const int, int>>>, T>)
    {
        for (unsigned int i = 0; i < 10; ++i)
        {
            t[i] = Factorial(i);
        }
    }
    else if constexpr (std::is_same_v<CustomList<int>, T> || std::is_same_v<CustomList<int, CustomAllocator<int>>, T>)
    {
        for (unsigned int i = 0; i < 10; ++i)
        {
            t.push_front(i);
        }
    }
    else if constexpr (std::is_same_v<std::vector<int, CustomAllocator<int>>, T>)
    {
        for (unsigned int i = 0; i < 10; ++i)
        {
            t.emplace_back(i);
        }
    }
    else //Не поддерживаем такой тип
    {
        static_assert(type_false<T>::value, "Not support template type");
    }
}
//-----------------------------------------------------------------------------
