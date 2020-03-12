#pragma once

#include <new>
#include <optional>
#include <type_traits>

#include "utils/pp.h"

namespace utils {

/// Allow custom lifetime management.  You have to manually destroy objects you create.
/// You probably should rather use std::optional in most cases (especially when size is not a constraint).
template <typename TElement>
struct Uninitialized {
    using Element = TElement;

    Uninitialized() = default;

    ~Uninitialized()
    {
        ASSERT(!init_);
    }

    Uninitialized(const Uninitialized&) = delete;
    Uninitialized& operator=(const Uninitialized&) = delete;
    Uninitialized(Uninitialized&&) noexcept = delete;
    Uninitialized& operator=(Uninitialized&&) noexcept = delete;

    const Element& operator*() const
    {
        ASSERT(init_);
        return *std::launder(reinterpret_cast<const Element*>(&storage_));
    }
    Element& operator*()
    {
        ASSERT(init_);
        return *std::launder(reinterpret_cast<Element*>(&storage_));
    }

    const Element* operator->() const
    {
        ASSERT(init_);
        return std::launder(reinterpret_cast<const Element*>(&storage_));
    }
    Element* operator->()
    {
        ASSERT(init_);
        return std::launder(reinterpret_cast<Element*>(&storage_));
    }

    template <typename... Args>
    void emplace(Args&&... args)
    {
        ASSERT(!init_);
        new (&storage_) Element{std::forward<Args>(args)...};
#ifdef DEBUG
        ASSERT(!init_);
        init_ = true;
#endif
    }

    void destroy() noexcept
    {
        ASSERT(init_);
        (**this).~Element();
#ifdef DEBUG
        init_ = false;
#endif
    }

private:
    std::aligned_storage_t<sizeof(Element), alignof(Element)> storage_;

#ifdef DEBUG
    bool init_ = false;
#endif
};

}  // namespace utils
