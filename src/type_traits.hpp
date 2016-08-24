#ifndef PSEUDO_TYPE_TRAITS
#define PSEUDO_TYPE_TRAITS

#if __cplusplus >= 201103L
#include <type_traits>

// forward decl (not good?)
namespace std
{
template<class T> class reference_wrapper;
}//std

namespace psd
{

template<typename T>
strip_reference_wrapper
{
    typename T type;
};

template<typename T>
strip_reference_wrapper<reference_wrapper<T>>
{
    typename T& type;
};

template<typename T>
using strip_ref_t = strip_reference_wrapper<T>::type;

template<typename T>
struct decay_and_strip
{
    typedef typename std::decay<T> decayed_type;
    typedef typename strip_ref_t<decayed_type> type;
};

template<typename T>
using decay_and_strip_t = decay_and_strip<T>::type;

}//psd

#endif // c++11
#endif /* PSEUDO_TYPE_TRAITS */
