#pragma once

#include <boost/optional.hpp>

#include <boost/fusion/include/adapt_struct.hpp>
#include <boost/operators.hpp>

#include <iosfwd>
#include <string>
#include <stdexcept>


namespace ompu {

class Version;

struct version_policy_error : std::invalid_argument
{
    explicit version_policy_error(Version const& target_version, Version const& actual_version, char const* policy);
};

namespace version_policies {

struct upgrade_by_major {};
struct upgrade_by_minor {};
struct upgrade_by_patch {};

namespace detail {

template<class VP> struct to_string;
template<> struct to_string<upgrade_by_major> { static constexpr char const* value() noexcept { return "upgrade_by_major"; } };
template<> struct to_string<upgrade_by_minor> { static constexpr char const* value() noexcept { return "upgrade_by_minor"; } };
template<> struct to_string<upgrade_by_patch> { static constexpr char const* value() noexcept { return "upgrade_by_patch"; } };

} // detail

} // version_policies


class Version
    : boost::partially_ordered<Version>
{
public:
    using major_type = unsigned;
    using minor_type = major_type;
    using patch_type = major_type;
    using pre_release_type = std::string;
    using optional_pre_release_type = boost::optional<pre_release_type>;

    Version() noexcept = default;
    ~Version() noexcept = default;
    explicit Version(major_type major, minor_type minor, patch_type patch) noexcept;
    explicit Version(major_type major, minor_type minor, patch_type patch, pre_release_type pre_release) noexcept;

    Version(Version const&) = default;
    Version(Version&&) noexcept = default;

    Version& operator=(Version const&) = default;
    Version& operator=(Version&&) noexcept = default;

    std::string str() const;
    explicit operator std::string() const { return this->str(); }

    bool operator<(Version const& rhs) const noexcept;
    bool operator==(Version const& rhs) const noexcept;

    major_type major;
    minor_type minor;
    patch_type patch;
    optional_pre_release_type pre_release;
};

Version make_version(std::string const& buf);


namespace detail {

template<class Policy>
struct upgrade_from_impl;

template<>
struct upgrade_from_impl<version_policies::upgrade_by_major>
{
    static void upgrade(Version const& real, Version& locked_at)
    {
        if (real > locked_at) {
            throw version_policy_error(
                real,
                locked_at,
                version_policies::detail::to_string<version_policies::upgrade_by_major>::value()
            );
        }
        locked_at = real;
    }
};

template<>
struct upgrade_from_impl<version_policies::upgrade_by_minor>
{
    static void upgrade(Version const& real, Version& locked_at)
    {
        if (real.major != locked_at.major || real > locked_at) {
            throw version_policy_error(
                real,
                locked_at,
                version_policies::detail::to_string<version_policies::upgrade_by_minor>::value()
            );
        }
        locked_at = real;
    }
};

template<>
struct upgrade_from_impl<version_policies::upgrade_by_patch>
{
    static void upgrade(Version const& real, Version& locked_at)
    {
        if (!(real.major == locked_at.major && real.minor == locked_at.minor) || (real > locked_at)) {
            throw version_policy_error(
                real,
                locked_at,
                version_policies::detail::to_string<version_policies::upgrade_by_minor>::value()
            );
        }
        locked_at = real;
    }
};

} // detail

template<class Policy>
inline void upgrade_version(Version const& real, Version& locked_at, Policy)
{
    return detail::upgrade_from_impl<Policy>::upgrade(real, locked_at);
}

} // ompu


BOOST_FUSION_ADAPT_STRUCT(
    ompu::Version,
    (auto, major)
    (auto, minor)
    (auto, patch)
    (auto, pre_release)
)
