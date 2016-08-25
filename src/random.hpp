#ifndef PSEUDO_RANDOM
#define PSEUDO_RANDOM

#if __cplusplus >= 201103L
#include <limits>
#include <random>
#include <cstdint>

namespace psd
{

class xorshift32
{
  public:

    xorshift32(){std::random_device rd; seed_ = rd();}
    xorshift32(const std::uint_fast32_t s) : seed_(s){}
    ~xorshift32() = default;

    std::uint_fast32_t operator()();
    constexpr static std::uint_fast32_t max()
    {return std::numeric_limits<std::uint_fast32_t>::max();}
    constexpr static std::uint_fast32_t min(){return 0;}

  private:
    std::uint32_t seed_;
};

inline std::uint_fast32_t xorshift32::operator()()
{
    seed_ ^= (seed_ << 13);
    seed_ ^= (seed_ >> 17);
    return seed_ ^= (seed_ << 5);
}


class xorshift64
{
  public:

    xorshift64(){std::random_device rd; seed_ = rd();}
    xorshift64(const std::uint_fast64_t s) : seed_(s){}
    ~xorshift64() = default;

    std::uint_fast64_t operator()();
    constexpr static std::uint_fast64_t max()
    {return std::numeric_limits<std::uint_fast64_t>::max();}
    constexpr static std::uint_fast64_t min(){return 0;}

  private:
    std::uint_fast64_t seed_;
};

inline std::uint_fast64_t xorshift64::operator()()
{
    seed_ ^= (seed_ << 13);
    seed_ ^= (seed_ >> 7);
    return seed_ ^= (seed_ << 17);
}

class xorshift96
{
  public:

    xorshift96() : z(521288629){}
    xorshift96(const std::uint_fast32_t s) : z(s){}
    ~xorshift96() = default;

    std::uint_fast32_t operator()();
    constexpr static std::uint_fast32_t max()
    {return std::numeric_limits<std::uint_fast32_t>::max();}
    constexpr static std::uint_fast32_t min(){return 0;}

  private:
    std::uint_fast32_t x = 123456789;
    std::uint_fast32_t y = 362436069;
    std::uint_fast32_t z;
};

inline std::uint_fast32_t xorshift96::operator()()
{
    const std::uint_fast32_t tmp =
        (x ^ (x << 3)) ^ (y ^ (y >> 19)) ^ (z ^ (z << 6));
    x = y; y = z;
    return z = tmp;
}

class xorshift128
{
  public:

    xorshift128(): w(88675123) {}
    xorshift128(const std::uint_fast32_t s) : w(s){}
    ~xorshift128() = default;

    std::uint_fast32_t operator()();
    constexpr static std::uint_fast32_t max()
    {return std::numeric_limits<std::uint_fast32_t>::max();}
    constexpr static std::uint_fast32_t min(){return 0;}

  private:
    std::uint_fast32_t x = 123456789;
    std::uint_fast32_t y = 362436069;
    std::uint_fast32_t z = 521288629;
    std::uint_fast32_t w;
};

inline std::uint_fast32_t xorshift128::operator()()
{
    const std::uint_fast32_t tmp = x ^ (x << 11);
    x = y; y = z; z = w;
    return w = (w ^ (w >> 19)) ^ (tmp ^ (tmp >> 8));
}


}//psd
#endif /* c++11 */
#endif /* PSEUDO_RANDOM */
