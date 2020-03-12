#include "kae/resource_manager.h"

namespace kae {

void ResourceManager::cleanup()
{
    for (auto it = store_.begin(); it != store_.end();)
        if (it->second.expired())
            it = store_.erase(it);
        else
            ++it;
}

}  // namespace kae
