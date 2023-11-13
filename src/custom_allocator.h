#include <memory>
#include <stdexcept>
//-----------------------------------------------------------------------------
template<typename T>
struct pointer_traits
{
    using reference = T&;
    using const_reference = const T&;
};
//-----------------------------------------------------------------------------
template<>
struct pointer_traits<void> { };
//-----------------------------------------------------------------------------
template<typename T = void, size_t N = 10>
struct CustomAllocator : public pointer_traits<T>
{
public:
    using value_type = T;
    using size_type = std::size_t;
    using pointer = value_type*;
    using const_pointer = const value_type*;
    using difference_type = typename std::pointer_traits<pointer>::difference_type;

    CustomAllocator() noexcept
        : m_Begin(nullptr),
        m_End(nullptr),
        m_StackPointer(nullptr)
    {

    }

    ~CustomAllocator() noexcept
    {

    }

    template<typename U>
    CustomAllocator(const CustomAllocator<U>&) noexcept
        : m_Begin(nullptr),
        m_End(nullptr),
        m_StackPointer(nullptr)
    {

    }

    pointer allocate(size_type n, const void* = 0)
    {
        if (m_Begin == nullptr)
        {
            init();
        }

        if (n <= size_type(std::distance(m_StackPointer, m_End)))
        {
            pointer result = m_StackPointer;
            m_StackPointer += n;
            return result;
        }

        try
        {
            return allocate(n);
        }
        catch (const std::bad_alloc& e)
        {
            throw std::runtime_error(e.what());
        }
        return pointer();
    }

    void deallocate(pointer p, size_type n)
    {
        if (owns(p))
        {
            m_StackPointer -= n;
        }
    }

    bool owns(const_pointer p)
    {
        return (!(std::less<const_pointer>()(p, m_Begin)) && (std::less<const_pointer>()(p, m_End)));
    }

    pointer buffer() const noexcept
    {
        return m_Begin;
    }

    template<typename U>
    struct rebind
    {
        using other = CustomAllocator<U>;
    };

private:
    pointer m_Begin;
    pointer m_End;

    pointer m_StackPointer;
    void init()
    {
        m_Begin = reinterpret_cast<pointer>(operator new (N * sizeof(value_type)));
        m_End = m_Begin + N;
        m_StackPointer = m_Begin;
    }
};
//-----------------------------------------------------------------------------
template <typename T1, std::size_t N, typename T2>
bool operator==(const CustomAllocator<T1, N>& lhs, const CustomAllocator<T2, N>& rhs) noexcept
{
    return lhs.buffer() == rhs.buffer();
}
//-----------------------------------------------------------------------------
template <typename T1, std::size_t N, typename T2>
bool operator!=(const CustomAllocator<T1, N>& lhs, const CustomAllocator<T2, N>& rhs) noexcept
{
    return !(lhs == rhs);
}
//-----------------------------------------------------------------------------
