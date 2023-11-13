#include <memory>
#include <stdexcept>
//-----------------------------------------------------------------------------
template<typename T, size_t N = 10, typename Alloc = std::allocator<T>>
class CustomAllocator
{
public:
    using value_type = typename std::allocator_traits<Alloc>::value_type;
    using pointer = typename std::allocator_traits<Alloc>::pointer;
    using const_pointer = typename std::allocator_traits<Alloc>::const_pointer;
    using const_void_pointer = typename std::allocator_traits<Alloc>::const_void_pointer;
    using size_type = size_t;

    explicit CustomAllocator()
        : m_Begin(nullptr),
        m_End(nullptr),
        m_StackPointer(nullptr)
    {

    }

    explicit CustomAllocator(pointer buffer)
        : m_Begin(buffer),
        m_End(buffer + N),
        m_StackPointer(buffer)
    {

    }

    CustomAllocator(const CustomAllocator& other)
        : m_Allocator(other.m_Allocator),
        m_Begin(other.m_Begin),
        m_End(other.m_End),
        m_StackPointer(other.m_StackPointer)
    {

    }

    template<typename U>
    CustomAllocator(const CustomAllocator<U, N>& other)
        : m_Allocator(other.m_Allocator),
        m_Begin(other.m_Begin),
        m_End(other.m_End),
        m_StackPointer(other.m_StackPointer)
    {

    }

    pointer allocate(size_type n, const_void_pointer hint = const_void_pointer())
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
            return m_Allocator.allocate(n, hint);
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

    template<typename U, typename... Args>
    void construct(U* p, Args &&... args)
    {
        m_Allocator.construct(p, std::forward<Args>(args)...);
    }

    template<typename U>
    void destroy(U* p)
    {
        m_Allocator.destroy(p);
    }

    bool owns(const_pointer p)
    {
        return (!(std::less<const_pointer>()(p, m_Begin)) && (std::less<const_pointer>()(p, m_End)));
    }

    pointer buffer() const noexcept
    {
        return m_Begin;
    }

    template <class U>
    struct rebind
    {
        using other = CustomAllocator<U, N>;
    };

private:
    pointer m_Begin;
    pointer m_End;
    pointer m_StackPointer;
    Alloc m_Allocator;

    void init()
    {
        m_Begin = reinterpret_cast<T*>(operator new (N * sizeof(T)));
        m_End = m_Begin + N;
        m_StackPointer = m_Begin;
    }
};
//-----------------------------------------------------------------------------
template <typename T1, std::size_t N, typename T2>
bool operator == (const CustomAllocator<T1, N>& lhs, const CustomAllocator<T2, N>& rhs) noexcept
{
    return lhs.buffer() == rhs.buffer();
}
//-----------------------------------------------------------------------------
template <typename T1, std::size_t N, typename T2>
bool operator != (const CustomAllocator<T1, N>& lhs, const CustomAllocator<T2, N>& rhs) noexcept
{
    return !(lhs == rhs);
}
//-----------------------------------------------------------------------------
