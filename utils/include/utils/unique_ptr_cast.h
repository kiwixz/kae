#pragma once

#include <memory>

namespace utils {

template <typename Derived, typename Base>
std::unique_ptr<Derived> static_unique_ptr_cast(std::unique_ptr<Base>&& ptr)
{
    return std::unique_ptr<Derived>{static_cast<Derived*>(ptr.release())};
}

/// Will not alter base pointer on conversion failure (returning empty unique_ptr).
template <typename Derived, typename Base>
std::unique_ptr<Derived> dynamic_unique_ptr_cast(std::unique_ptr<Base>&& ptr)
{
    Derived* derived = dynamic_cast<Derived*>(ptr.get());
    if (!derived)
        return {};
    ptr.release();
    return std::unique_ptr<Derived>{derived};
}

}  // namespace utils
