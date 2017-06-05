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

}//psd
#endif /* PSEUDO_TYPE_TRAITS */
