#pragma once // include this file only once per .cpp file

#include "../games/entity.h"
#include <memory>
#include <vector>
#include <typeindex>
#include <unordered_map>

/*
 * type erasure
 *  Every Pool<T> inherits from IPool, so every pool can be treated as an IPool*
 *  We hide the specific type behind this interface
 */
class IPool
{
public:
    virtual ~IPool() = default;
    virtual void remove(EntityId id) = 0;
};

template <typename T>
class Pool : public IPool
{
public:
    T& add(EntityId e, T c) { return m_data[e] = std::move(c); }
    T* get(EntityId e)
    {
        auto it = m_data.find(e);
        return it == m_data.end() ? nullptr : &it->second;
    }
    void remove(EntityId e) override { m_data.erase(e); }
    std::unordered_map<EntityId, T>& all() { return m_data; }
private:
    std::unordered_map<EntityId, T> m_data;
};

class Registry
{
public:
    EntityId create() { return allocateEntityId(); }

    // Deffered, same idea as destroy() + pruneInactive()
    void destroy(EntityId e) { m_toDestroy.push_back(e); }
    void flushDestroyed()
    {
        for (EntityId e : m_toDestroy)
        {
            for (auto& [type, pool] : m_pools)
            {
                pool->remove(e);
            }
        }
        m_toDestroy.clear();
    }

    template <typename T>
    T& add(EntityId e, T c = {})
    {
        return pool<T>().add(e, std::move(c));
    }

    template <typename T>
    T* get(EntityId e)
    {
        return pool<T>().get(e);
    }

    template <typename T>
    bool has(EntityId e)
    {
        return get<T>(e) != nullptr;
    }

    template <typename T>
    void remove(EntityId e)
    {
        pool<T>().remove(e);
    }

    // Run fn(entity, First&, Rest&...) for every entity that has ALL the listed components
    template <typename First, typename... Rest, typename Fn>
    void each(Fn&& fn)
    {
        for (auto& [e, first] : pool<First>().all())
        {
            if ((has<Rest>(e) && ...))
            {
                fn(e, first, *get<Rest>(e)...);
            }
        }
    }

private:
    template <typename T>
    Pool<T>& pool()
    {
        auto& p = m_pools[std::type_index(typeid(T))];
        if (!p) p = std::make_unique<Pool<T>>();
        return static_cast<Pool<T>&>(*p);
    }

    std::unordered_map<std::type_index, std::unique_ptr<IPool>> m_pools;
    std::vector<EntityId> m_toDestroy;
};
