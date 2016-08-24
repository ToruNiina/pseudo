/*! @file triplet.hpp
 *  @brief struct template that stores three heterogenous objects.            *
 *  template struct like pair. you can use tuple instead.                     *
 *  this source code is distributed under the MIT license.                    *
 *  @author Toru Niina (niina.toru.68u@gmail.com)                             *
 *  @date 2016-08-23 21:00                                                    *
 *  @copyright (c) 2016 Toru Niina, All rights reserved.                      */
#ifndef PSEUDO_TRIPLET
#define PSEUDO_TRIPLET 1
#include "definitions.hpp"
#include <utility>
#if __cplusplus >= 201103L
#include <type_traits>
#include <functional> //for reference_wrapper<T>
#include "type_traits.hpp" // for decay_and_strip<T>
#endif /* c++11 or later */

namespace psd
{

template<typename T1, typename T2, typename T3>
struct triplet
{
    typedef T1 first_type;
    typedef T2 second_type;
    typedef T3 third_type;

    T1 first;
    T2 second;
    T3 third;

    PSEUDO_CONSTEXPR triplet() : first(), second(), third(){}
    PSEUDO_CONSTEXPR triplet(const T1& a, const T2& b, const T3& c)
        : first(a), second(b), third(c){}

// definition of ctor and assign operator= when only c++98 is available {{{
#if __cplusplus < 201103L
    // c++98 only
    template<typename U1, typename U2, typename U3>
    triplet(const U1& a, const U2& b, const U3& c)
        : first(a), second(b), third(c){}

    triplet& operator=(const triplet& t)
    {
        first  = t.first;
        second = t.second;
        third  = t.third;
        return *this;
    }

    template<typename U1, typename U2, typename U3>
    triplet& operator=(const triplet<U1, U2, U3>& t)
    {
        first  = t.first;
        second = t.second;
        third  = t.third;
        return *this;
    }
// }}}
// member funcs only available in c++11 or later {{{
#else
    template<typename U1, typename U2, typename U3, class = typename 
            std::enable_if<std::is_convertible<const U1&, T1>::value &&
                           std::is_convertible<const U2&, T2>::value &&
                           std::is_convertible<const U3&, T3>::value>::type>
    constexpr triplet(const U1& a, const U2& b, const U3& c)
        : first(a), second(b), third(c){}

    ~triplet() = default;
    constexpr triplet(const triplet&)  = default;
    constexpr triplet(triplet&&)       = default;
    triplet& operator=(const triplet&) = default;
    triplet& operator=(triplet&&)      = default;

    template<typename U1, class = typename std::enable_if<
        std::is_convertible<U1, T1>::value>::type>
    constexpr triplet(U1&& a, const T2& b, const T3& c)
        : first(std::forward<U1>(a)), second(b), third(c){}

    template<typename U2, class = typename std::enable_if<
        std::is_convertible<U2, T2>::value>::type>
    constexpr triplet(const T1& a, U2&& b, const T3& c)
        : first(a), second(std::forward<U2>(b)), third(c){}

    template<typename U3, class = typename std::enable_if<
        std::is_convertible<U3, T3>::value>::type>
    constexpr triplet(const T1& a, const T2& b, U3&& c)
        : first(a), second(b), third(std::forward<U3>(c)){}

    template<typename U1, typename U2, class = typename std::enable_if<
        std::is_convertible<U1, T1>::value&&std::is_convertible<U2, T2>::value
        >::type>
    constexpr triplet(U1&& a, U2&& b, const T3& c)
        : first(std::forward<U1>(a)), second(std::forward<U2>(b)), third(c){}

    template<typename U2, typename U3, class = typename std::enable_if<
        std::is_convertible<U2, T2>::value&&std::is_convertible<U3, T3>::value
        >::type>
    constexpr triplet(const T1& a, U2&& b, U3&& c)
        : first(a), second(std::forward<U2>(b)), third(std::forward<U3>(c)){}

    template<typename U3, typename U1, class = typename std::enable_if<
        std::is_convertible<U3, T3>::value&&std::is_convertible<U1, T1>::value
        >::type>
    constexpr triplet(U1&& a, const T2& b, U3&& c)
        : first(std::forward<U1>(a)), second(b), third(std::forward<U3>(c)){}

    template<typename U1, typename U2, typename U3, class = typename
        std::enable_if<std::is_convertible<U1, T1>::value &&
                       std::is_convertible<U2, T2>::value &&
                       std::is_convertible<U3, T3>::value>::type>
    constexpr triplet(U1&& a, U2&& b, U3&& c)
        : first(std::forward<U1>(a)), second(std::forward<U2>(b)),
          third(std::forward<U3>(c)){}

