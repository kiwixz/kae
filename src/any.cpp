#include "kae/any.h"

namespace kae {

Any::operator bool() const
{
    return static_cast<bool>(impl_);
}


UniqueAny::operator bool() const
{
    return static_cast<bool>(impl_);
}

}  // namespace kae
