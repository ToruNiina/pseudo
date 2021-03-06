#ifndef PSEUDO_TRIPLET
#define PSEUDO_TRIPLET
#include <pseudo/type_traits.hpp>
#include <pseudo/utility.hpp>
#include <pseudo/tuple.hpp>

namespace psd
{

template<typename T1, typename T2, typename T3>
struct triplet
{
    typedef T1 first_type;
    typedef T2 second_type;
    typedef T3 third_type;

    first_type  first;
    second_type second;
    third_type  third;

    constexpr triplet() noexcept(
        std::is_nothrow_default_constructible<first_type >::value &&
        std::is_nothrow_default_constructible<second_type>::value &&
        std::is_nothrow_default_constructible<third_type >::value)
        : first{}, second{}, third{}{}
    ~triplet() noexcept(
        std::is_nothrow_destructible<first_type >::value &&
        std::is_nothrow_destructible<second_type>::value &&
        std::is_nothrow_destructible<third_type >::value) = default;
    constexpr triplet(triplet&&) noexcept(
        std::is_nothrow_move_constructible<first_type >::value &&
        std::is_nothrow_move_constructible<second_type>::value &&
        std::is_nothrow_move_constructible<third_type >::value) = default;
    constexpr triplet(triplet const&) noexcept(
        std::is_nothrow_copy_constructible<first_type >::value &&
        std::is_nothrow_copy_constructible<second_type>::value &&
        std::is_nothrow_copy_constructible<third_type >::value) = default;
    triplet& operator=(triplet&&) noexcept(
        std::is_nothrow_move_assignable<first_type >::value &&
        std::is_nothrow_move_assignable<second_type>::value &&
        std::is_nothrow_move_assignable<third_type >::value) = default;
    triplet& operator=(const triplet&) noexcept(
        std::is_nothrow_copy_assignable<first_type >::value &&
        std::is_nothrow_copy_assignable<second_type>::value &&
        std::is_nothrow_copy_assignable<third_type >::value) = default;

    template<typename U1, typename U2, typename U3, typename std::enable_if<
        std::is_convertible<U1, T1>::value&&std::is_convertible<U2, T2>::value&&
        std::is_convertible<U3, T3>::value, std::nullptr_t>::type = nullptr>
    constexpr triplet(U1&& a, U2&& b, U3&& c) noexcept(
        std::is_nothrow_constructible<first_type,  U1&&>::value &&
        std::is_nothrow_constructible<second_type, U2&&>::value &&
        std::is_nothrow_constructible<third_type,  U3&&>::value)
        : first(std::forward<U1>(a)), second(std::forward<U2>(b)),
          third(std::forward<U3>(c))
    {}

    template<typename U1, typename U2, typename U3, typename std::enable_if<
        std::is_convertible<U1, T1>::value&&std::is_convertible<U2, T2>::value&&
        std::is_convertible<U3, T3>::value, std::nullptr_t>::type = nullptr>
    constexpr triplet(const std::tuple<U1, U2, U3>& t) noexcept(
        std::is_nothrow_constructible<first_type,  U1 const&>::value &&
        std::is_nothrow_constructible<second_type, U2 const&>::value &&
        std::is_nothrow_constructible<third_type,  U3 const&>::value)
        : first(std::get<0>(t)), second(std::get<1>(t)), third(std::get<2>(t))
    {}

    template<typename U1, typename U2, typename U3, typename std::enable_if<
        std::is_convertible<U1, T1>::value&&std::is_convertible<U2, T2>::value&&
        std::is_convertible<U3, T3>::value, std::nullptr_t>::type = nullptr>
    constexpr triplet(std::tuple<U1, U2, U3>&& t) noexcept(
        std::is_nothrow_constructible<first_type,  U1&&>::value &&
        std::is_nothrow_constructible<second_type, U2&&>::value &&
        std::is_nothrow_constructible<third_type,  U3&&>::value)
        : first(std::move(std::get<0>(t)))
        , second(std::move(std::get<1>(t)))
        , third(std::move(std::get<2>(t)))
    {}

