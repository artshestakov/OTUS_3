#define _SILENCE_CXX17_OLD_ALLOCATOR_MEMBERS_DEPRECATION_WARNING //Для использования if constexpr
//-----------------------------------------------------------------------------
#include <map>
#include <vector>
#include "utils.h"
//-----------------------------------------------------------------------------
int main()
{
    // 1. Создание экземпляра std::map<int, int>
    {
        std::map<int, int> m;
        Fill(m);
    }

    // 2. std::map с кастомным аллокатором
    {
        std::map<int, int, std::less<int>, CustomAllocator<std::pair<const int, int>>> m;
        Fill(m);
    }

    // 3. Кастомный контейнер
    {
        CustomList<int> l;
        Fill(l);
    }

    // 4. Кастомный контейнер с кастомным аллокатором
    {
        CustomList<int, CustomAllocator<int>> l;
        Fill(l);
    }

    // 5. Стандартный контейнер с кастомным аллокатором
    {
        std::vector<int, CustomAllocator<int>> v;
        Fill(v);
    }

    return 0;
}
//-----------------------------------------------------------------------------
