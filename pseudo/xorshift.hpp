#ifndef PSEUDO_XORSHIFT
#define PSEUDO_XORSHIFT

/* This is based on the article "Xorshift RNGs" by George Marsaglia (2003)    *
 * Journal of Statistical Software Vol. 8 (Issue  14). All parameters used    *
 * here in default are taken from the article.                                */

#if __cplusplus < 201103L
#error "pseudo/xorshift is for c++11 random"
#endif

#include <type_traits>
#include <limits>
#include <array>
#include <cstdint>

namespace psd
{

enum class xorshift_period_tag
{
    p32,
    p64,
    p96,
    p128,
    p160,
};

template<typename uintT, uintT a, uintT b, uintT c, xorshift_period_tag>
class xorshift_engine;

template<typename uintT, uintT a, uintT b, uintT c>
class xorshift_engine<uintT, a, b, c, xorshift_period_tag::p32>
{
  public:
    static_assert(std::is_unsigned<uintT>::value,
                  "xorshift engine require unsigned integer type");

    using uint_type = uintT;
    using result_type = uint_type;
    constexpr static uint_type A = a;
    constexpr static uint_type B = b;
    constexpr static uint_type C = c;
    constexpr static std::size_t word_size  = 32;
    constexpr static std::size_t state_size = 1;

  public:

    xorshift_engine() : x_(2463534242){}
    ~xorshift_engine() = default;

    template<typename T, typename std::enable_if<
        std::is_integral<T>::value, std::nullptr_t>::type = nullptr>
    xorshift_engine(T s) : x_(static_cast<uint_type>(s)){}

    template<typename T, typename std::enable_if<
        std::is_class<T>::value, std::nullptr_t>::type = nullptr>
    xorshift_engine(T& s){this->seed(s);}

    template<typename T, typename std::enable_if<
        std::is_integral<T>::value, std::nullptr_t>::type = nullptr>
    void seed(T s) noexcept
    {
        x_ = static_cast<uint_type>(s);
    }

    template<typename T, typename std::enable_if<
        std::is_class<T>::value, std::nullptr_t>::type = nullptr>
    void seed(T& s)
    {// assuming T is std::seed_seq or compatible class
        std::array<uint_type, 1> tmp;
        s.generate(tmp.begin(), tmp.end());
        x_ = tmp.front();
        return;
    }

    xorshift_engine(xorshift_engine const& rhs) = default;
    xorshift_engine(xorshift_engine&&      rhs) = default;

    result_type operator()() noexcept
    {
        x_ ^= (x_ << A);
        x_ ^= (x_ >> B);
        return x_ ^= (x_ << C);
    }
    void discard(std::size_t z) noexcept
    {
        while(z-- != 0){this->operator()();}
        return;
    }

    static constexpr
    result_type max(){return std::numeric_limits<result_type>::max();}
    static constexpr
    result_type min(){return std::numeric_limits<result_type>::min();}

  private:
    result_type x_;
};

template<typename uintT, uintT a, uintT b, uintT c>
class xorshift_engine<uintT, a, b, c, xorshift_period_tag::p64>
{
  public:
    static_assert(std::is_unsigned<uintT>::value,
                  "xorshift engine require unsigned integer type");

    using uint_type   = uintT;
    using result_type = uint_type;
    constexpr static uint_type A = a;
    constexpr static uint_type B = b;
    constexpr static uint_type C = c;
    constexpr static std::size_t word_size  = 64;
    constexpr static std::size_t state_size = 1;

  public:

    xorshift_engine() : x_(88172645463325252){}
    ~xorshift_engine() = default;

    template<typename T, typename std::enable_if<
        std::is_integral<T>::value, std::nullptr_t>::type = nullptr>
    xorshift_engine(T s) : x_(static_cast<uint_type>(s)){}
    template<typename T, typename std::enable_if<
        std::is_class<T>::value, std::nullptr_t>::type = nullptr>
    xorshift_engine(T& s){this->seed(s);}

    template<typename T, typename std::enable_if<
        std::is_integral<T>::value, std::nullptr_t>::type = nullptr>
    void seed(T s) noexcept
    {
        x_ = static_cast<uint_type>(s);
    }

