#pragma once

#include <array>
#include <type_traits>

#include "kae/pp.h"
#include "kae/uninitialized.h"

namespace kae {

template <typename TElement>
struct CircularIterator {
    using Element = TElement;

    using Native = std::conditional_t<std::is_const_v<Element>,
                                      const Uninitialized<std::remove_const_t<Element>>,
                                      Uninitialized<Element>>;

    CircularIterator() = default;

    CircularIterator(Native* element,
                     Native* begin,
                     Native* end) :
        element_{element},
        begin_{begin}, end_{end}
    {}

    template <typename T, typename std::enable_if_t<std::is_same_v<const T, Element>, int> = 0>
    CircularIterator(const CircularIterator<T>& other) :
        CircularIterator{other.element_, other.begin_, other.end_}
    {}

    Element& operator*() const
    {
        return **element_;
    }
    Element* operator->() const
    {
        return element_;
    }

    CircularIterator<Element>& operator++()
    {
        element_ = (element_ == end_ - 1 ? begin_ : element_ + 1);
        return *this;
    }
    CircularIterator<Element> operator++(int)
    {
        CircularIterator<Element> r = *this;
        ++*this;
        return r;
    }

    CircularIterator<Element>& operator--()
    {
        element_ = (element_ == 0 ? end_ - 1 : element_ - 1);
        return *this;
    }
    CircularIterator<Element> operator--(int)
    {
        CircularIterator<Element> r = *this;
        --*this;
        return r;
    }

#define DEF_OP(op)                                           \
    bool operator op(const CircularIterator<Element>& other) \
    {                                                        \
        return element_ op other.element_;                   \
    }

    DEF_OP(==)
    DEF_OP(!=)
    DEF_OP(<)
    DEF_OP(>)
    DEF_OP(<=)
    DEF_OP(>=)
#undef DEF_OP

private:
    Native* element_ = nullptr;
    Native* begin_;
    Native* end_;
};


template <typename TElement, size_t Tsize>
struct Circular {
    using Element = TElement;

    using ConstIterator = CircularIterator<const Element>;
    using Iterator = CircularIterator<Element>;

    Circular() = default;

    ~Circular()
    {
        clear();
    }

    Circular(const Circular& other)
    {
        *this = other;
    }

    Circular& operator=(const Circular& other)
    {
        if (this == &other)
            return *this;

        clear();
        for (const Element& element : other)
            emplace_back(element);
        return *this;
    }

    Circular(Circular&& other) noexcept
    {
        *this = std::move(other);
    }

    Circular& operator=(Circular&& other) noexcept
    {
        if (this == &other)
            return *this;

        clear();
        for (Element& element : other)
            emplace_back(std::move(element));
        return *this;
    }

    Iterator begin()
    {
        return {storage_.data() + begin_, storage_.data(), storage_.data() + storage_.size()};
    }
    ConstIterator begin() const
    {
        return {storage_.data() + begin_, storage_.data(), storage_.data() + storage_.size()};
    }

    Iterator end()
    {
        return {storage_.data() + end_, storage_.data(), storage_.data() + storage_.size()};
    }
    ConstIterator end() const
    {
        return {storage_.data() + end_, storage_.data(), storage_.data() + storage_.size()};
    }

    bool empty() const
    {
        return begin_ == end_;
    }

    size_t size() const
    {
        return begin_ <= end_ ? end_ - begin_ : (storage_.size() - begin_) + end_;
    }
    size_t max_size() const
    {
        return Tsize;
    }

    void clear()
    {
        while (!empty())
            pop_front();
    }

    Element& front()
    {
        ASSERT(size() > 0);
        return *storage_[begin_];
    }
    const Element& front() const
    {
        ASSERT(size() > 0);
        return *storage_[begin_];
    }

    Element& back()
    {
        ASSERT(size() > 0);
        return *storage_[decrement(end_)];
    }
    const Element& back() const
    {
        ASSERT(size() > 0);
        return *storage_[decrement(end_)];
    }

    template <typename... Args>
    void emplace_back(Args&&... args)
    {
        if (size() == max_size())
            pop_front();

        storage_[end_].emplace(std::forward<Args>(args)...);
        end_ = increment(end_);
    }

    template <typename... Args>
    void emplace_front(Args&&... args)
    {
        if (size() == max_size())
            pop_back();

        begin_ = decrement(begin_);
        storage_[begin_].emplace(std::forward<Args>(args)...);
    }

    void pop_back()
    {
        ASSERT(size() > 0);
        end_ = decrement(end_);
        storage_[end_].destroy();
    }

    void pop_front()
    {
        ASSERT(size() > 0);
        storage_[begin_].destroy();
        begin_ = increment(begin_);
    }

    void push_back(Element element)
    {
        return emplace_back(std::move(element));
    }

    void push_front(Element element)
    {
        return emplace_front(std::move(element));
    }

private:
    // we need Tsize+1 to ensure that begin_!=end_ when full
    std::array<Uninitialized<Element>, Tsize + 1> storage_;
    size_t begin_ = 0;
    size_t end_ = 0;

    size_t increment(size_t index) const
    {
        return index == storage_.size() - 1 ? 0 : index + 1;
    }

    size_t decrement(size_t index) const
    {
        return index == 0 ? storage_.size() - 1 : index - 1;
    }
};

}  // namespace kae
