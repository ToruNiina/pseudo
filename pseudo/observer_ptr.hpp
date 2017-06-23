#ifndef PSEUDO_OBSERVER_PTR
#define PSEUDO_OBSERVER_PTR
#include <type_traits>

namespace psd
{

template<typename T>
class observer_ptr
{
  public:

    typedef T element_type;
    typedef typename std::add_pointer<element_type>::type          pointer;
    typedef typename std::add_lvalue_reference<element_type>::type reference;

  public:

    constexpr observer_ptr() noexcept : resource_(nullptr) {}
    ~observer_ptr() noexcept  = default;
    constexpr observer_ptr(const observer_ptr&)  noexcept = default;
    constexpr observer_ptr(observer_ptr&&)       noexcept = default;
    observer_ptr& operator=(const observer_ptr&) noexcept = default;
    observer_ptr& operator=(observer_ptr&&)      noexcept = default;

    constexpr
    observer_ptr(std::nullptr_t) noexcept : resource_(nullptr) {}
    constexpr explicit
    observer_ptr(pointer ptr) noexcept : resource_(ptr){}

    template<typename U, typename std::enable_if<
        std::is_convertible<typename std::add_pointer<U>::type, pointer>::value,
        std::nullptr_t>::type = nullptr>
    constexpr observer_ptr(observer_ptr<U> other) noexcept
        : resource_(other.get())
    {}

    constexpr pointer get() const noexcept {return resource_;}
    constexpr pointer  operator->() const noexcept {return resource_;}
    constexpr reference operator*() const noexcept {return *resource_;}

    constexpr explicit operator bool()    const noexcept
    {return static_cast<bool>(resource_);}
    constexpr explicit operator pointer() const noexcept
    {return resource_;}

    pointer release() noexcept
    {const auto tmp = resource_; resource_ = nullptr; return tmp;}

    void reset(pointer ptr = nullptr) noexcept {resource_ = ptr;}
    void swap(observer_ptr& other) noexcept
    {const auto tmp = other.get(); other.reset(resource_); resource_ = tmp;}

  private:

    pointer resource_;
};

template<typename T>
inline void swap(observer_ptr<T>& lhs, observer_ptr<T>& rhs)
{
    lhs.swap(rhs);
    return;
}

template<typename T>
constexpr inline observer_ptr<T> make_observer(T* ptr) noexcept
{
    return observer_ptr<T>(ptr);
}

template<typename T>
constexpr inline bool
operator==(observer_ptr<T> const& lhs, observer_ptr<T> const& rhs)
{
    return lhs.get() == rhs.get();
}

template<typename T>
constexpr inline bool
operator!=(observer_ptr<T> const& lhs, observer_ptr<T> const& rhs)
{
    return lhs.get() != rhs.get();
}

template<typename T>
constexpr inline bool
operator<(observer_ptr<T> const& lhs, observer_ptr<T> const& rhs)
{
    return lhs.get() < rhs.get();
}

template<typename T>
constexpr inline bool
operator<=(observer_ptr<T> const& lhs, observer_ptr<T> const& rhs)
{
    return lhs.get() <= rhs.get();
}

template<typename T>
constexpr inline bool
operator>(observer_ptr<T> const& lhs, observer_ptr<T> const& rhs)
{
    return lhs.get() > rhs.get();
}

template<typename T>
constexpr inline bool
operator>=(observer_ptr<T> const& lhs, observer_ptr<T> const& rhs)
{
    return lhs.get() >= rhs.get();
}

template<typename T> using raw_ptr       = observer_ptr<T>;
template<typename T> using pure_ptr      = observer_ptr<T>;
template<typename T> using naked_ptr     = observer_ptr<T>;
template<typename T> using ownerless_ptr = observer_ptr<T>;
template<typename T> using unowned_ptr   = observer_ptr<T>;

} // psd

namespace std
{
template<typename T>
struct hash<psd::observer_ptr<T>>
{
    typedef psd::observer_ptr<T> argument_type;
    typedef std::size_t          result_type;

    result_type operator()(argument_type const& p) const
    {
        return std::hash<T*>{}(p.get());
    }
};
}// std

#endif// PSEUDO_OBSERVER_PTR
