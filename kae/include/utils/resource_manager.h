#pragma once

#include <memory>
#include <string>
#include <unordered_map>

#include "kae/exception.h"

namespace kae {

template <typename T>
using ResourceHandle = std::shared_ptr<const T>;


struct ResourceManager {
    template <typename T>
    ResourceHandle<T> get(const std::string& id) const;

    template <typename T, typename F>
    ResourceHandle<T> get(const std::string& id, const F& make);

    template <typename T>
    void get_to(const std::string& id, ResourceHandle<T>& handle) const;

    template <typename T, typename F>
    void get_to(const std::string& id, ResourceHandle<T>& handle, const F& make);

    void cleanup();

private:
    std::unordered_map<std::string, std::weak_ptr<const void>> store_;
};


template <typename T>
ResourceHandle<T> ResourceManager::get(const std::string& id) const
{
    std::shared_ptr<const void> resource = store_.at(id).lock();
    if (!resource)
        throw MAKE_EXCEPTION("resource expired");
    return ResourceHandle<T>{std::reinterpret_pointer_cast<const T>(resource)};
}

template <typename T, typename F>
ResourceHandle<T> ResourceManager::get(const std::string& id, const F& make)
{
    std::pair ret = store_.try_emplace(id);
    auto it = ret.first;

    std::shared_ptr<const void> resource;
    if (ret.second) {  // inserted an empty weak_ptr
        resource = make();
        it->second = resource;
    }
    else
        resource = it->second.lock();

    return ResourceHandle<T>{std::reinterpret_pointer_cast<const T>(resource)};
}

template <typename T>
void ResourceManager::get_to(const std::string& id, ResourceHandle<T>& handle) const
{
    handle = get<T>(id);
}

template <typename T, typename F>
void ResourceManager::get_to(const std::string& id, ResourceHandle<T>& handle, const F& make)
{
    handle = get<T>(id, make);
}

}  // namespace kae
