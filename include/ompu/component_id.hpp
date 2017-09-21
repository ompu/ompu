#pragma once

#include "ompu/ompu_fwd.hpp"

#include <boost/assert.hpp>

#include <functional>


namespace ompu {

class ComponentID
{
public:
    using id_type = int;

    ComponentID() noexcept = default;

    explicit ComponentID(id_type v) noexcept
        : id_(v)
    {
        BOOST_ASSERT(v != 0); // 0 is invalid in juce context
    }

    ~ComponentID() noexcept = default;

    ComponentID(ComponentID const&) noexcept = default;
    ComponentID(ComponentID&&) noexcept = default;

    ComponentID& operator=(ComponentID const&) noexcept = default;
    ComponentID& operator=(ComponentID&&) noexcept = default;

    id_type id() const noexcept { return id_; }
    id_type index() const noexcept { return id_ - 1; }

private:
    id_type id_{0};
};

bool operator==(ComponentID const& lhs, ComponentID const& rhs);

template<class T>
inline ComponentID from_id(T v) noexcept { return ComponentID{static_cast<int>(v)}; }

template<class T>
inline ComponentID from_index(T v) noexcept { return ComponentID{static_cast<int>(v) + 1}; }

} // ompu

namespace std {

template<>
class hash<ompu::ComponentID>
{
public:
    bool operator()(ompu::ComponentID const& v) const
    {
        return hash<decltype(v.id())>{}(v.id());
    }
};

} // std
