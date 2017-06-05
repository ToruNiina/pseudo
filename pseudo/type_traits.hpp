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
std::true_type is_iterator_impl(int);
template<typename T>
std::false_type is_iterator_impl(long);

} // detail

template<typename T>
struct is_iterator : decltype(detail::is_iterator_impl<T>(0)){};

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
: public std::conditional<T1::value, T1, _or_<T2, T3, Ts...>>::type {};

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

}//psd
#endif /* PSEUDO_TYPE_TRAITS */
