#pragma once

#include <memory>

#include "kae/function_ref.h"
#include "kae/static_function.h"

namespace kae {

template <typename T, void (*Tdeleter)(std::remove_all_extents_t<T>*)>
using CPtr = std::unique_ptr<T, StaticFunction<Tdeleter>>;


template <typename T, void (*Tdeleter)(std::remove_all_extents_t<T>*)>
CPtr<T, Tdeleter> make_c_ptr(std::remove_all_extents_t<T>* native);

template <typename T, void (*Tdeleter)(std::remove_all_extents_t<T>*)>
CPtr<T, Tdeleter> make_c_ptr(FunctionRef<void(std::remove_all_extents_t<T>*&)> setter);

template <typename T, typename... Args>
bool set_c_ptr(T& ptr, Args&&... args);


template <typename T, void (*Tdeleter)(std::remove_all_extents_t<T>*)>
CPtr<T, Tdeleter> make_c_ptr(std::remove_all_extents_t<T>* native)
{
    return CPtr<T, Tdeleter>{native};
}

template <typename T, void (*Tdeleter)(std::remove_all_extents_t<T>*)>
CPtr<T, Tdeleter> make_c_ptr(FunctionRef<void(std::remove_all_extents_t<T>*&)> setter)
{
    std::remove_all_extents_t<T>* native = nullptr;
    setter(native);
    return make_c_ptr<T, Tdeleter>(native);
}

template <typename T, typename... Args>
bool set_c_ptr(T& ptr, Args&&... args)
{
    ptr = make_c_ptr<typename T::element_type, T::deleter_type::function>(std::forward<Args>(args)...);
    return static_cast<bool>(ptr);
}

}  // namespace kae
