#ifndef PSEUDO_ZIP_ITERATOR
#define PSEUDO_ZIP_ITERATOR
#include <pseudo/type_traits.hpp>
#include <pseudo/utility.hpp>
#include <pseudo/tuple.hpp>
#include <iterator>

namespace psd
{

namespace detail
{

template<typename...>
struct common_iterator_tag;

template<typename T>
struct common_iterator_tag<T>{typedef T type;};

template<typename T>
struct common_iterator_tag<T, T>{typedef T type;};

template<typename T1, typename T2>
struct common_iterator_tag<T1, T2>
{
    typedef typename std::conditional<std::is_base_of<T1, T2>::value, T1,
            typename std::conditional<std::is_base_of<T2, T1>::value, T2, void
            >::type>::type type;
};

template<typename T, typename ... Ts>
struct common_iterator_tag<T, Ts...>
{
    typedef typename common_iterator_tag<T,
            typename common_iterator_tag<Ts...>::type>::type
        type;
};

template<typename ... Ts>
using common_iterator_tag_t = typename common_iterator_tag<Ts...>::type;

template<typename ...Ts>
struct check_all_size_equal_impl;

template<typename T1, typename ... Ts>
struct check_all_size_equal_impl<T1, Ts...>
{
    static bool invoke(std::size_t sz, const T1& head, const Ts& ... args)
    {
        using psd::size;
        return sz == size(head) &&
               check_all_size_equal_impl<Ts...>::invoke(sz, args...);
    }
};

template<>
struct check_all_size_equal_impl<>
{
    static bool invoke(std::size_t){return true;}
};

template<typename ...Ts>
struct check_all_size_equal;

template<typename T, typename ...Ts>
struct check_all_size_equal<T, Ts...>
{
    static void invoke(const T& head, const Ts& ...args)
    {
        using psd::size;
        if(!check_all_size_equal_impl<Ts...>::invoke(size(head), args...))
            throw std::out_of_range("zipped containers have different size");
        return;
    }
};

template<>
struct check_all_size_equal<>
{
    static void invoke(){return;}
};

}// detail

template<typename ... Ts>
class zip_iterator
{
  public:

    static_assert(psd::is_all<psd::is_iterator, Ts...>::value,
                  "zip_iterator contains only iterators");

    template<typename T>
    struct value_of
    {
        typedef typename std::conditional<psd::is_const_iterator<T>::value,
                    typename std::add_const<
                        typename std::iterator_traits<T>::value_type>::type,
                    typename std::iterator_traits<T>::value_type>::type type;
    };
    template<typename T>
    struct reference_of
    {
        typedef typename std::iterator_traits<T>::reference ref;
        typedef typename std::remove_reference<ref>::type   val;

        typedef typename std::conditional<psd::is_const_iterator<T>::value,
                typename std::add_lvalue_reference<
                    typename std::add_const<val>::type
                >::type, ref>::type type;
    };
    template<typename T>
    struct pointer_of
    {
        typedef typename std::iterator_traits<T>::pointer ptr;
        typedef typename std::remove_pointer<ptr>::type   val;

        typedef typename std::conditional<psd::is_const_iterator<T>::value,
                typename std::add_pointer<typename std::add_const<val>::type
                >::type, ptr>::type type;
    };

    template<typename T>
    using category_of = typename std::iterator_traits<T>::iterator_category;

    typedef zip_iterator<Ts...> self_type;
    typedef typename detail::common_iterator_tag_t<category_of<Ts> ...>
            iterator_category;
    typedef std::tuple<typename value_of<Ts>::type  ...> value_type;
    typedef std::tuple<typename value_of<Ts>::type& ...> reference;
    typedef std::tuple<typename value_of<Ts>::type* ...> pointer;
    typedef std::size_t                     difference_type;
    typedef std::tuple<Ts...>               container_type;

  public:

    constexpr zip_iterator() = default;
    ~zip_iterator() = default;
    constexpr zip_iterator(zip_iterator && rhs)      = default;
    constexpr zip_iterator(zip_iterator const & rhs) = default;
    zip_iterator& operator=(zip_iterator && rhs)      = default;
    zip_iterator& operator=(zip_iterator const & rhs) = default;

    zip_iterator(Ts&& ... args)
        noexcept(psd::is_all<std::is_nothrow_move_constructible, Ts...>::value)
        : iters_(std::move(args)...){}
    zip_iterator(Ts const& ... args)
        noexcept(psd::is_all<std::is_nothrow_copy_constructible, Ts...>::value)
        : iters_(args...){}

