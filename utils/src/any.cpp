#include "utils/any.h"

namespace utils {

Any::operator bool() const
{
    return static_cast<bool>(impl_);
}


UniqueAny::operator bool() const
{
    return static_cast<bool>(impl_);
}

}  // namespace utils
