#include "utils/any_detail.h"

namespace utils::any_detail {

Impl::~Impl()
{
    destroy();
}

Impl::Impl(const Impl& other)
{
    *this = other;
}

Impl& Impl::operator=(const Impl& other)
{
    if (this != &other) {
        destroy();
        if (other) {
            proxy_ = other.proxy_;
            proxy_(any_detail::ProxyOp::copy, const_cast<any_detail::Storage*>(&other.storage_), &storage_);  // will not modify other.storage_
        }
    }
    return *this;
}

Impl::Impl(Impl&& other) noexcept
{
    *this = std::move(other);
}

Impl& Impl::operator=(Impl&& other) noexcept
{
    if (this != &other) {
        destroy();
        if (other) {
            proxy_ = other.proxy_;
            proxy_(any_detail::ProxyOp::move, &other.storage_, &storage_);
        }
    }
    return *this;
}

Impl::operator bool() const
{
    return storage_.operator bool();
}

void Impl::destroy()
{
    if (storage_)
        proxy_(any_detail::ProxyOp::destroy, &storage_, nullptr);
}

}  // namespace utils::any_detail