    template<typename U1, typename U2, typename U3, class = typename
        std::enable_if<std::is_convertible<U1, T1>::value &&
                       std::is_convertible<U2, T2>::value &&
                       std::is_convertible<U3, T3>::value>::type>
    constexpr triplet(triplet<U1, U2, U3>&& t)
        : first(std::forward<U1>(t.first)), second(std::forward<U2>(t.second)),
          third(std::forward<U3>(t.third)){}

    template<typename U1, typename U2, typename U3, class = typename
        std::enable_if<std::is_convertible<U1 const&, T1>::value &&
                       std::is_convertible<U2 const&, T2>::value &&
                       std::is_convertible<U3 const&, T3>::value>::type>
    triplet& operator=(const triplet<U1, U2, U3>& t)
    {
        first  = t.first;
        second = t.second;
        third  = t.third;
        return *this;
    }

    template<typename U1, typename U2, typename U3, class = typename
        std::enable_if<std::is_convertible<U1, T1>::value &&
                       std::is_convertible<U2, T2>::value &&
                       std::is_convertible<U3, T3>::value>::type>
    triplet& operator=(triplet<U1, U2, U3>&& t)
    {
        first  = std::forward<U1>(t.first);
        second = std::forward<U2>(t.second);
        third  = std::forward<U3>(t.third);
        return *this;
    }

