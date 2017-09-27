#pragma once

#include "ompu/geo.hpp"
#include "ompu/ui_fwd.hpp"

#include <boost/functional/hash.hpp>

#include <memory>
#include <functional>
#include <type_traits>


namespace ompu { namespace ui {

namespace tags {

struct Component {};

} // tags

template<class Model>
class Component
    : tags::Component
    , public std::enable_shared_from_this<Component<Model>>
{
public:
    using base_type = Component<Model>;
    using model_type = Model;
    using id_type = void const*;

    Component() noexcept = default;
    ~Component() = default;

    model_type const& model() const noexcept { return model_; }
    void set_model(model_type m) noexcept { model_ = std::move(m); }

    id_type id() const noexcept { return static_cast<id_type>(this); }

    Component(Component const&) = delete;
    Component(Component&&) noexcept = default;

    Component& operator=(Component const&) = delete;
    Component& operator=(Component&&) noexcept = default;

protected:
    model_type model_;
};

template<class C, std::enable_if_t<std::is_base_of<tags::Component, C>::value, int> = 0>
inline bool operator==(C const& lhs, C const& rhs) noexcept
{
    return lhs.id() == rhs.id();
}

template<class C, std::enable_if_t<std::is_base_of<tags::Component, C>::value, int> = 0>
inline std::size_t hash_value(C const& v)
{
    return std::hash<void*>{}(v.id());
}

}} // ompu