    reference operator*() const noexcept;
    pointer  operator->() const noexcept;

    zip_iterator& operator++()    noexcept;
    zip_iterator  operator++(int) noexcept;
    zip_iterator& operator--()    noexcept;
    zip_iterator  operator--(int) noexcept;
    zip_iterator& operator+=(difference_type d) noexcept;
    zip_iterator& operator-=(difference_type d) noexcept;

    // in c++11, std::get is not constexpr
    template<std::size_t i>
    typename std::iterator_traits<
        typename std::tuple_element<i, container_type>::type>::reference
    ref() const noexcept {return *(std::get<i>(iters_));}

    template<std::size_t i>
    typename std::iterator_traits<
        typename std::tuple_element<i, container_type>::type>::pointer
    ptr() const noexcept {return &(*std::get<i>(iters_));}

    constexpr container_type const& base() const noexcept {return iters_;}

  private:

    template<std::size_t I, std::size_t SZ>
    struct get_reference_recursively
    {
        // in c++11, std::tuple_cat is not constexpr
        static
        tuple_transform_t<reference_of, tuple_take_back_t<I, container_type>>
        invoke(const self_type* c)
        {
            return std::tuple_cat(std::tuple<typename reference_of<
                    typename std::tuple_element<I, container_type>::type
                >::type>(c->ref<I>()),
                get_reference_recursively<I+1, SZ>::invoke(c));
        }
    };
    template<std::size_t I>
    struct get_reference_recursively<I, I>
    {
        static std::tuple<> invoke(const self_type* c){return std::tuple<>{};}
    };

    template<std::size_t I, std::size_t SZ>
    struct get_pointer_recursively
    {
        static
        tuple_transform_t<pointer_of, tuple_take_back_t<I, container_type>>
        invoke(const self_type* c)
        {
            return std::tuple_cat(std::tuple<typename pointer_of<
                    typename std::tuple_element<I, container_type>::type
                >::type>(c->ptr<I>()),
                get_pointer_recursively<I+1, SZ>::invoke(c));
        }
    };
    template<std::size_t I>
    struct get_pointer_recursively<I, I>
    {
        static std::tuple<> invoke(const self_type* c){return std::tuple<>{};}
    };

  private:

    container_type iters_;
};


template<typename ... Ts>
inline typename zip_iterator<Ts...>::reference
zip_iterator<Ts...>::operator*() const noexcept
{
    return get_reference_recursively<0, std::tuple_size<container_type>::value
        >::invoke(this);
}

template<typename ... Ts>
inline typename zip_iterator<Ts...>::pointer
zip_iterator<Ts...>::operator->() const noexcept
{
    return get_pointer_recursively<0, std::tuple_size<container_type>::value
        >::invoke(this);
}

namespace detail
{
template<std::size_t i, typename... Ts>
struct recursively_increment
{
    static std::tuple<Ts...>& invoke(std::tuple<Ts...>& is) noexcept
    {
        ++std::get<i-1>(is);
        return recursively_increment<i-1, Ts...>::invoke(is);
    }

    static std::tuple<Ts...>& invoke(std::tuple<Ts...>& is, std::size_t d,
                                     std::random_access_iterator_tag t) noexcept
    {
        std::get<i-1>(is) += d;
        return recursively_increment<i-1, Ts...>::invoke(is, d, t);
    }

    static std::tuple<Ts...>& invoke(std::tuple<Ts...>& is, std::size_t d,
                                     std::bidirectional_iterator_tag t) noexcept
    {
        std::size_t c = d;
        while(c-- != 0){++std::get<i-1>(is);}
        return recursively_increment<i-1, Ts...>::invoke(is, d, t);
    }

