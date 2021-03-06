#pragma once

#include <exception>
#include <future>

#include "kae/function.h"

namespace kae {

template <template <typename> typename, typename>
struct TaskImpl;

template <template <typename> typename Function, typename Result, typename... Args>
struct TaskImpl<Function, Result(Args...)> {
    TaskImpl() = default;

    TaskImpl(Function<Result(Args...)> function)
    {
        std::promise<Result> promise;
        future_ = promise.get_future();
        function_ = [promise = std::move(promise), function = std::move(function)](Args&&... args) mutable {
            try {
                if constexpr (std::is_same_v<Result, void>) {
                    function(std::forward<Args>(args)...);
                    promise.set_value();
                }
                else {
                    promise.set_value(function(std::forward<Args>(args)...));
                }
            }
            catch (...) {
                promise.set_exception(std::current_exception());
            }
        };
    }

    explicit operator bool() const
    {
        return function_;
    }

    std::future<Result>& future()
    {
        return future_;
    }

    void operator()(Args&&... args) const
    {
        function_(std::forward<Args>(args)...);
    }

private:
    std::future<Result> future_;
    Function<void(Args...)> function_;
};


template <typename... Args>
using Task = TaskImpl<Function, Args...>;

template <typename... Args>
using UniqueTask = TaskImpl<UniqueFunction, Args...>;

}  // namespace kae
