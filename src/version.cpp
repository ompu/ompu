#include "ompu/version.hpp"

#include <boost/format.hpp>


namespace ompu {

Version::Version(unsigned major, unsigned minor, unsigned patch, std::string release)
    : internal_(std::make_tuple(std::move(major), std::move(minor), std::move(patch), std::move(release)))
{}

std::string Version::str() const
{
    return release().empty() ?
        boost::str(boost::format("%d.%d.%d") % major() % minor() % patch()) :
        boost::str(boost::format("%d.%d.%d-%s") % major() % minor() % patch() % release())
    ;
}

void Version::upgrade(Version rhs)
{
    if (rhs >= *this) {
        (*this) = std::move(rhs);
    }
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