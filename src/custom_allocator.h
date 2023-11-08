#include <memory>
#include <iostream>
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
        : begin(nullptr),
        end(nullptr),
        stack_pointer(nullptr)
    {

    }

    explicit CustomAllocator(pointer buffer)
        : begin(buffer),
        end(buffer + N),
        stack_pointer(buffer)
    {

    }

    CustomAllocator(const CustomAllocator& other)
        : allocator(other.allocator),
        begin(other.begin),
        end(other.end),
        stack_pointer(other.stack_pointer)
    {

    }

    template<typename U>
    CustomAllocator(const CustomAllocator<U, N>& other)
        : allocator(other.allocator),
        begin(other.begin),
        end(other.end),
        stack_pointer(other.stack_pointer)
    {

    }

    pointer allocate(size_type n, const_void_pointer hint = const_void_pointer())
    {
        if (begin == nullptr)
        {
            init();
        }

        if (n <= size_type(std::distance(stack_pointer, end)))
        {
            pointer result = stack_pointer;
            stack_pointer += n;
            return result;
        }

        return allocator.allocate(n, hint);
    }

    void deallocate(pointer p, size_type n)
    {
        if (owns(p))
        {
            stack_pointer -= n;
        }
    }

    template<typename U, typename... Args>
    void construct(U* p, Args &&... args)
    {
        allocator.construct(p, std::forward<Args>(args)...);
    }

    template<typename U>
    void destroy(U* p)
    {
        allocator.destroy(p);
    }

    bool owns(const_pointer p)
    {
        return (!(std::less<const_pointer>()(p, begin)) && (std::less<const_pointer>()(p, end)));
    }

    pointer buffer() const noexcept
    {
        return begin;
    }

    template <class U>
    struct rebind
    {
        using other = CustomAllocator<U, N>;
    };

private:
    pointer begin;
    pointer end;
    pointer stack_pointer;
    Alloc allocator;

    void init()
    {
        begin = reinterpret_cast<T*>(operator new (N * sizeof(T)));
        end = begin + N;
        stack_pointer = begin;
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