    static std::tuple<Ts...>& invoke(std::tuple<Ts...>& is, std::size_t d,
                                     std::forward_iterator_tag t) noexcept
    {
        std::size_t c = d;
        while(c-- != 0){++std::get<i-1>(is);}
        return recursively_increment<i-1, Ts...>::invoke(is, d, t);
    }
};
template<typename... Ts>
struct recursively_increment<0, Ts...>
{
    static std::tuple<Ts...>& invoke(std::tuple<Ts...>& is) noexcept
    {
        return is;
    }
    template<typename T>
    static std::tuple<Ts...>& invoke(std::tuple<Ts...>& is, std::size_t d, T t) noexcept
    {
        return is;
    }
};
template<std::size_t i, typename... Ts>
struct recursively_decrement
{
    static std::tuple<Ts...>& invoke(std::tuple<Ts...>& is) noexcept
    {
        --std::get<i-1>(is);
        return recursively_decrement<i-1, Ts...>::invoke(is);
    }
    static std::tuple<Ts...>& invoke(std::tuple<Ts...>& is, std::size_t d,
                                     std::random_access_iterator_tag t) noexcept
    {
        std::get<i-1>(is) -= d;
        return recursively_decrement<i-1, Ts...>::invoke(is, d, t);
    }
    static std::tuple<Ts...>& invoke(std::tuple<Ts...>& is, std::size_t d,
                                     std::bidirectional_iterator_tag t) noexcept
    {
        std::size_t c = d;
        while(c-- != 0){--std::get<i-1>(is);}
        return recursively_decrement<i-1, Ts...>::invoke(is, d, t);
    }
};
template<typename... Ts>
struct recursively_decrement<0, Ts...>
{
    static std::tuple<Ts...>& invoke(std::tuple<Ts...>& is) noexcept
    {
        return is;
    }
    static std::tuple<Ts...>& invoke(std::tuple<Ts...>& is, std::size_t d) noexcept
    {
        return is;
    }
    template<typename T>
    static std::tuple<Ts...>& invoke(std::tuple<Ts...>& is, std::size_t d, T t) noexcept
    {
        return is;
    }
};
}// detail

template<typename ...Ts>
zip_iterator<Ts...>& zip_iterator<Ts...>::operator++() noexcept
{
    detail::recursively_increment<sizeof...(Ts), Ts...>::invoke(iters_);
    return *this;
}

template<typename ...Ts>
zip_iterator<Ts...> zip_iterator<Ts...>::operator++(int) noexcept
{
    const auto tmp = *this;
    detail::recursively_increment<sizeof...(Ts), Ts...>::invoke(iters_);
    return tmp;
}

template<typename ...Ts>
zip_iterator<Ts...>& zip_iterator<Ts...>::operator--() noexcept
{
    detail::recursively_decrement<sizeof...(Ts), Ts...>::invoke(iters_);
    return *this;
}

template<typename ...Ts>
zip_iterator<Ts...> zip_iterator<Ts...>::operator--(int) noexcept
{
    const auto tmp = *this;
    detail::recursively_decrement<sizeof...(Ts), Ts...>::invoke(iters_);
    return tmp;
}

template<typename ...Ts>
zip_iterator<Ts...>& zip_iterator<Ts...>::operator+=(difference_type d) noexcept
{
    detail::recursively_increment<sizeof...(Ts), Ts...>::invoke(
            iters_, d, iterator_category{});
    return *this;
}

template<typename ...Ts>
zip_iterator<Ts...>& zip_iterator<Ts...>::operator-=(difference_type d) noexcept
{
    detail::recursively_decrement<sizeof...(Ts), Ts...>::invoke(
            iters_, d, iterator_category{});
    return *this;
}

template<std::size_t i, typename... Ts>
inline typename std::iterator_traits<
    typename std::tuple_element<i, std::tuple<Ts...>>::type>::pointer
get(zip_iterator<Ts...>& t) noexcept
{
    return t.template ptr<i>();
}

template<std::size_t i, typename... Ts>
inline typename std::iterator_traits<
    typename std::tuple_element<i, std::tuple<Ts...>>::type>::pointer
get(const zip_iterator<Ts...>& t) noexcept
{
    return t.template ptr<i>();
}

template<typename ...Ts1, typename ...Ts2>
inline bool
operator==(zip_iterator<Ts1...> const& lhs, zip_iterator<Ts2...> const& rhs)
{
    return lhs.base() == rhs.base();
}

template<typename ...Ts1, typename ...Ts2>
inline bool
operator!=(zip_iterator<Ts1...> const& lhs, zip_iterator<Ts2...> const& rhs)
{
    return lhs.base() != rhs.base();
}

template<typename ...Ts1, typename ...Ts2>
inline bool
operator<(zip_iterator<Ts1...> const& lhs, zip_iterator<Ts2...> const& rhs)
{
    return lhs.base() < rhs.base();
}

template<typename ...Ts1, typename ...Ts2>
inline bool
operator<=(zip_iterator<Ts1...> const& lhs, zip_iterator<Ts2...> const& rhs)
{
    return lhs.base() <= rhs.base();
}

template<typename ...Ts1, typename ...Ts2>
inline bool
operator>(zip_iterator<Ts1...> const& lhs, zip_iterator<Ts2...> const& rhs)
{
    return lhs.base() > rhs.base();
}

template<typename ...Ts1, typename ...Ts2>
inline bool
operator>=(zip_iterator<Ts1...> const& lhs, zip_iterator<Ts2...> const& rhs)
{
    return lhs.base() >= rhs.base();
}

//------------------------------ helper functions ------------------------------

template<typename ... Ts>
inline zip_iterator<Ts...> make_zip_iterator(Ts&& ... args)
    noexcept(noexcept(zip_iterator<Ts...>(std::forward<Ts>(args)...)))
{
    return zip_iterator<Ts...>(std::forward<Ts>(args)...);
}

namespace detail
{

template<typename ... Ts, std::size_t ...Is>
inline zip_iterator<typename psd::iterator_of<Ts>::iterator...>
make_zip_begin(const std::tuple<Ts& ...>& args,
               psd::detail::index_tuple<std::size_t, Is...>)
{
    using std::begin;
    return zip_iterator<typename psd::iterator_of<Ts>::iterator...>(
            begin(std::get<Is>(args))...);
}

template<typename ... Ts, std::size_t ...Is>
inline zip_iterator<typename psd::iterator_of<Ts>::iterator...>
make_zip_end(const std::tuple<Ts& ...>& args,
             psd::detail::index_tuple<std::size_t, Is...>)
{
    using std::end;
    return zip_iterator<typename psd::iterator_of<Ts>::iterator...>(
            end(std::get<Is>(args))...);
}

template<typename ... Ts, std::size_t ...Is>
inline zip_iterator<typename psd::iterator_of<Ts>::const_iterator...>
make_zip_cbegin(const std::tuple<Ts& ...>& args,
                psd::detail::index_tuple<std::size_t, Is...>)
{
    using std::begin;
    return zip_iterator<typename psd::iterator_of<Ts>::const_iterator...>(
            begin(psd::as_const(std::get<Is>(args)))...);
}

template<typename ... Ts, std::size_t ...Is>
inline zip_iterator<typename psd::iterator_of<Ts>::const_iterator...>
make_zip_cend(const std::tuple<Ts& ...>& args,
              psd::detail::index_tuple<std::size_t, Is...>)
{
    using std::end;
    return zip_iterator<typename psd::iterator_of<Ts>::const_iterator...>(
            end(psd::as_const(std::get<Is>(args)))...);
}

template<typename ... Ts>
class zippable
{
    template<typename T>
    struct is_iteratable
    {
        constexpr static bool value =
            psd::has_iterator<T>::value || std::is_array<T>::value;
    };

