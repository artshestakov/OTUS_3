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
        : head(nullptr),
        tail(nullptr)
    {

    }

    CustomList(const allocator_type& a)
        : head(nullptr),
        tail(nullptr),
        allocator(a)
    {

    }

    void push_front(const T& value)
    {
        typename Alloc::template rebind<ListNode>::other alloc;
        ListNode* n = alloc.allocate(1);
        n->value = value;
        n->next = nullptr;

        if (head == nullptr)
        {
            head = n;
            tail = n;
        }
        else
        {
            tail->next = n;
            tail = tail->next;
        }
    }

    T& front() const
    {
        return head->value;
    }

    T& back()
    {
        return tail->value;
    }

    class iterator
    {
    public:
        iterator() noexcept
            : current(nullptr)
        {

        }

        iterator(const ListNode* ptr) noexcept
            : current(ptr)
        {

        }

        iterator& operator=(ListNode* ptr)
        {
            current = ptr;
            return *this;
        }

        iterator& operator++()
        {
            if (current)
                current = current->next;
            return *this;
        }

        bool operator==(const iterator& it)
        {
            return current == it.current;
        }

        bool operator!=(const iterator& it)
        {
            return current != it.current;
        }

        T operator*() const
        {
            return current->value;
        }

    private:
        const ListNode* current;
    };

    iterator begin()
    {
        return iterator(head);
    }

    iterator begin() const
    {
        return iterator(head);
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
    ListNode* head;
    ListNode* tail;
    Alloc allocator;
};
//-----------------------------------------------------------------------------
