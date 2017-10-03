#pragma once

#include "ompu/version/version.hpp"

#include "saya/zed/blackhole.hpp"

#include <boost/variant/variant.hpp>
#include <boost/functional/hash.hpp>

#include <unordered_map>
#include <utility>
#include <type_traits>


namespace ompu {

template<class Tag>
using make_component_version_type = std::pair<Tag, Version>;

template<class Tag, class... Args>
inline make_component_version_type<Tag>
make_component_version(Args&&... args)
{
    return make_component_version_type<Tag>{Tag{}, Version{std::forward<Args>(args)...}};
}

template<class... Tags>
class VersionSet;

namespace detail {

template<class Policy, class VSet, class Tag>
inline std::nullptr_t
version_set_upgrade_impl(Policy, VSet const& real, VSet& locked_at, Tag)
{
    upgrade_version(real.components_.at(Tag{}), locked_at.components_.at(Tag{}), Policy{});
    return nullptr;
}

} // detail

template<class... Tags>
class VersionSet
{
    template<class Policy, class VSet, class Tag>
    friend std::nullptr_t
    detail::version_set_upgrade_impl(Policy, VSet const& real, VSet& locked_at, Tag);

public:
    using any_component = boost::variant<Tags...>;
    using components_type = std::unordered_map<any_component, Version, boost::hash<any_component>>;

    VersionSet() = delete;

    explicit VersionSet(make_component_version_type<Tags>&&... versions) noexcept
        : components_{std::forward<make_component_version_type<Tags>>(versions)...}
    {}

    ~VersionSet() = default;

    VersionSet(VersionSet const&) = default;
    VersionSet(VersionSet&&) noexcept = default;

    VersionSet& operator=(VersionSet const&) = delete;
    VersionSet& operator=(VersionSet&&) noexcept = default;

    template<class Tag>
    Version& at(Tag) noexcept { return components_.at(Tag{}); }

    template<class Tag>
    Version const& at(Tag) const noexcept { return components_.at(Tag{}); }

    template<class Policy>
    void upgrade_from(VersionSet const& real, Policy)
    {
        saya::zed::blackhole(detail::version_set_upgrade_impl(
            Policy{},
            real,
            *this,
            Tags{}
        )...);
    }

    components_type const& components() const noexcept { return components_; }

private:
    components_type components_;
};

} // ompu
