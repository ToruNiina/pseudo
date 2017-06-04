Pseudo
====

pseudo is a STL like header only template library.

## random

pseudo provides STL-compatible `xorshift` engine [Mersagria (2003), _J STAT SOFTW_].

### synopsis

```cpp
namespace psd
{
template<typename uintT, uintT a, uintT b, uintT c, xorshift_period_tag>
class xorshift_engine;

using xorshift32  = xorshift_engine<std::uint32_t, 13, 17,  5, xorshift_period_tag::p32>;
using xorshift64  = xorshift_engine<std::uint64_t, 13,  7, 17, xorshift_period_tag::p64>;
using xorshift96  = xorshift_engine<std::uint32_t,  3, 19,  6, xorshift_period_tag::p96>;
using xorshift128 = xorshift_engine<std::uint32_t, 11, 19,  8, xorshift_period_tag::p128>;
using xorshift160 = xorshift_engine<std::uint32_t,  2,  1,  4, xorshift_period_tag::p160>;
}
```

### example

`psd::xorshift` can be used in the same way as STL RNG engine.

```cpp
#include <pseudo/random.hpp>
int main()
{
    psd::xorshift32 x; // use default seed
    std::uniform_real_distribution<double> uni(0., 1.);
    for(std::size_t i=0; i<100; ++i)
        std::cout << uni(x) << std::endl;
    return 0;
}
```