    template<typename T, typename std::enable_if<
        std::is_class<T>::value, std::nullptr_t>::type = nullptr>
    void seed(T& s)
    {// assuming T is std::seed_seq or compatible class
        std::array<std::uint32_t, 2> tmp;
        s.generate(tmp.begin(), tmp.end());
        x_ = (static_cast<uint_type>(tmp[0]) << 32) ^ static_cast<uint_type>(tmp[1]);
        return;
    }

    xorshift_engine(xorshift_engine const& rhs) = default;
    xorshift_engine(xorshift_engine&&      rhs) = default;

    result_type operator()() noexcept
    {
        x_ ^= (x_ << A);
        x_ ^= (x_ >> B);
        return x_ ^= (x_ << C);
    }
    void discard(std::size_t z) noexcept
    {
        while(z-- != 0){this->operator()();}
        return;
    }

    static constexpr
    result_type max(){return std::numeric_limits<result_type>::max();}
    static constexpr
    result_type min(){return std::numeric_limits<result_type>::min();}

  private:
    result_type x_;
};

template<typename uintT, uintT a, uintT b, uintT c>
class xorshift_engine<uintT, a, b, c, xorshift_period_tag::p96>
{
  public:
    static_assert(std::is_unsigned<uintT>::value,
                  "xorshift engine require unsigned integer type");

    using uint_type   = uintT;
    using result_type = uint_type;
    constexpr static uint_type A = a;
    constexpr static uint_type B = b;
    constexpr static uint_type C = c;
    constexpr static std::size_t word_size  = 32;
    constexpr static std::size_t state_size = 3;

  public:

    xorshift_engine() : x_(123456789), y_(362436069), z_(521288629){}
    ~xorshift_engine() = default;

    template<typename T, typename std::enable_if<
        std::is_integral<T>::value, std::nullptr_t>::type = nullptr>
    xorshift_engine(T s)
    : x_(static_cast<uint_type>(s)), y_(362436069), z_(521288629){}

    template<typename T, typename std::enable_if<
        std::is_class<T>::value, std::nullptr_t>::type = nullptr>
    xorshift_engine(T& s){this->seed(s);}

    template<typename T, typename std::enable_if<
        std::is_class<T>::value, std::nullptr_t>::type = nullptr>
    void seed(T& s)
    {// assuming T is std::seed_seq or compatible class
        std::array<uint_type, state_size> tmp;
        s.generate(tmp.begin(), tmp.end());
        x_ = tmp[0];
        y_ = tmp[1];
        z_ = tmp[2];
        return;
    }

    xorshift_engine(xorshift_engine const& rhs) = default;
    xorshift_engine(xorshift_engine&&      rhs) = default;

    result_type operator()() noexcept
    {
        const result_type t = (x_ ^ (x_ << A)); x_=y_; y_=z_;
        return z_ = (z_ ^ (z_ >> C)) ^ (t ^ (t >> B));
    }
    void discard(std::size_t count) noexcept
    {
        while(count-- != 0){this->operator()();}
        return;
    }

    static constexpr
    result_type max(){return std::numeric_limits<result_type>::max();}
    static constexpr
    result_type min(){return std::numeric_limits<result_type>::min();}

  private:
    result_type x_, y_, z_;
};

template<typename uintT, uintT a, uintT b, uintT c>
class xorshift_engine<uintT, a, b, c, xorshift_period_tag::p128>
{
  public:
    static_assert(std::is_unsigned<uintT>::value,
                  "xorshift engine require unsigned integer type");

    using uint_type   = uintT;
    using result_type = uint_type;
    constexpr static uint_type A = a;
    constexpr static uint_type B = b;
    constexpr static uint_type C = c;
    constexpr static std::size_t word_size  = 32;
    constexpr static std::size_t state_size = 4;

  public:

    xorshift_engine(): x_(123456789), y_(362436069), z_(521288629), w_(88675123)
    {}
    ~xorshift_engine() = default;

    xorshift_engine(xorshift_engine const& rhs) = default;
    xorshift_engine(xorshift_engine&&      rhs) = default;

    template<typename T, typename std::enable_if<
        std::is_integral<T>::value, std::nullptr_t>::type = nullptr>
    xorshift_engine(T s) : x_(static_cast<uint_type>(s)),
        y_(362436069), z_(521288629), w_(88675123)
    {}
    template<typename T, typename std::enable_if<
        std::is_class<T>::value, std::nullptr_t>::type = nullptr>
    xorshift_engine(T& s){this->seed(s);}