    template<typename ... T1s, typename ... T2s, typename ... T3s>
    constexpr triplet(std::piecewise_construct_t, std::tuple<T1s...> arg1,
                      std::tuple<T1s...> arg2,    std::tuple<T1s...> arg3)
        noexcept(std::is_nothrow_constructible<first_type,  T1s...>::value&&
                 std::is_nothrow_constructible<second_type, T2s...>::value&&
                 std::is_nothrow_constructible<third_type,  T3s...>::value)
        : triplet(std::move(arg1), std::move(arg2), std::move(arg3),
                  detail::make_index_tuple(arg1),
                  detail::make_index_tuple(arg2),
                  detail::make_index_tuple(arg3))
    {}

    void swap(triplet& t) noexcept(noexcept(std::swap(first,  t.first)) &&
                                   noexcept(std::swap(second, t.second))&&
                                   noexcept(std::swap(third,  t.third)))
    {
        using std::swap;
        swap(this->first,  t.first);
        swap(this->second, t.second);
        swap(this->third,  t.third);
        return;
    }

  private:

    template<typename    ... T1s, typename    ... T2s, typename    ... T3s,
             std::size_t ... I1s, std::size_t ... I2s, std::size_t ... I3s>
    constexpr triplet(std::tuple<T1s...>&& t1,
                      std::tuple<T2s...>&& t2,
                      std::tuple<T3s...>&& t3,
                      detail::index_tuple<std::size_t, I1s...> i1,
                      detail::index_tuple<std::size_t, I2s...> i2,
                      detail::index_tuple<std::size_t, I3s...> i3)
        noexcept(std::is_nothrow_constructible<first_type,  T1s...>::value&&
                 std::is_nothrow_constructible<second_type, T2s...>::value&&
                 std::is_nothrow_constructible<third_type,  T3s...>::value)
        :  first(std::forward<T1s>(std::get<I1s>(t1))...)
        , second(std::forward<T2s>(std::get<I2s>(t2))...)
        ,  third(std::forward<T3s>(std::get<I3s>(t3))...)
    {}
};

template<typename T1, typename T2, typename T3>
inline void swap(triplet<T1, T2, T3>& lhs, triplet<T1, T2, T3>& rhs)
{
    lhs.swap(rhs);
    return;
}

template<typename T1, typename T2, typename T3>
constexpr inline
triplet<typename detail::decay_and_strip<T1>::type,
        typename detail::decay_and_strip<T2>::type,
        typename detail::decay_and_strip<T3>::type>
make_triplet(T1&& arg1, T2&& arg2, T3&& arg3)
{
    return triplet<typename detail::decay_and_strip<T1>::type,
                   typename detail::decay_and_strip<T2>::type,
                   typename detail::decay_and_strip<T3>::type>(
        std::forward<T1>(arg1), std::forward<T2>(arg2), std::forward<T3>(arg3));
}

template<typename T1, typename T2, typename T3>
constexpr inline std::tuple<T1, T2, T3>
make_tuple(const triplet<T1, T2, T3>& lhs) noexcept(
        std::is_nothrow_copy_constructible<T1>::value&&
        std::is_nothrow_copy_constructible<T2>::value&&
        std::is_nothrow_copy_constructible<T3>::value&&
        std::is_nothrow_constructible<std::tuple<T1, T2, T3>,
            T1 const&, T2 const&, T3 const&>::value)
{
    return std::tuple<T1, T2, T3>(lhs.first, lhs.second, lhs.third);
}

template<typename T1, typename T2, typename T3>
constexpr inline std::tuple<T1, T2, T3>
make_tuple(triplet<T1, T2, T3>&& lhs) noexcept(
        std::is_nothrow_move_constructible<T1>::value&&
        std::is_nothrow_move_constructible<T2>::value&&
        std::is_nothrow_move_constructible<T3>::value&&
        std::is_nothrow_constructible<std::tuple<T1, T2, T3>,
            T1&&, T2&&, T3&&>::value)
{
    return std::tuple<T1, T2, T3>(
            std::move(lhs.first), std::move(lhs.second), std::move(lhs.third));
}


template<typename T1, typename T2, typename T3>
constexpr inline bool
operator==(const triplet<T1, T2, T3>& lhs, const triplet<T1, T2, T3>& rhs)
{
    return (lhs.first == rhs.first) && (lhs.second == rhs.second) &&
           (lhs.third == rhs.third);
}

template<typename T1, typename T2, typename T3>
constexpr inline bool
operator<(const triplet<T1, T2, T3>& lhs, const triplet<T1, T2, T3>& rhs)
{
    return (lhs.first < rhs.first) ||
           (!(lhs.first > rhs.first) && (lhs.second < rhs.second)) ||
           (!(lhs.first > rhs.first) && !(lhs.second > rhs.second) &&
             (lhs.third < rhs.third));
}

template<typename T1, typename T2, typename T3>
constexpr inline bool
operator!=(const triplet<T1, T2, T3>& lhs, const triplet<T1, T2, T3>& rhs)
{
    return !(lhs == rhs);
}

template<typename T1, typename T2, typename T3>
constexpr inline bool
operator>(const triplet<T1, T2, T3>& lhs, const triplet<T1, T2, T3>& rhs)
{
    return rhs < lhs;
}

template<typename T1, typename T2, typename T3>
constexpr inline bool
operator<=(const triplet<T1, T2, T3>& lhs, const triplet<T1, T2, T3>& rhs)
{
    return !(rhs < lhs);
}

template<typename T1, typename T2, typename T3>
constexpr inline bool
operator>=(const triplet<T1, T2, T3>& lhs, const triplet<T1, T2, T3>& rhs)
{
    return !(lhs < rhs);
}

template<typename T1, typename T2, typename T3>
struct tuple_element<0, triplet<T1, T2, T3>> {typedef T1 type;};
template<typename T1, typename T2, typename T3>
struct tuple_element<1, triplet<T1, T2, T3>> {typedef T2 type;};
template<typename T1, typename T2, typename T3>
struct tuple_element<2, triplet<T1, T2, T3>> {typedef T3 type;};

template<typename T1, typename T2, typename T3>
struct tuple_size<triplet<T1, T2, T3>> : std::integral_constant<std::size_t, 3>
{};

namespace detail
{
template<std::size_t I, typename Triple>
struct triplet_get_impl;
template<typename T1, typename T2, typename T3>
struct triplet_get_impl<0, triplet<T1, T2, T3>>
{
    constexpr static T1& invoke(triplet<T1, T2, T3>& tri) noexcept
    {return tri.first;}
    constexpr static T1&& invoke(triplet<T1, T2, T3>&& tri) noexcept
    {return std::move(tri.first);}
    constexpr static T1 const& invoke(const triplet<T1, T2, T3>& tri) noexcept
    {return tri.first;}
};
template<typename T1, typename T2, typename T3>
struct triplet_get_impl<1, triplet<T1, T2, T3>>
{
    constexpr static T2& invoke(triplet<T1, T2, T3>& tri) noexcept
    {return tri.second;}
    constexpr static T2&& invoke(triplet<T1, T2, T3>&& tri) noexcept
    {return std::move(tri.second);}
    constexpr static T2 const& invoke(const triplet<T1, T2, T3>& tri) noexcept
    {return tri.second;}
};
template<typename T1, typename T2, typename T3>
struct triplet_get_impl<2, triplet<T1, T2, T3>>
{
    constexpr static T3& invoke(triplet<T1, T2, T3>& tri) noexcept
    {return tri.third;}
    constexpr static T3&& invoke(triplet<T1, T2, T3>&& tri) noexcept
    {return std::move(tri.third);}
    constexpr static T3 const& invoke(const triplet<T1, T2, T3>& tri) noexcept
    {return tri.third;}
};
}// detail

template<std::size_t I, typename T1, typename T2, typename T3>
constexpr typename tuple_element<I, triplet<T1, T2, T3>>::type&
get(triplet<T1, T2, T3>& tri) noexcept
{
    return detail::triplet_get_impl<I, triplet<T1, T2, T3>>::invoke(tri);
}

template<std::size_t I, typename T1, typename T2, typename T3>
constexpr typename tuple_element<I, triplet<T1, T2, T3>>::type&&
get(triplet<T1, T2, T3>&& tri) noexcept
{
    return detail::triplet_get_impl<I, triplet<T1, T2, T3>>::invoke(std::move(tri));
}

template<std::size_t I, typename T1, typename T2, typename T3>
constexpr typename tuple_element<I, triplet<T1, T2, T3>>::type const&
get(triplet<T1, T2, T3> const& tri) noexcept
{
    return detail::triplet_get_impl<I, triplet<T1, T2, T3>>::invoke(tri);
}

} // psd
#endif /* PSEUDO_TRIPLET */
