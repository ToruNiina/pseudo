#ifndef PSEUDO_CIRCULAR_LIST
#define PSEUDO_CIRCULAR_LIST
#include <memory>
#include <iterator>
#include <cstdint>

namespace psd
{

namespace detail
{

template<typename T>
struct circular_list_node
{
    typedef T value_type;
    typedef value_type* pointer;
    typedef value_type& reference;
    typedef std::ptrdiff_t difference_type;
    typedef std::bidirectional_iterator_tag iterator_category;
    typedef circular_list_node* self_ptr;

    self_ptr prev_;
    self_ptr next_;
    value_type value_;
};

template<typename T>
struct circular_list_iterator
{
    typedef T value_type;
    typedef circular_list_node<value_type> node_type;
    typedef value_type* pointer;
    typedef value_type& reference;
    typedef std::ptrdiff_t difference_type;
    typedef std::bidirectional_iterator_tag iterator_category;
    typedef circular_list_iterator self_type;

    circular_list_iterator() noexcept : node_(nullptr){}
    ~circular_list_iterator() = default;
    circular_list_iterator(circular_list_iterator const&) = default;
    circular_list_iterator(circular_list_iterator&&)      = default;
    circular_list_iterator& operator=(circular_list_iterator const&) = default;
    circular_list_iterator& operator=(circular_list_iterator&&)      = default;

    explicit circular_list_iterator(node_type* ptr) noexcept : node_(ptr){}

    reference operator*()  const noexcept {return node_->value_;}
    pointer   operator->() const noexcept {return std::addressof(node_->value_);}

    self_type& operator++()    noexcept;
    self_type  operator++(int) noexcept;
    self_type& operator--()    noexcept;
    self_type  operator--(int) noexcept;

    node_type* node_;
};

template<typename T>
inline circular_list_iterator<T>&
circular_list_iterator<T>::operator++() noexcept
{
    node_ = node_->next_;
    return *this;
}

template<typename T>
inline circular_list_iterator<T>
circular_list_iterator<T>::operator++(int) noexcept
{
    const auto tmp = *this;
    ++(*this);
    return tmp;
}

template<typename T>
inline circular_list_iterator<T>&
circular_list_iterator<T>::operator--() noexcept
{
    node_ = node_->prev_;
    return *this;
}

template<typename T>
inline circular_list_iterator<T>
circular_list_iterator<T>::operator--(int) noexcept
{
    const auto tmp = *this;
    --(*this);
    return tmp;
}

template<typename T>
inline bool operator==(const circular_list_iterator<T>& lhs,
                       const circular_list_iterator<T>& rhs) noexcept
{
    return lhs.node_ == rhs.node_;
}

template<typename T>
inline bool operator!=(const circular_list_iterator<T>& lhs,
                       const circular_list_iterator<T>& rhs) noexcept
{
    return lhs.node_ != rhs.node_;
}

template<typename T>
inline bool operator==(const circular_list_iterator<T>& lhs,
                       const circular_list_iterator<const T>& rhs) noexcept
{
    return lhs.node_ == rhs.node_;
}

template<typename T>
inline bool operator!=(const circular_list_iterator<T>& lhs,
                       const circular_list_iterator<const T>& rhs) noexcept
{
    return lhs.node_ != rhs.node_;
}

template<typename T>
inline bool operator==(const circular_list_iterator<const T>& lhs,
                       const circular_list_iterator<T>& rhs) noexcept
{
    return lhs.node_ == rhs.node_;
}

template<typename T>
inline bool operator!=(const circular_list_iterator<const T>& lhs,
                       const circular_list_iterator<T>& rhs) noexcept
{
    return lhs.node_ != rhs.node_;
}

}// detail

template<typename T, typename Allocator = std::allocator<T>>
class circular_list
{
  public:
    typedef T value_type;
    typedef Allocator allocator_type;
    typedef value_type*       pointer;
    typedef value_type const* const_pointer;
    typedef value_type&       reference;
    typedef value_type const& const_reference;
    typedef detail::circular_list_iterator<value_type>       iterator;
    typedef detail::circular_list_const_iterator<value_type> const_iterator;
    typedef std::reverse_iterator<iterator>       reverse_iterator;
    typedef std::reverse_iterator<const_iterator> const_reverse_iterator;

    typedef circular_list_node<value_type> node_type;
    typedef allocator_type::template rebind<node_type>::other node_allocator_type;
    typedef std::size_t    size_type;
    typedef std::ptrdiff_t difference_type;

  public:
    circular_list()  noexcept;
    ~circular_list() noexcept;

    circular_list(const allocator_type& alloc);
    circular_list(size_type n, const value_type& v,
                  const allocator_type& a = allocator_type());
    explicit
    circular_list(size_type n, const allocator_type& a = allocator_type());
    template<typename InputIter>
    circular_list(InputIter first, InputIter last,
                  const allocator_type& a = allocator_type());
    circular_list(const circular_list& x);
    circular_list(circular_list&& x);
    circular_list(const circular_list& x, const allocator_type& a);
    circular_list(circular_list&& x, const allocator_type& a);
    circular_list(std::initializer_list<value_type> il, const allocator_type& a);