    template<typename T, typename std::enable_if<
        std::is_integral<T>::value, std::nullptr_t>::type = nullptr>
    void seed(T s) noexcept
    {
        x_ = static_cast<uint_type>(s);
    }

    template<typename T, typename std::enable_if<
        std::is_class<T>::value, std::nullptr_t>::type = nullptr>
    void seed(T& s)
    {// assuming T is std::seed_seq or compatible class
        std::array<uint_type, state_size> tmp;
        s.generate(tmp.begin(), tmp.end());
        x_ = tmp[0];
        y_ = tmp[1];
        z_ = tmp[2];
        w_ = tmp[3];
        return;
    }

    result_type operator()() noexcept
    {
        const result_type t = (x_ ^ (x_ << A)); x_ = y_; y_ = z_; z_ = w_;
        return w_ = (w_ ^ (w_ >> C)) ^ (t ^ (t >> B));
    }
    void discard(std::size_t count) noexcept
    {
        while(count-- != 0){this->operator()();}
        return;
    }

    static constexpr
    result_type max(){return std::numeric_limits<result_type>::max();}
    static constexpr
    result_type min(){return std::numeric_limits<result_type>::min();}

  private:
    result_type x_, y_, z_, w_;
};

template<typename uintT, uintT a, uintT b, uintT c>
class xorshift_engine<uintT, a, b, c, xorshift_period_tag::p160>
{
  public:
    static_assert(std::is_unsigned<uintT>::value,
                  "xorshift engine require unsigned integer type");

    using uint_type   = uintT;
    using result_type = uint_type;
    constexpr static uint_type A = a;
    constexpr static uint_type B = b;
    constexpr static uint_type C = c;
    constexpr static std::size_t word_size  = 32;
    constexpr static std::size_t state_size = 5;

  public:

    xorshift_engine()
    : x_(123456789), y_(362436069), z_(521288629), w_(88675123), v_(5783321)
    {}
    ~xorshift_engine() = default;

    xorshift_engine(xorshift_engine const& rhs) = default;
    xorshift_engine(xorshift_engine&&      rhs) = default;

    template<typename T, typename std::enable_if<
        std::is_integral<T>::value, std::nullptr_t>::type = nullptr>
    xorshift_engine(T s)
    : x_(static_cast<uint_type>(s)), y_(362436069), z_(521288629), w_(88675123),
      v_(5783321)
    {}

    template<typename T, typename std::enable_if<
        std::is_class<T>::value, std::nullptr_t>::type = nullptr>
    xorshift_engine(T& s){this->seed(s);}

    template<typename T, typename std::enable_if<
        std::is_class<T>::value, std::nullptr_t>::type = nullptr>
    void seed(T& s)
    {// assuming T is std::seed_seq or compatible class
        std::array<uint_type, state_size> tmp;
        s.generate(tmp.begin(), tmp.end());
        x_ = tmp[0];
        y_ = tmp[1];
        z_ = tmp[2];
        w_ = tmp[3];
        v_ = tmp[4];
        return;
    }

    result_type operator()() noexcept
    {
        const result_type t = (x_ ^ (x_ >> A));
        x_ = y_; y_ = z_; z_ = w_; w_ = v_;
        return v_ = (v_ ^ (v_ >> C)) ^ (t ^ (t >> B));
    }
    void discard(std::size_t count) noexcept
    {
        while(count-- != 0){this->operator()();}
        return;
    }

    static constexpr
    result_type max(){return std::numeric_limits<result_type>::max();}
    static constexpr
    result_type min(){return std::numeric_limits<result_type>::min();}

  private:
    result_type x_, y_, z_, w_, v_;
};

typedef xorshift_engine<std::uint32_t, 13, 17, 5, xorshift_period_tag::p32>
        xorshift32;
typedef xorshift_engine<std::uint64_t, 13, 7, 17, xorshift_period_tag::p64>
        xorshift64;
typedef xorshift_engine<std::uint32_t,  3, 19, 6, xorshift_period_tag::p96>
        xorshift96;
typedef xorshift_engine<std::uint32_t, 11, 19, 8, xorshift_period_tag::p128>
        xorshift128;
typedef xorshift_engine<std::uint32_t,  2,  1, 4, xorshift_period_tag::p160>
        xorshift160;

}//namespace psd
#endif //PSEUDO_XORSHIFT