    static_assert(psd::is_all<is_iteratable, typename std::decay<Ts>::type...>::value,
                  "uniteratable type exists");

  public:

    zippable(Ts& ... args) : refs(args...)
    {
        detail::check_all_size_equal<Ts...>::invoke(args...);
    }
    ~zippable() = default;

    zip_iterator<typename psd::iterator_of<Ts>::iterator ...>
    begin() noexcept
    {
        return make_zip_begin(refs, psd::detail::make_index_tuple(refs));
    }
    zip_iterator<typename psd::iterator_of<Ts>::const_iterator ...>
    begin() const noexcept
    {
        return make_zip_cbegin(refs, psd::detail::make_index_tuple(refs));
    }
    zip_iterator<typename psd::iterator_of<Ts>::const_iterator ...>
    cbegin() const noexcept
    {
        return make_zip_cbegin(refs, psd::detail::make_index_tuple(refs));
    }

    zip_iterator<typename psd::iterator_of<Ts>::iterator ...>
    end() noexcept
    {
        return make_zip_end(refs, psd::detail::make_index_tuple(refs));
    }
    zip_iterator<typename psd::iterator_of<Ts>::const_iterator ...>
    end() const noexcept
    {
        return make_zip_cend(refs, psd::detail::make_index_tuple(refs));
    }
    zip_iterator<typename psd::iterator_of<Ts>::const_iterator ...>
    cend() const noexcept
    {
        return make_zip_cend(refs, psd::detail::make_index_tuple(refs));
    }

  private:

    std::tuple<Ts&...> refs;
};

} // detail

template<typename ... Ts>
inline detail::zippable<typename std::remove_reference<Ts>::type...>
make_zip(Ts&& ... args)
{
    return detail::zippable<typename std::remove_reference<Ts>::type...>(
            std::forward<Ts>(args)...);
}

}
#endif// PSEUDO_ZIP_ITERATOR
