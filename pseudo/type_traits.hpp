#ifndef PSEUDO_TYPE_TRAITS
#define PSEUDO_TYPE_TRAITS
#include <type_traits>
#include <iterator>

namespace psd
{

namespace detail
{

template<typename T, class = typename std::iterator_traits<T>::value_type,
         class = typename std::iterator_traits<T>::pointer,
         class = typename std::iterator_traits<T>::reference,
         class = typename std::iterator_traits<T>::difference_type,
         class = typename std::iterator_traits<T>::iterator_category>
std::true_type  is_iterator_impl(int);
template<typename T>
std::false_type is_iterator_impl(long);

template<typename Iterator>
struct is_const_iterator_judge_const_impl
{
    typedef typename std::iterator_traits<Iterator>::pointer pointer;
    typedef bool value_type;
    constexpr static value_type value = std::is_const<
        typename std::remove_pointer<pointer>::type>::value;
    typedef std::integral_constant<value_type, value> type;

    constexpr operator value_type()   const {return value;}
    constexpr value_type operator()() const {return value;}
};

template<typename T, class = typename std::iterator_traits<T>::value_type,
         class = typename std::iterator_traits<T>::pointer,
         class = typename std::iterator_traits<T>::reference,
         class = typename std::iterator_traits<T>::difference_type,
         class = typename std::iterator_traits<T>::iterator_category>
is_const_iterator_judge_const_impl<T> is_const_iterator_impl(int);
template<typename T>
std::false_type is_const_iterator_impl(long);

template<typename T, class = typename T::iterator,
         decltype(std::declval<T>().begin())* = nullptr,
         decltype(std::declval<T>().end())* = nullptr>
std::true_type has_iterator_impl(int);
template<typename T>
std::false_type has_iterator_impl(long);

} // detail

template<typename T>
struct is_iterator : decltype(detail::is_iterator_impl<T>(0)){};
template<typename T>
struct is_const_iterator : decltype(detail::is_const_iterator_impl<T>(0)){};
template<typename T>
struct has_iterator : decltype(detail::has_iterator_impl<T>(0)){};



template<typename T>
struct iterator_of
{
    typedef typename T::iterator               iterator;
    typedef typename T::const_iterator         const_iterator;
    typedef typename T::reverse_iterator       reverse_iterator;
    typedef typename T::const_reverse_iterator const_reverse_iterator;
};

template<typename T>
struct iterator_of<const T>
{
    typedef typename T::const_iterator         iterator;
    typedef typename T::const_iterator         const_iterator;
    typedef typename T::const_reverse_iterator reverse_iterator;
    typedef typename T::const_reverse_iterator const_reverse_iterator;
};

template<typename T, std::size_t N>
struct iterator_of<T[N]>
{
    typedef T*       iterator;
    typedef const T* const_iterator;
    typedef std::reverse_iterator<T*>       reverse_iterator;
    typedef std::reverse_iterator<const T*> const_reverse_iterator;
};

template<typename T, std::size_t N>
struct iterator_of<const T[N]>
{
    typedef const T* iterator;
    typedef const T* const_iterator;
    typedef std::reverse_iterator<const T*> reverse_iterator;
    typedef std::reverse_iterator<const T*> const_reverse_iterator;
};


template<typename ...>
struct meta_or;
template<>
struct meta_or<> : public std::false_type {};
template<typename T>
struct meta_or<T> : public T {};
template<typename T1, typename T2>
struct meta_or<T1, T2> : public std::conditional<T1::value, T1, T2>::type {};
template<typename T1, typename T2, typename T3, typename ... Ts>
struct meta_or<T1, T2, T3, Ts...>
: public std::conditional<T1::value, T1, meta_or<T2, T3, Ts...>>::type {};

template<typename ...>
struct meta_and;
template<>
struct meta_and<> : public std::true_type {};
template<typename T>
struct meta_and<T> : public T {};
template<typename T1, typename T2>
struct meta_and<T1, T2> : public std::conditional<T1::value, T2, T1>::type {};
template<typename T1, typename T2, typename T3, typename ... Ts>
struct meta_and<T1, T2, T3, Ts...>
: public std::conditional<T1::value, meta_and<T2, T3, Ts...>, T1>::type {};

template<typename T>
struct meta_not : public std::integral_constant<bool, !T::value>{};

template<template<typename T> class opT, typename ... Ts>
struct is_all;
template<template<typename T> class opT, typename T>
struct is_all<opT, T> : opT<T>{};
template<template<typename T> class opT, typename T, typename ...Ts>
struct is_all<opT, T, Ts...> : meta_and<opT<T>, is_all<opT, Ts...>>{};

}//psd
#endif /* PSEUDO_TYPE_TRAITS */
