#define _SILENCE_CXX17_OLD_ALLOCATOR_MEMBERS_DEPRECATION_WARNING //��� ������������� if constexpr
//-----------------------------------------------------------------------------
#include <map>
#include <vector>
#include "utils.h"
//-----------------------------------------------------------------------------
int main()
{
    // 1. �������� ���������� std::map<int, int>
    {
        std::map<int, int> m;
        Fill(m);
    }

    // 2. std::map � ��������� �����������
    {
        std::map<int, int, std::less<int>, CustomAllocator<std::pair<const int, int>>> m;
        Fill(m);
    }

    // 3. ��������� ���������
    {
        CustomList<int> l;
        Fill(l);
    }

    // 4. ��������� ��������� � ��������� �����������
    {
        CustomList<int, CustomAllocator<int>> l;
        Fill(l);
    }

    // 5. ����������� ��������� � ��������� �����������
    {
        std::vector<int, CustomAllocator<int>> v;
        Fill(v);
    }

    return 0;
}
//-----------------------------------------------------------------------------
