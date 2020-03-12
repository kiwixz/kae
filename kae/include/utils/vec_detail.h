#pragma once

#include <algorithm>
#include <cmath>
#include <type_traits>

#include "kae/function_ref.h"

namespace kae::vec_detail {

struct VecBase {};  // simplify sfinae

template <typename T>
inline constexpr bool is_vec = std::is_base_of_v<VecBase, T>;


template <template <typename> typename TVec, typename TElement, int Tsize>
struct VecMeta : VecBase {
    template <typename T>
    using Vec = TVec<T>;

    using Element = TElement;
    static constexpr int size = Tsize;

    using Self = Vec<Element>;

    constexpr Element& operator[](size_t idx);
    constexpr Element operator[](size_t idx) const;

    template <typename T>
    constexpr Vec<T> transform(kae::FunctionRef<T(Element)> func);

    constexpr Self transform(kae::FunctionRef<Element(Element)> func);

    constexpr Element dot(const Self& other) const;
    constexpr Element length() const;

    constexpr void apply(kae::FunctionRef<Element(Element)> func);
    constexpr void normalize();

private:
    constexpr Self& self();
    constexpr const Self& self() const;
};


template <template <typename> typename TVec, typename TElement,
          std::enable_if_t<is_vec<TVec<TElement>>, int> = 0>
constexpr bool operator==(const TVec<TElement>& lhs, const TVec<TElement>& rhs);

template <template <typename> typename TVec, typename TElement,
          std::enable_if_t<is_vec<TVec<TElement>>, int> = 0>
constexpr bool operator!=(const TVec<TElement>& lhs, const TVec<TElement>& rhs);

template <template <typename> typename TVec, typename TElement,
          std::enable_if_t<is_vec<TVec<TElement>>, int> = 0>
constexpr TVec<TElement> operator-(const TVec<TElement>& lhs);

#define DEF_OP(op, type, operand)                                        \
    template <template <typename> typename TVec, typename TElement,      \
              std::enable_if_t<is_vec<TVec<TElement>>, int> = 0>         \
    constexpr TVec<TElement>& operator op(TVec<TElement>& lhs, type rhs) \
    {                                                                    \
        for (int i = 0; i < TVec<TElement>::size; ++i)                   \
            lhs[i] op operand;                                           \
        return lhs;                                                      \
    }

DEF_OP(+=, const TVec<TElement>&, rhs[i])
DEF_OP(-=, const TVec<TElement>&, rhs[i])
DEF_OP(*=, const TVec<TElement>&, rhs[i])
DEF_OP(/=, const TVec<TElement>&, rhs[i])
DEF_OP(+=, TElement, rhs)
DEF_OP(-=, TElement, rhs)
DEF_OP(*=, TElement, rhs)
DEF_OP(/=, TElement, rhs)
#undef DEF_OP

#define DEF_OP(op, type)                                                      \
    template <template <typename> typename TVec, typename TElement,           \
              std::enable_if_t<is_vec<TVec<TElement>>, int> = 0>              \
    constexpr TVec<TElement> operator op(const TVec<TElement>& lhs, type rhs) \
    {                                                                         \
        TVec<TElement> vec = lhs;                                             \
        vec op## = rhs;                                                       \
        return vec;                                                           \
    }

DEF_OP(+, const TVec<TElement>&)
DEF_OP(-, const TVec<TElement>&)
DEF_OP(*, const TVec<TElement>&)
DEF_OP(/, const TVec<TElement>&)
DEF_OP(+, TElement)
DEF_OP(-, TElement)
DEF_OP(*, TElement)
DEF_OP(/, TElement)
#undef DEF_OP

#define DEF_OP(op)                                                                \
    template <template <typename> typename TVec, typename TElement,               \
              std::enable_if_t<is_vec<TVec<TElement>>, int> = 0>                  \
    constexpr TVec<TElement> operator op(TElement lhs, const TVec<TElement>& rhs) \
    {                                                                             \
        TVec<TElement> vec;                                                       \
        for (int i = 0; i < TVec<TElement>::size; ++i)                            \
            vec[i] = lhs op rhs[i];                                               \
        return vec;                                                               \
    }

DEF_OP(+)
DEF_OP(-)
DEF_OP(*)
DEF_OP(/)
#undef DEF_OP


template <template <typename> typename TVec, typename TElement,
          std::enable_if_t<is_vec<TVec<TElement>>, int>>
constexpr bool operator==(const TVec<TElement>& lhs, const TVec<TElement>& rhs)
{
    for (int i = 0; i < TVec<TElement>::size; ++i)
        if (lhs[i] != rhs[i])
            return false;
    return true;
}

template <template <typename> typename TVec, typename TElement,
          std::enable_if_t<is_vec<TVec<TElement>>, int>>
constexpr bool operator!=(const TVec<TElement>& lhs, const TVec<TElement>& rhs)
{
    return !(lhs == rhs);
}

template <template <typename> typename TVec, typename TElement,
          std::enable_if_t<is_vec<TVec<TElement>>, int>>
constexpr TVec<TElement> operator-(const TVec<TElement>& lhs)
{
    TVec<TElement> vec;
    for (int i = 0; i < TVec<TElement>::size; ++i)
        vec[i] = -lhs[i];
    return vec;
}


template <template <typename> typename TVec, typename TElement, int Tsize>
constexpr TElement& VecMeta<TVec, TElement, Tsize>::operator[](size_t idx)
{
    return reinterpret_cast<Element*>(this)[idx];
}

template <template <typename> typename TVec, typename TElement, int Tsize>
constexpr TElement VecMeta<TVec, TElement, Tsize>::operator[](size_t idx) const
{
    return reinterpret_cast<const Element*>(this)[idx];
}

template <template <typename> typename TVec, typename TElement, int Tsize>
template <typename T>
constexpr TVec<T> VecMeta<TVec, TElement, Tsize>::transform(kae::FunctionRef<T(Element)> func)
{
    Vec<T> vec;
    for (int i = 0; i < size; ++i)
        vec[i] = func(self()[i]);
    return vec;
}

template <template <typename> typename TVec, typename TElement, int Tsize>
constexpr TVec<TElement> VecMeta<TVec, TElement, Tsize>::transform(kae::FunctionRef<Element(Element)> func)
{
    return transform<Element>(func);
}

template <template <typename> typename TVec, typename TElement, int Tsize>
constexpr TElement VecMeta<TVec, TElement, Tsize>::dot(const Self& other) const
{
    Element r{};
    for (int i = 0; i < size; ++i)
        r += self()[i] * other[i];
    return r;
}

template <template <typename> typename TVec, typename TElement, int Tsize>
constexpr TElement VecMeta<TVec, TElement, Tsize>::length() const
{
    return std::sqrt(dot(self()));
}

template <template <typename> typename TVec, typename TElement, int Tsize>
constexpr void VecMeta<TVec, TElement, Tsize>::apply(kae::FunctionRef<Element(Element)> func)
{
    for (int i = 0; i < size; ++i)
        self()[i] = func(self()[i]);
}

template <template <typename> typename TVec, typename TElement, int Tsize>
constexpr void VecMeta<TVec, TElement, Tsize>::normalize()
{
    self() /= length();
}

template <template <typename> typename TVec, typename TElement, int Tsize>
constexpr TVec<TElement>& VecMeta<TVec, TElement, Tsize>::self()
{
    return *static_cast<Self*>(this);
}

template <template <typename> typename TVec, typename TElement, int Tsize>
constexpr const TVec<TElement>& VecMeta<TVec, TElement, Tsize>::self() const
{
    return *static_cast<const Self*>(this);
}

}  // namespace kae::vec_detail
