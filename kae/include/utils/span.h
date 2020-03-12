#pragma once

#include <cstddef>
#include <type_traits>

namespace kae {

template <typename TElement>
struct Span {
    using Element = TElement;

    constexpr Span() = default;
    constexpr Span(Element* data, size_t size);

    template <typename T, std::enable_if_t<!std::is_same_v<std::decay_t<T>, Span<Element>> && !std::is_const_v<Element>, int> = 0>
    constexpr Span(T& container);

    template <typename T, std::enable_if_t<!std::is_same_v<std::decay_t<T>, Span<Element>> && std::is_const_v<Element>, int> = 0>
    constexpr Span(const T& container);

    template <typename T, std::enable_if_t<!std::is_same_v<std::decay_t<T>, Span<Element>>, int> = 0>
    constexpr Span(T&& container);

    constexpr Element& operator[](size_t idx) const;

    constexpr Element* begin() const;
    constexpr Element* data() const;
    constexpr Element* end() const;
    constexpr size_t size() const;
    constexpr size_t size_bytes() const;
    constexpr Span<Element> subspan(size_t offset, size_t size) const;
    constexpr Span<const std::byte> as_bytes() const;

private:
    Element* data_ = nullptr;
    size_t size_ = 0;
};


template <typename TElement>
template <typename T, std::enable_if_t<!std::is_same_v<std::decay_t<T>, Span<TElement>> && !std::is_const_v<TElement>, int>>
constexpr Span<TElement>::Span(T& container) :
    Span{container.data(), container.size()}
{}

template <typename TElement>
template <typename T, std::enable_if_t<!std::is_same_v<std::decay_t<T>, Span<TElement>> && std::is_const_v<TElement>, int>>
constexpr Span<TElement>::Span(const T& container) :
    Span{container.data(), container.size()}
{}

template <typename TElement>
template <typename T, std::enable_if_t<!std::is_same_v<std::decay_t<T>, Span<TElement>>, int>>
constexpr Span<TElement>::Span(T&& container) :
    Span{container.data(), container.size()}
{}

template <typename TElement>
constexpr Span<TElement>::Span(Element* data, size_t size) :
    data_{data},
    size_{size}
{}

template <typename TElement>
constexpr TElement& Span<TElement>::operator[](size_t idx) const
{
    return data_[idx];
}

template <typename TElement>
constexpr TElement* Span<TElement>::begin() const
{
    return data_;
}

template <typename TElement>
constexpr TElement* Span<TElement>::data() const
{
    return data_;
}

template <typename TElement>
constexpr TElement* Span<TElement>::end() const
{
    return data_ + size_;
}

template <typename TElement>
constexpr size_t Span<TElement>::size() const
{
    return size_;
}

template <typename TElement>
constexpr size_t Span<TElement>::size_bytes() const
{
    return size_ * sizeof(Element);
}

template <typename TElement>
constexpr Span<TElement> Span<TElement>::subspan(size_t offset, size_t size) const
{
    return {begin() + offset, size};
}

template <typename TElement>
constexpr Span<const std::byte> Span<TElement>::as_bytes() const
{
    return {reinterpret_cast<const std::byte*>(data_), size_bytes()};
}

}  // namespace kae
