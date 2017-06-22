#ifndef PSEUDO_UTILITY
#define PSEUDO_UTILITY
#include <utility>
#include <functional>

namespace psd
{
namespace detail
{

template<typename T, T ... values>
struct index_tuple{};

template<typename T, T value, typename Tuple>
struct index_tuple_push_back;

template<typename T, T value, T ... values>
struct index_tuple_push_back<T, value, index_tuple<T, values...>>
{
    typedef index_tuple<T, values..., value> type;
};

template<std::size_t N>
struct index_tuple_generator
{
    typedef typename index_tuple_push_back<
        std::size_t, N, typename index_tuple_generator<N-1>::type>::type type;
};

template<>
struct index_tuple_generator<0>
{
    typedef index_tuple<std::size_t, 0> type;
};

template<typename ... Ts>
constexpr inline typename index_tuple_generator<sizeof...(Ts)-1>::type
make_index_tuple(std::tuple<Ts...>)
{
    return typename index_tuple_generator<sizeof...(Ts)-1>::type{};
}


template<typename T>
struct strip_reference_wrapper
{
    typedef T type;
};

template<typename T>
struct strip_reference_wrapper<std::reference_wrapper<T>>
{
    typedef T& type;
};

template<typename T>
struct decay_and_strip
{
    typedef typename strip_reference_wrapper<typename std::decay<T>::type>::type type;
};

} // detail
} // psd
#endif//PSEUDO_UTILITY
