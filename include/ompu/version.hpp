#pragma once

#include <tuple>
#include <string>

namespace ompu {

class Version
{
public:
    using internal_type = std::tuple<unsigned, unsigned, unsigned, std::string>;

    explicit Version(unsigned major, unsigned minor = 0, unsigned patch = 0, std::string release = {});

    Version(Version const&) = default;
    Version(Version&&) = default;

    Version& operator=(Version const&) = delete;
    Version& operator=(Version&&) = default;

    void upgrade(Version rhs);

    unsigned major() const noexcept { return std::get<0>(internal_); }
    unsigned minor() const noexcept { return std::get<1>(internal_); }
    unsigned patch() const noexcept { return std::get<2>(internal_); }
    std::string const& release() const noexcept { return std::get<3>(internal_); }

    std::string str() const;
    explicit operator std::string() const { return this->str(); }

    friend bool operator<(Version const& lhs, Version const& rhs) noexcept;
    friend bool operator<=(Version const& lhs, Version const& rhs) noexcept;
    friend bool operator>(Version const& lhs, Version const& rhs) noexcept;
    friend bool operator>=(Version const& lhs, Version const& rhs) noexcept;
    friend bool operator==(Version const& lhs, Version const& rhs) noexcept;

private:
    internal_type internal_;
};

} // ompu
