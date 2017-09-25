#pragma once

#include "ompu/geo.hpp"
#include "ompu/ui_fwd.hpp"

#include <boost/functional/hash.hpp>

#include <memory>
#include <functional>
#include <type_traits>


namespace ompu { namespace ui {

struct ComponentBaseTag {};


template<class Derived, class Model>
class Component
    : ComponentBaseTag
    , public Model
    , public std::enable_shared_from_this<Component<Derived, Model>>
{
public:
    using base_type = Component<Derived, Model>;
    using self_type = Derived;
    using model_type = Model;

    Component() noexcept = default;

    explicit Component(model_type model) noexcept
        : model_type(std::move(model))
    {}

    ~Component() = default;

    model_type const& model() const noexcept { return *static_cast<model_type const*>(this); }

    void* id() const noexcept { return static_cast<void*>(this); }

    Component(Component const&) = delete;
    Component(Component&&) noexcept = default;

    Component& operator=(Component const&) = delete;
    Component& operator=(Component&&) noexcept = default;
};

template<class C, std::enable_if_t<std::is_base_of<ComponentBaseTag, C>::value, int> = 0>
inline bool operator==(C const& lhs, C const& rhs) noexcept
{
    return lhs.id() == rhs.id();
}

template<class C, std::enable_if_t<std::is_base_of<ComponentBaseTag, C>::value, int> = 0>
inline std::size_t hash_value(C const& v)
{
    return std::hash<void*>{}(v.id());
}

}} // ompu