    void swap(triplet& t) noexcept(noexcept(std::swap(first,  t.first)) &&
                                   noexcept(std::swap(second, t.second))&&
                                   noexcept(std::swap(third,  t.third)))
    {
        std::swap(this->first,  t.first);
        std::swap(this->second, t.second);
        std::swap(this->third,  t.third);
        return;
    }
#endif // c++11
//}}}

};// class triplet

// comparison operators {{{
template<typename T1, typename T2, typename T3>
inline PSEUDO_CONSTEXPR bool
operator==(const triplet<T1, T2, T3>& lhs, const triplet<T1, T2, T3>& rhs)
{
    return (lhs.first == rhs.first) && (lhs.second == rhs.second) &&
           (lhs.third == rhs.third);
}

template<typename T1, typename T2, typename T3>
inline PSEUDO_CONSTEXPR bool
operator<(const triplet<T1, T2, T3>& lhs, const triplet<T1, T2, T3>& rhs)
{
    return (lhs.first < rhs.first) ||
           (!(lhs.first > rhs.first) && (lhs.second < rhs.second)) || 
           (!(lhs.first > rhs.first) && !(lhs.second > rhs.second) &&
            (lhs.third < rhs.third));
}

template<typename T1, typename T2, typename T3>
inline PSEUDO_CONSTEXPR bool
operator!=(const triplet<T1, T2, T3>& lhs, const triplet<T1, T2, T3>& rhs)
{
    return not(lhs == rhs);
}

template<typename T1, typename T2, typename T3>
inline PSEUDO_CONSTEXPR bool
operator>(const triplet<T1, T2, T3>& lhs, const triplet<T1, T2, T3>& rhs)
{
    return rhs < lhs;
}

template<typename T1, typename T2, typename T3>
inline PSEUDO_CONSTEXPR bool
operator<=(const triplet<T1, T2, T3>& lhs, const triplet<T1, T2, T3>& rhs)
{
    return !(rhs < lhs);
}

template<typename T1, typename T2, typename T3>
inline PSEUDO_CONSTEXPR bool
operator>=(const triplet<T1, T2, T3>& lhs, const triplet<T1, T2, T3>& rhs)
{
    return !(lhs < rhs);
}
// }}}

// make_triplet(x, y, z) helper func {{{
#if __cplusplus >= 201103L
// if c++11 or later is available
template<typename T1, typename T2, typename T3>
constexpr inline triplet<decay_and_strip_t<T1>, decay_and_strip_t<T2>,
    decay_and_strip_t<T3>>
make_triplet(T1&& x, T2&& y, T3&& z)
{
    typedef decay_and_strip_t<T1> ds_t1;
    typedef decay_and_strip_t<T2> ds_t2;
    typedef decay_and_strip_t<T3> ds_t3;
    return triplet<ds_t1, ds_t2, ds_t3>(
            std::forward<T1>(x), std::forward<T2>(y), std::forward<T3>(z));
}
#else
// if only c++98 is available
template<typename T1, typename T2, typename T3>
inline triplet<T1, T2, T3>
make_triplet(T1 x, T2 y, T3 z)
{
    return triplet<T1, T2, T3>(x, y, z);
}
#endif // c++11
// }}}

// get<0> funcs {{{

template<std::size_t i, typename T1, typename T2, typename T3>
struct get_triplet_helper;

template<typename T1, typename T2, typename T3>
struct get_triplet_helper<0, T1, T2, T3>
{
#if __cplusplus < 201103L
    static T1&       get_elem(triplet<T1, T2, T3>& tri)      {return tri.first;}
    static T1 const& get_elem(triplet<T1, T2, T3> const& tri){return tri.first;}
#else 
    static typename std::add_lvalue_reference<T1>::type
    get_elem(triplet<T1, T2, T3>& tri) noexcept {return tri.first;}
    static const typename std::add_lvalue_reference<T1>::type
    get_elem(triplet<T1, T2, T3> const& tri) noexcept {return tri.first;}
    static typename std::add_rvalue_reference<T1>::type
    get_elem(triplet<T1, T2, T3>&& tri) noexcept {return std::forward<T1>(tri.first);}
#endif
};

template<typename T1, typename T2, typename T3>
struct get_triplet_helper<1, T1, T2, T3>
{
#if __cplusplus < 201103L
    static T2&       get_elem(triplet<T1, T2, T3>& tri)       throw() {return tri.second;}
    static T2 const& get_elem(triplet<T1, T2, T3> const& tri) throw() {return tri.second;}
#else 
    static typename std::add_lvalue_reference<T2>::type
    get_elem(triplet<T1, T2, T3>& tri) noexcept {return tri.second;}
    static const typename std::add_lvalue_reference<T2>::type
    get_elem(triplet<T1, T2, T3> const& tri) noexcept {return tri.second;}
    static typename std::add_rvalue_reference<T2>::type
    get_elem(triplet<T1, T2, T3>&& tri) noexcept {return std::forward<T2>(tri.second);}
#endif
};

template<typename T1, typename T2, typename T3>
struct get_triplet_helper<2, T1, T2, T3>
{
#if __cplusplus < 201103L
    static T3&       get_elem(triplet<T1, T2, T3>& tri)      throw(){return tri.third;}
    static T3 const& get_elem(triplet<T1, T2, T3> const& tri)throw(){return tri.third;}
#else 
    static typename std::add_lvalue_reference<T3>::type
    get_elem(triplet<T1, T2, T3>& tri) noexcept {return tri.third;}
    static const typename std::add_lvalue_reference<T3>::type
    get_elem(triplet<T1, T2, T3> const& tri) noexcept {return tri.third;}
    static typename std::add_rvalue_reference<T3>::type
    get_elem(triplet<T1, T2, T3>&& tri) noexcept {return std::forward<T3>(tri.third);}
#endif
};

template<std::size_t i, typename T1, typename T2, typename T3>
struct get_triplet_type;

template<typename T1, typename T2, typename T3>
struct get_triplet_type<0, T1, T2, T3>{typedef T1 type;};

template<typename T1, typename T2, typename T3>
struct get_triplet_type<1, T1, T2, T3>{typedef T2 type;};

template<typename T1, typename T2, typename T3>
struct get_triplet_type<2, T1, T2, T3>{typedef T3 type;};

#if __cplusplus < 201103L

template<std::size_t i, typename T1, typename T2, typename T3>
inline typename get_triplet_type<i, T1, T2, T3>::type&
get(triplet<T1, T2, T3>& tri) throw()
{
    return get_triplet_helper<i, T1, T2, T3>::get_elem(tri);
}
template<std::size_t i, typename T1, typename T2, typename T3>
inline typename get_triplet_type<i, T1, T2, T3>::type const&
get(const triplet<T1, T2, T3>& tri) throw()
{
    return get_triplet_helper<i, T1, T2, T3>::get_elem(tri);
}
#else // c++11

template<std::size_t i, typename T1, typename T2, typename T3>
inline constexpr typename std::add_lvalue_reference<
    typename get_triplet_type<i, T1, T2, T3>::type>::type
get(triplet<T1, T2, T3>& tri) noexcept
{
    return get_triplet_helper<i, T1, T2, T3>::get_elem(tri);
}

template<std::size_t i, typename T1, typename T2, typename T3>
inline constexpr typename std::add_lvalue_reference<
    const typename get_triplet_type<i, T1, T2, T3>::type>::type
get(const triplet<T1, T2, T3>& tri) noexcept
{
    return get_triplet_helper<i, T1, T2, T3>::get_elem(tri);
}

template<std::size_t i, typename T1, typename T2, typename T3>
inline constexpr typename std::add_rvalue_reference<
    typename get_triplet_type<i, T1, T2, T3>::type>::type
get(const triplet<T1, T2, T3>& tri) noexcept
{
    return std::forward<typename get_triplet_type<i, T1, T2, T3>::type>(
            get_triplet_helper<i, T1, T2, T3>::get_elem(tri));
}
#endif

// }}}
} // psd

#endif /* PSEUDO_TRIPLET */
