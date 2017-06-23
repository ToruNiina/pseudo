pseudo
====

[![Build Status](https://travis-ci.org/ToruNiina/pseudo.svg?branch=master)](https://travis-ci.org/ToruNiina/pseudo)

pseudo is a STL like header only template library. compatible with c++11.
It depends only on standard template library.

All the functions and classes are in namespace `psd`.

## table of contents

- [xorshift\_engine](#xorshift\_engine)
- [zip\_iterator](#zip\_iterator)
- [observer\_ptr](#observer\_ptr)
- [triplet](#triplet)

## random

## xorshift\_engine

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

You can use it with standard `distribution`s.

```cpp
#include <pseudo/random.hpp>
#include <random>
int main()
{
    psd::xorshift32 x; // use default seed
    std::uniform_real_distribution<double> uni(0., 1.);
    for(std::size_t i=0; i<100; ++i)
        std::cout << uni(x) << std::endl;
    return 0;
}
```

You can set the seed value to the engine.

```cpp
#include <pseudo/random.hpp>
#include <random>
#include <iostream>
int main()
{
    psd::xorshift32 x(12345u);
    std::uniform_real_distribution<double> uni(0., 1.);
    for(std::size_t i=0; i<100; ++i)
        std::cout << uni(x) << std::endl;
    return 0;
}
```

You can also use `std::seed_seq` as a seed parameter.

```cpp
#include <pseudo/random.hpp>
#include <random>
#include <iostream>
int main()
{
    std::seed_seq s{1,2,3};
    psd::xorshift32 x(s);
    std::uniform_real_distribution<double> uni(0., 1.);
    for(std::size_t i=0; i<100; ++i)
        std::cout << uni(x) << std::endl;
    return 0;
}
```

## zip\_iterator

It enables operate several iterators at once with tuple-like interface.

### synopsis

```cpp
namespace psd
{

template<typename ... Ts>
class zip_iterator
{
    zip_iterator()  = default;
    ~zip_iterator() = default;
    zip_iterator(zip_iterator && rhs)      = default;
    zip_iterator(zip_iterator const & rhs) = default;
    zip_iterator& operator=(zip_iterator && rhs)      = default;
    zip_iterator& operator=(zip_iterator const & rhs) = default;

    zip_iterator(Ts&& ... args) noexcept(psd::is_all<
            std::is_nothrow_move_constructible, Ts...>::value);
    zip_iterator(Ts const& ... args) noexcept(psd::is_all<
            std::is_nothrow_copy_constructible, Ts...>::value);

    /* std::tuple<0th value_type&, ...> */  operator*() const noexcept;
    /* std::tuple<0th value_type*, ...> */ operator->() const noexcept;

    zip_iterator& operator++()    noexcept;
    zip_iterator  operator++(int) noexcept;
    zip_iterator& operator--()    noexcept;
    zip_iterator  operator--(int) noexcept;
    zip_iterator& operator+=(difference_type d) noexcept;
    zip_iterator& operator-=(difference_type d) noexcept;

    template<std::size_t i>
    /* <i-th iterator> */::value_type& ref() noexcept;
    template<std::size_t i>
    /* <i-th iterator> */::value_type* ptr() noexcept;

    container_type const& base() const noexcept;
};

}
```

### example

You can zip several iterators.


```cpp
#include<pseudo/zip_iterator.hpp>
#include<vector>
#include<list>
#include<forward_list>

std::vector<int>        v1{1,  2,   3,    4,     5};
std::vector<double>     v2{3., 3.1, 3.14, 3.141, 3.1415};
std::list<char>         ls{'a', 'b', 'c', 'd', 'e'};
std::forward_list<char> fl{'a', 'b', 'c', 'd', 'e'};

for(auto iter(psd::make_zip_iterator(v1.begin(), v2.begin(), ls.begin(), fl.begin())),
         end(psd::make_zip_iterator(v1.end(), v2.end(), ls.end(), fl.end()));
    iter != end; ++iter)
{
    std::cout << std::get<0>(*iter) << ", "
              << std::get<1>(*iter) << ", "
              << std::get<2>(*iter) << ", "
              << std::get<3>(*iter) << std::endl;

    std::cout << iter.ref<0>() << ", "
              << iter.ref<1>() << ", "
              << iter.ref<2>() << ", "
              << iter.ref<3>() << std::endl;

    std::cout << *(iter.ptr<0>()) << ", "
              << *(iter.ptr<1>()) << ", "
              << *(iter.ptr<2>()) << ", "
              << *(iter.ptr<3>()) << std::endl;
}
```

You can also zip containers in range-based for loop.

```cpp
#include<pseudo/zip_iterator.hpp>
#include<vector>
#include<list>
#include<forward_list>

std::vector<int>        v1{1,  2,   3,    4,     5};
std::vector<double>     v2{3., 3.1, 3.14, 3.141, 3.1415};
std::list<char>         ls{'a', 'b', 'c', 'd', 'e'};
std::forward_list<char> fl{'a', 'b', 'c', 'd', 'e'};

for(const auto& item : psd::make_zip(v1, v2, ls, fl))
{
    std::cout << std::get<0>(item) << ", "
              << std::get<1>(item) << ", "
              << std::get<2>(item) << ", "
              << std::get<3>(item) << std::endl;
    // "1, 3, a, a"
    // ...
}
```

## observer\_ptr

The world dumbest smart pointer (N4282) in c++11. Representing ownerless pointer.

### synopsis

```cpp
namespace psd
{

template<typename T>
class observer_ptr
{
    using element_type = T;
    using pointer      = typename std::add_pointer<T>::type;
    using reference    = typename std::add_lvalue_reference<T>::type;

    constexpr observer_ptr() noexcept : resource_(nullptr) {}
    ~observer_ptr() noexcept  = default;
    constexpr observer_ptr(const observer_ptr&)  noexcept = default;
    constexpr observer_ptr(observer_ptr&&)       noexcept = default;
    observer_ptr& operator=(const observer_ptr&) noexcept = default;
    observer_ptr& operator=(observer_ptr&&)      noexcept = default;

    constexpr observer_ptr(std::nullptr_t) noexcept;
    constexpr explicit observer_ptr(pointer ptr) noexcept;

    template<typename U>
    constexpr observer_ptr(observer_ptr<U> other) noexcept;

    constexpr pointer get() const noexcept;
    constexpr pointer  operator->() const noexcept;
    constexpr reference operator*() const noexcept;

    constexpr explicit operator bool()    const noexcept;
    constexpr explicit operator pointer() const noexcept;

    pointer release() noexcept;
    void    reset(pointer ptr = nullptr) noexcept;
    void    swap(observer_ptr& other) noexcept;
};

template<typename T>
void swap(observer_ptr<T>& lhs, observer_ptr<T>& rhs);

template<typename T>
constexpr observer_ptr<T> make_observer(T* ptr) noexcept;

template<typename T>
constexpr bool operator==(observer_ptr<T> const& lhs, observer_ptr<T> const& rhs);
template<typename T>
constexpr bool operator!=(observer_ptr<T> const& lhs, observer_ptr<T> const& rhs);
template<typename T>
constexpr bool operator< (observer_ptr<T> const& lhs, observer_ptr<T> const& rhs);
template<typename T>
constexpr bool operator<=(observer_ptr<T> const& lhs, observer_ptr<T> const& rhs);
template<typename T>
constexpr bool operator> (observer_ptr<T> const& lhs, observer_ptr<T> const& rhs);
template<typename T>
constexpr bool operator>=(observer_ptr<T> const& lhs, observer_ptr<T> const& rhs);
} // psd

namespace std
{
template<typename T>
struct hash<psd::observer_ptr<T>>{/* same as std::hash<T*>{} */};
}// std
```

### example

It does not do anything more than thing raw pointer does.

```cpp
int *ip = new int{42};
{
    psd::observer_ptr<int> optr = psd::make_observer(ip);
    std::cout << *optr << std::endl;
}
delete ip;
```

## triplet

A simple extension of `std::pair`. A special case of `std::tuple`.

### synopsis

```cpp
template<typename T1, typename T2, typename T3>
struct triplet
{
    typedef T1 first_type;
    typedef T2 second_type;
    typedef T3 third_type;

    first_type  first;
    second_type second;
    third_type  third;
};

template<typename T1, typename T2, typename T3>
void swap(triplet<T1, T2, T3>& lhs, triplet<T1, T2, T3>& rhs);

template<typename T1, typename T2, typename T3>
constexpr /* triplet of T1, T2, T3 ... stripped and decayed */
make_triplet(T1&& arg1, T2&& arg2, T3&& arg3);

template<typename T1, typename T2, typename T3>
constexpr /* tuple of T1, T2, T3 ... stripped and decayed */
make_tuple(T1&& arg1, T2&& arg2, T3&& arg3);

template<std::size_t I, typename T1, typename T2, typename T3>
constexpr TI& get(triplet<T1, T2, T3>&);
template<std::size_t I, typename T1, typename T2, typename T3>
constexpr TI&& get(triplet<T1, T2, T3>&&);
template<std::size_t I, typename T1, typename T2, typename T3>
constexpr TI const& get(triplet<T1, T2, T3> const&);

template<typename T1, typename T2, typename T3>
constexpr bool
operator==(const triplet<T1, T2, T3>& lhs, const triplet<T1, T2, T3>& rhs);

template<typename T1, typename T2, typename T3>
constexpr bool
operator!=(const triplet<T1, T2, T3>& lhs, const triplet<T1, T2, T3>& rhs);

template<typename T1, typename T2, typename T3>
constexpr bool
operator<(const triplet<T1, T2, T3>& lhs, const triplet<T1, T2, T3>& rhs);

template<typename T1, typename T2, typename T3>
constexpr bool
operator>(const triplet<T1, T2, T3>& lhs, const triplet<T1, T2, T3>& rhs);

template<typename T1, typename T2, typename T3>
constexpr bool
operator<=(const triplet<T1, T2, T3>& lhs, const triplet<T1, T2, T3>& rhs);

template<typename T1, typename T2, typename T3>
constexpr bool
operator>=(const triplet<T1, T2, T3>& lhs, const triplet<T1, T2, T3>& rhs);
```

### example

Almost same as `std::pair`.

```cpp
#include<pseudo/triplet.hpp>
#include<iostream>

auto tri = psd::make_triplet(true, 42, 3.14);
std::cout << tri.first << ", " << tri.second << ", " << tri.third << std::endl;

psd::triplet<bool, int, double> from_tuple = std::make_tuple(true, 42, 3.14);

psd::triplet<std::vector<bool>, std::vector<int>, std::vector<double>>
    triple(std::piecewise_construct, std::make_tuple(2, true),
           std::make_tuple(5, 42), std::make_tuple(10, 3.14));
```
