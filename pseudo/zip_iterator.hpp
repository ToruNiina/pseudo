#ifndef PSEUDO_ZIP_ITERATOR
#define PSEUDO_ZIP_ITERATOR
#include "type_traits.hpp"
#include <iterator>
#include <tuple>

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

template<typename ... Tuples>
struct tuple_concatenated_type
{
    typedef decltype(std::tuple_cat(std::declval<Tuples>()...)) type;
};

template<std::size_t I, std::size_t SZ, typename T,
         template<typename> class Tf>
struct tuple_transformed_tails_type
{
    typedef typename tuple_concatenated_type<
        std::tuple<typename Tf<typename std::tuple_element<I, T>::type>::type>,
        typename tuple_transformed_tails_type<I+1, SZ, T, Tf>::type
        >::type type;
};

template<std::size_t I, typename T, template<typename> class Tf>
struct tuple_transformed_tails_type<I, I, T, Tf>
{
    typedef std::tuple<> type;
};

template<std::size_t I, template<typename> class Tf>
struct tuple_transformed_tails_type<I, 0, std::tuple<>, Tf>
{
    typedef std::tuple<> type;
};

}// detail


template<typename ... Ts>
class zip_iterator
{
  public:

    template<typename T>
    using value_t_of   = typename std::iterator_traits<T>::value_type;
    template<typename T>
    struct reference_of
    {typedef typename std::iterator_traits<T>::value_type& type;};
    template<typename T>
    struct pointer_of
    {typedef typename std::iterator_traits<T>::value_type* type;};

    template<typename T>
    using category_of = typename std::iterator_traits<T>::iterator_category;

    typedef zip_iterator<Ts...> self_type;
    typedef typename detail::common_iterator_tag_t<category_of<Ts> ...>
            iterator_category;
    typedef std::tuple<value_t_of<Ts>  ...> value_type;
    typedef std::tuple<value_t_of<Ts>& ...> reference;
    typedef std::tuple<value_t_of<Ts>* ...> pointer;
    typedef std::size_t                     difference_type;
    typedef std::tuple<Ts...>               container_type;

  public:

    zip_iterator()  = default;
    ~zip_iterator() = default;
    zip_iterator(zip_iterator && rhs)      = default;
    zip_iterator(zip_iterator const & rhs) = default;
    zip_iterator& operator=(zip_iterator && rhs)      = default;
    zip_iterator& operator=(zip_iterator const & rhs) = default;

    zip_iterator(Ts&& ... args) noexcept(psd::is_all<
            std::is_nothrow_move_constructible, Ts...>::value)
        : iters_(std::move(args)...){}
    zip_iterator(Ts const& ... args) noexcept(psd::is_all<
            std::is_nothrow_copy_constructible, Ts...>::value)
        : iters_(args...){}

    reference operator*() const noexcept;
    pointer  operator->() const noexcept;

    zip_iterator& operator++()    noexcept;
    zip_iterator  operator++(int) noexcept;
    zip_iterator& operator--()    noexcept;
    zip_iterator  operator--(int) noexcept;
    zip_iterator& operator+=(difference_type d) noexcept;
    zip_iterator& operator-=(difference_type d) noexcept;

    template<std::size_t i>
    typename std::iterator_traits<
        typename std::tuple_element<i, container_type>::type>::reference
    ref() const noexcept {return *(std::get<i>(iters_));}

    template<std::size_t i>
    typename std::iterator_traits<
        typename std::tuple_element<i, container_type>::type>::pointer
    ptr() const noexcept {return &(*std::get<i>(iters_));}

    container_type const& base() const noexcept {return iters_;}

  private:

    template<std::size_t I, std::size_t SZ>
    struct get_reference_recursively
    {
        static typename detail::tuple_transformed_tails_type<
            I, SZ, container_type, reference_of>::type
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
        static typename detail::tuple_transformed_tails_type<
            I, SZ, container_type, pointer_of>::type
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

template<typename ... Ts>
inline zip_iterator<Ts...> make_zip(Ts&& ... args)
{
    return zip_iterator<Ts...>(std::forward<Ts>(args)...);
}

}
#endif// PSEUDO_ZIP_ITERATOR
