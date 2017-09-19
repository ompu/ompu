#include "ompu/version.hpp"


namespace ompu {

Version::Version(unsigned major, unsigned minor, unsigned patch, std::string release)
    : internal_(std::make_tuple(std::move(major), std::move(minor), std::move(patch), std::move(release)))
{}

boost::format Version::format() const
{
    return release().empty() ?
        (boost::format("%d.%d.%d") % major() % minor() % patch()) :
        (boost::format("%d.%d.%d-%s") % major() % minor() % patch() % release())
    ;
}

std::string Version::str() const
{
    return boost::str(format());
}

void Version::upgrade(Version rhs)
{
    if (rhs >= *this) {
        (*this) = std::move(rhs);
    }
}

std::ostream& operator<<(std::ostream& os, Version const& v)
{
    return os << v.format();
}

bool operator<(Version const& lhs, Version const& rhs) noexcept
{ return lhs.internal_ < rhs.internal_; }

bool operator<=(Version const& lhs, Version const& rhs) noexcept
{ return lhs.internal_ <= rhs.internal_; }

bool operator>(Version const& lhs, Version const& rhs) noexcept
{ return lhs.internal_ > rhs.internal_; }

bool operator>=(Version const& lhs, Version const& rhs) noexcept
{ return lhs.internal_ >= rhs.internal_; }

bool operator==(Version const& lhs, Version const& rhs) noexcept
{ return lhs.internal_ == rhs.internal_; }

} // ompu
