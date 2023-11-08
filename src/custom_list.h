#pragma once
//-----------------------------------------------------------------------------
#include <memory>
//-----------------------------------------------------------------------------
template<typename T, typename Alloc = std::allocator<T>>
class CustomList
{
    struct ListNode
    {
        ListNode* next;
        T value;
    };

public:
    using allocator_type = Alloc;

    CustomList()
        : m_Head(nullptr),
        m_Tail(nullptr)
    {

    }

    CustomList(const allocator_type& a)
        : m_Head(nullptr),
        m_Tail(nullptr),
        m_Allocator(a)
    {

    }

    void push_front(const T& value)
    {
        typename Alloc::template rebind<ListNode>::other alloc;
        ListNode* n = alloc.allocate(1);
        n->value = value;
        n->next = nullptr;

        if (m_Head == nullptr)
        {
            m_Head = n;
            m_Tail = n;
        }
        else
        {
            m_Tail->next = n;
            m_Tail = m_Tail->next;
        }
    }

    T& front() const
    {
        return m_Head->value;
    }

    T& back()
    {
        return m_Tail->value;
    }

    class iterator
    {
    public:
        iterator() noexcept
            : m_Current(nullptr)
        {

        }

        iterator(const ListNode* ptr) noexcept
            : m_Current(ptr)
        {

        }

        iterator& operator=(ListNode* ptr)
        {
            m_Current = ptr;
            return *this;
        }

        iterator& operator++()
        {
            if (m_Current)
                m_Current = m_Current->next;
            return *this;
        }

        bool operator==(const iterator& it)
        {
            return m_Current == it.m_Current;
        }

        bool operator!=(const iterator& it)
        {
            return m_Current != it.m_Current;
        }

        T operator*() const
        {
            return m_Current->value;
        }

    private:
        const ListNode* m_Current;
    };

    iterator begin()
    {
        return iterator(m_Head);
    }

    iterator begin() const
    {
        return iterator(m_Head);
    }

    iterator end()
    {
        return iterator(nullptr);
    }

    iterator end() const
    {
        return iterator(nullptr);
    }

private:
    ListNode* m_Head;
    ListNode* m_Tail;
    Alloc m_Allocator;
};
//-----------------------------------------------------------------------------
