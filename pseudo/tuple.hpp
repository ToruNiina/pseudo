#ifndef PSEUDO_TUPLE
#define PSEUDO_TUPLE
#include <type_traits>
#include <tuple>

namespace psd
{

template<typename ... Tuples>
struct tuple_concat
{
    typedef decltype(std::tuple_cat(std::declval<Tuples>()...)) type;
};

template<typename ... Tuples>
using tuple_concat_t = typename tuple_concat<Tuples...>::type;

namespace detail
{

template<std::size_t I, std::size_t N, typename Tuple>
struct tuple_take_impl
{
    typedef psd::tuple_concat_t<
        std::tuple<typename std::tuple_element<I, Tuple>::type>,
        typename tuple_take_impl<I+1, N, Tuple>::type> type;
};

template<std::size_t N, typename Tuple>
struct tuple_take_impl<N, N, Tuple>
{
    typedef std::tuple<> type;
};

template<std::size_t I, typename Tuple>
struct tuple_take_impl<I, 0, Tuple>
{
    typedef std::tuple<> type;
};

} // detail

template<std::size_t I, typename Tuple>
struct tuple_take_front
{
    static_assert(I <= std::tuple_size<Tuple>::value, "take_front more than size");
    typedef typename detail::tuple_take_impl<0, I, Tuple>::type type;
};

template<std::size_t I, typename Tuple>
using tuple_take_front_t = typename tuple_take_front<I, Tuple>::type;

template<std::size_t I, typename Tuple>
struct tuple_take_back
{
    static_assert(I <= std::tuple_size<Tuple>::value, "take_back more than size");
    typedef typename detail::tuple_take_impl<
        I, std::tuple_size<Tuple>::value, Tuple>::type type;
};

template<std::size_t I, typename Tuple>
using tuple_take_back_t = typename tuple_take_back<I, Tuple>::type;

template<template<typename>class F, typename Tuple>
struct tuple_transform;

template<template<typename> class F, typename T, typename ... Ts>
struct tuple_transform<F, std::tuple<T, Ts...>>
{
    typedef tuple_concat_t<std::tuple<typename F<T>::type>,
            typename tuple_transform<F, std::tuple<Ts...>>::type> type;
};

template<template<typename> class F, typename T>
struct tuple_transform<F, std::tuple<T>>
{
    typedef std::tuple<typename F<T>::type> type;
};

template<template<typename> class F>
struct tuple_transform<F, std::tuple<>>
{
    typedef std::tuple<> type;
};

template<template<typename>class F, typename Tuple>
using tuple_transform_t = typename tuple_transform<F, Tuple>::type;

}//psd
#endif /* PSEUDO_TUPLE */
