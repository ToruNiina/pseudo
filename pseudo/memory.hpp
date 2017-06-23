#ifndef PSEUDO_MEMORY
#define PSEUDO_MEMORY
#include <pseudo/observer_ptr.hpp>
#include <memory>

namespace psd
{

template<typename T, typename ... Ts>
inline std::unique_ptr<T> make_unique(Ts&& ... args)
{
    return std::unique_ptr<T>(new T(std::forward<Ts>(args)...));
}

} // psd
#endif// PSEUDO_MEMORY
