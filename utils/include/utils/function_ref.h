#pragma once

#include <functional>
#include <type_traits>

namespace utils {

template <typename>
struct FunctionRef;


template <typename Result, typename... Args>
struct FunctionRef<Result(Args...)> {
    FunctionRef() = default;

    template <typename T, std::enable_if_t<!std::is_same_v<std::decay_t<T>, FunctionRef<Result(Args...)>> && !std::is_pointer_v<T>, int> = 0>
    FunctionRef(T&& function) :
        native_{reinterpret_cast<void*>(&function)}
    {
        invoker_ = +[](void* native, Args... args) -> Result {
            return std::invoke(*reinterpret_cast<std::add_pointer_t<T>>(native),
                               std::forward<Args>(args)...);
        };
    }

    template <typename T>
    FunctionRef(T* function) :
        native_{reinterpret_cast<void*>(function)}
    {
        invoker_ = +[](void* native, Args... args) -> Result {
            return std::invoke(reinterpret_cast<T*>(native),
                               std::forward<Args>(args)...);
        };
    }

    ~FunctionRef() = default;
    FunctionRef(const FunctionRef&) = default;
    FunctionRef& operator=(const FunctionRef&) = default;
    FunctionRef(FunctionRef&&) noexcept = default;
    FunctionRef& operator=(FunctionRef&&) noexcept = default;

    explicit operator bool() const
    {
        return native_;
    }

    Result operator()(Args... args) const
    {
        return invoker_(native_, std::forward<Args>(args)...);
    }

private:
    void* native_ = nullptr;
    Result (*invoker_)(void* native, Args...);
};

}  // namespace utils
