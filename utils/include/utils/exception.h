#pragma once

#include <memory>
#include <stdexcept>
#include <string>
#include <string_view>

#include <fmt/format.h>

#include "utils/pp.h"


#define MAKE_EXCEPTION(...) ::utils::Exception(SOURCE_LOCATION, __VA_ARGS__)
#define NOT_IMPLEMENTED throw MAKE_EXCEPTION("not implemented");


namespace utils {

struct Exception : std::runtime_error {
    Exception(std::string&& from, std::string_view what);

    template <typename... Args>
    Exception(std::string&& from, std::string_view format, Args&&... args);

    const std::string& from() const;

private:
    std::shared_ptr<const std::string> from_;
};


[[noreturn]] void terminate() noexcept;


template <typename... Args>
Exception::Exception(std::string&& from, std::string_view format, Args&&... args) :
    Exception{std::move(from), fmt::format(format, std::forward<Args>(args)...)}
{}

}  // namespace utils
