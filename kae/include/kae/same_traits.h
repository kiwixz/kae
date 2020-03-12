#pragma once

#include <type_traits>

namespace kae {

template <typename T, bool = std::is_copy_constructible_v<T>>
struct SameTraitsCopyConstructible {
    SameTraitsCopyConstructible() = default;
    ~SameTraitsCopyConstructible() = default;
    SameTraitsCopyConstructible(const SameTraitsCopyConstructible&) = default;
    SameTraitsCopyConstructible& operator=(const SameTraitsCopyConstructible&) = default;
    SameTraitsCopyConstructible(SameTraitsCopyConstructible&&) noexcept = default;
    SameTraitsCopyConstructible& operator=(SameTraitsCopyConstructible&&) noexcept = default;
};

template <typename T>
struct SameTraitsCopyConstructible<T, false> {
    SameTraitsCopyConstructible() = default;
    ~SameTraitsCopyConstructible() = default;
    SameTraitsCopyConstructible(const SameTraitsCopyConstructible&) = delete;
    SameTraitsCopyConstructible& operator=(const SameTraitsCopyConstructible&) = default;
    SameTraitsCopyConstructible(SameTraitsCopyConstructible&&) noexcept = default;
    SameTraitsCopyConstructible& operator=(SameTraitsCopyConstructible&&) noexcept = default;
};


template <typename T, bool = std::is_copy_assignable_v<T>>
struct SameTraitsCopyAssignable {
    SameTraitsCopyAssignable() = default;
    ~SameTraitsCopyAssignable() = default;
    SameTraitsCopyAssignable(const SameTraitsCopyAssignable&) = default;
    SameTraitsCopyAssignable& operator=(const SameTraitsCopyAssignable&) = default;
    SameTraitsCopyAssignable(SameTraitsCopyAssignable&&) noexcept = default;
    SameTraitsCopyAssignable& operator=(SameTraitsCopyAssignable&&) noexcept = default;
};

template <typename T>
struct SameTraitsCopyAssignable<T, false> {
    SameTraitsCopyAssignable() = default;
    ~SameTraitsCopyAssignable() = default;
    SameTraitsCopyAssignable(const SameTraitsCopyAssignable&) = default;
    SameTraitsCopyAssignable& operator=(const SameTraitsCopyAssignable&) = delete;
    SameTraitsCopyAssignable(SameTraitsCopyAssignable&&) noexcept = default;
    SameTraitsCopyAssignable& operator=(SameTraitsCopyAssignable&&) noexcept = default;
};


template <typename T, bool = std::is_move_constructible_v<T>>
struct SameTraitsMoveConstructible {
    SameTraitsMoveConstructible() = default;
    ~SameTraitsMoveConstructible() = default;
    SameTraitsMoveConstructible(const SameTraitsMoveConstructible&) = default;
    SameTraitsMoveConstructible& operator=(const SameTraitsMoveConstructible&) = default;
    SameTraitsMoveConstructible(SameTraitsMoveConstructible&&) noexcept = default;
    SameTraitsMoveConstructible& operator=(SameTraitsMoveConstructible&&) noexcept = default;
};

template <typename T>
struct SameTraitsMoveConstructible<T, false> {
    SameTraitsMoveConstructible() = default;
    ~SameTraitsMoveConstructible() = default;
    SameTraitsMoveConstructible(const SameTraitsMoveConstructible&) = default;
    SameTraitsMoveConstructible& operator=(const SameTraitsMoveConstructible&) = default;
    SameTraitsMoveConstructible(SameTraitsMoveConstructible&&) noexcept = delete;
    SameTraitsMoveConstructible& operator=(SameTraitsMoveConstructible&&) noexcept = default;
};


template <typename T, bool = std::is_move_assignable_v<T>>
struct SameTraitsMoveAssignable {
    SameTraitsMoveAssignable() = default;
    ~SameTraitsMoveAssignable() = default;
    SameTraitsMoveAssignable(const SameTraitsMoveAssignable&) = default;
    SameTraitsMoveAssignable& operator=(const SameTraitsMoveAssignable&) = default;
    SameTraitsMoveAssignable(SameTraitsMoveAssignable&&) noexcept = default;
    SameTraitsMoveAssignable& operator=(SameTraitsMoveAssignable&&) noexcept = default;
};

template <typename T>
struct SameTraitsMoveAssignable<T, false> {
    SameTraitsMoveAssignable() = default;
    ~SameTraitsMoveAssignable() = default;
    SameTraitsMoveAssignable(const SameTraitsMoveAssignable&) = default;
    SameTraitsMoveAssignable& operator=(const SameTraitsMoveAssignable&) = default;
    SameTraitsMoveAssignable(SameTraitsMoveAssignable&&) noexcept = default;
    SameTraitsMoveAssignable& operator=(SameTraitsMoveAssignable&&) noexcept = delete;
};


template <typename T>
struct SameTraits :
    SameTraitsCopyConstructible<T>,
    SameTraitsCopyAssignable<T>,
    SameTraitsMoveConstructible<T>,
    SameTraitsMoveAssignable<T> {};

}  // namespace kae
