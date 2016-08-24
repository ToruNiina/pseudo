#ifndef PSEUDO_DEFINITIONS
#define PSEUDO_DEFINITIONS

#if __cplusplus >= 201103L
// c++11 or later
#define PSEUDO_CONSTEXPR constexpr
#define PSEUDO_NOEXCEPT  noexcept
#define PSEUDO_OVERRIDE  override
#define PSEUDO_NULLPTR   nullptr
#else
// c++98 only
#define PSEUDO_CONSTEXPR
#define PSEUDO_NOEXCEPT  throw()
#define PSEUDO_OVERRIDE
#define PSEUDO_NULLPTR   NULL
#endif

namespace psd
{

}//psd

#endif/* PSEUDO_DEFINITIONS */
