#pragma once

#include <type_traits>

#include "utils/any.h"

namespace utils {

template <typename, typename>
struct FunctionImpl;

template <typename Storage, typename Result, typename... Args>
struct FunctionImpl<Storage, Result(Args...)> {
    FunctionImpl() = default;

    template <typename T, std::enable_if_t<!std::is_same_v<std::decay_t<T>, FunctionImpl>, int> = 0>
    FunctionImpl(T&& function) :
        storage_{std::forward<T>(function)}
    {
        invoker_ = +[](const Storage& storage, Args... args) -> Result {
            return std::invoke(storage.template as<std::decay_t<T>>(),
                               std::forward<Args>(args)...);
        };
    }

    explicit operator bool() const
    {
        return static_cast<bool>(storage_);
    }

    Result operator()(Args... args) const
    {
        return invoker_(storage_, std::forward<Args>(args)...);
    }

private:
    Storage storage_;
    Result (*invoker_)(const Storage& storage, Args...);
};


template <typename... Args>
using Function = FunctionImpl<Any, Args...>;

template <typename... Args>
using UniqueFunction = FunctionImpl<UniqueAny, Args...>;

}  // namespace utils