    circular_list& operator=(const circular_list& x);
    circular_list& operator=(circular_list&& x);
    circular_list& operator=(std::initializer_list x);

    template<typename InputIter>
    void assign(InputIter first, InputIter last);
    void assign(size_type sz, const value_type& x);
    void assign(std::initializer_list<value_type> init);

    bool      empty() const noexcept {return size_ == 0;}
    size_type size()  const noexcept {return size_;}
    size_type max_size() const noexcept {return alloc_.max_size();}

    void resize(size_type sz);
    void resize(size_type sz, const value_type& c);

    void push_front(const value_type& x);
    void push_front(value_type&& x);
    void push_back(const value_type& x);
    void push_back(value_type&& x);

    template<typename ... Args>
    void emplace_front(Args&& ... args);
    template<typename ... Args>
    void emplace_back(Args&& ... args);

    void pop_front();
    void pop_back();

    template<typename ... Args>
    iterator emplace(const_iterator position, Args&& ... args);

    iterator insert(iterator position, const value_type& x);
    iterator insert(const_iterator position, const value_type& x);
    iterator insert(const_iterator position, value_type&& x);
    void     insert(iterator       position, size_type n, const value_type& x);
    iterator insert(const_iterator position, size_type n, const value_type& x);
    iterator insert(const_iterator position, size_type n, value_type&& x);

    template<typename InputIter>
    void     insert(iterator       position, InputIter first, InputIter last);
    template<typename InputIter>
    iterator insert(const_iterator position, InputIter first, InputIter last);
    iterator insert(const_iterator position, std::initializer_list<value_type> il);

    iterator erase(iterator       position);
    iterator erase(const_iterator position);
    iterator erase(iterator       position, iterator       last);
    iterator erase(const_iterator position, const_iterator last);

    void swap(circular_list& x);
    void clear(){this->clear_(); this->init_();}

    void remove(const value_type& x);
    template<typename F>
    void remove_if(F pred);

    void unique();
    template<typename F>
    void unique(F pred);

    void merge(circular_list&  x);
    void merge(circular_list&& x);
    template<typename F>
    void merge(circular_list&  x, F comp);
    void merge(circular_list&& x, F comp);

    void sort();
    template<typename F>
    void sort(F comp);

    void reverse() noexcept;

    void splice(iterator position,       circular_list&  x);
    void splice(const_iterator position, circular_list&  x);
    void splice(const_iterator position, circular_list&& x);

    void splice(iterator       position, circular_list&  x, iterator i);
    void splice(const_iterator position, circular_list&  x, const_iterator i);
    void splice(const_iterator position, circular_list&& x, const_iterator i);

    void splice(iterator       position, circular_list&  x,
                iterator       first,    iterator        last);
    void splice(const_iterator position, circular_list&  x,
                const_iterator first,    const_iterator  last);
    void splice(const_iterator position, circular_list&& x,
                const_iterator first,    const_iterator  last);

    reference       front()       noexcept {return head_->value_;}
    cosnt_reference front() const noexcept {return head_->value_;}
    reference       back()        noexcept {return head_->prev->value_;}
    cosnt_reference back()  const noexcept {return head_->prev->value_;}

    iterator begin() noexcept {return iterator(head_);}
    iterator end()   noexcept {return iterator(head_);}
    const_iterator begin()  const noexcept {return const_iterator(head_);}
    const_iterator end()    const noexcept {return const_iterator(head_);}
    const_iterator cbegin() const noexcept {return const_iterator(head_);}
    const_iterator cend()   const noexcept {return const_iterator(head_);}

    reverse_iterator rbegin() noexcept {return reverse_iterator(begin());}
    reverse_iterator rend()   noexcept {return reverse_iterator(begin());}
    const_reverse_iterator rbegin()  const noexcept {return const_reverse_iterator(begin());}
    const_reverse_iterator rend()    const noexcept {return const_reverse_iterator(begin());}
    const_reverse_iterator crbegin() const noexcept {return const_reverse_iterator(begin());}
    const_reverse_iterator crend()   const noexcept {return const_reverse_iterator(begin());}

    allocator_type get_allocator() const noexcept {return allocator_type(n_alloc_);}

  private:

    void init_()  noexcept;
    void clear_() noexcept;
    node_ptr create_node_()           {return n_alloc_.allocate(1);}
    void     delete_node_(node_ptr p) {return n_alloc_.deallocate(p, 1);}

  private:

    size_type size_;
    node_ptr  head_;
    node_allocator_type n_alloc_;
};

} // psd

#include <pseudo/circular_list.tcc>
#endif// PSEUDO_BITS_CIRCULAR_LIST
