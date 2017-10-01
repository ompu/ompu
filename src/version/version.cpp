#include "ompu/version.hpp"
#include "ompu/io.hpp"

#pragma warning(push)

#if BOOST_MSVC
#pragma warning(disable: 4819 4459)
#endif

// #include <boost/spirit/include/qi.hpp>
#include <boost/spirit/include/qi_rule.hpp>
#include <boost/spirit/include/qi_lit.hpp>
#include <boost/spirit/include/qi_uint.hpp>
#include <boost/spirit/include/qi_char_class.hpp>
#include <boost/spirit/include/qi_char.hpp>
#include <boost/spirit/include/qi_optional.hpp>
#include <boost/spirit/include/qi_operator.hpp>
#include <boost/spirit/include/qi_parse.hpp>

#pragma warning(push)
#if BOOST_MSVC
#pragma warning(disable: 4456)
#endif

#include <boost/spirit/include/karma.hpp>
#pragma warning(pop)

#pragma warning(pop)


#include <boost/core/ignore_unused.hpp>
#include <boost/format.hpp>

#include <tuple>


namespace ompu {

version_policy_error::version_policy_error(Version const& target_version, Version const& actual_version, char const* policy)
    : std::invalid_argument(boost::str(
        boost::format("target version '%s' is incompatible with actual version '%s' for policy '%s'")
            % target_version % actual_version % policy
    ))
{}

Version::Version(major_type major, minor_type minor, patch_type patch, pre_release_type pre_release) noexcept
    : major(std::move(major))
    , minor(std::move(minor))
    , patch(std::move(patch))
    , pre_release(std::move(pre_release))
{}

Version::Version(major_type major, minor_type minor, patch_type patch) noexcept
    : major(std::move(major))
    , minor(std::move(minor))
    , patch(std::move(patch))
{}

std::string Version::str() const
{
    namespace kr = boost::spirit::karma;
    namespace ns = kr::standard;

    std::string buf;
    using iterator_type = std::back_insert_iterator<decltype(buf)>;

    kr::rule<iterator_type, Version::pre_release_type()> pre_release_rule;
    pre_release_rule.name("version-pre-release");
    pre_release_rule =
        kr::lit('-') << ns::string
    ;

    kr::rule<iterator_type, Version()> rule;
    rule.name("version");
    rule =
        kr::uint_ << kr::lit('.') << kr::uint_ << kr::lit('.') << kr::uint_ << -pre_release_rule
    ;

    auto const success = kr::generate(std::back_inserter(buf), rule, *this);
    if (!success) {
        throw std::invalid_argument("ompu::Version: failed to serialize");
    }

    return buf;
}

Version make_version(std::string const& buf)
{
    namespace qi = boost::spirit::qi;
    namespace ns = qi::standard;

    auto it = std::cbegin(buf);
    auto const last = std::cend(buf);

    qi::rule<decltype(it), Version::pre_release_type()> pre_release_rule;
    pre_release_rule.name("version-pre-release");
    pre_release_rule =
        qi::lit('-') > +ns::char_
    ;

    qi::rule<decltype(it), Version()> rule;
    rule.name("version");
    rule =
        qi::uint_ > qi::lit('.') > qi::uint_ > qi::lit('.') > qi::uint_ > -pre_release_rule
    ;

    Version v;
    bool const success = qi::parse(
        it,
        last,
        rule,
        v
    );

    if (!(success && it == last)) {
        throw std::invalid_argument("ompu::Version: invalid version string format");
    }
    return v;
}

bool Version::operator<(Version const& rhs) const noexcept
{
    if (this->major == rhs.major && this->minor == rhs.minor && this->patch == rhs.patch) {
        if (!this->pre_release) {
            if (!rhs.pre_release) {
                return false; // no release & no release -> always eq
            } else {
                return false; // no release & has release -> always newer
            }
        } else {
            if (!rhs.pre_release) {
                return true; // has release & no release -> always older
            } else {
                return *this->pre_release < *rhs.pre_release; // has release & has release -> lexicographic compare
            }
        }
    }

    return
        std::forward_as_tuple(this->major, this->minor, this->patch)
        < std::forward_as_tuple(rhs.major, rhs.minor, rhs.patch)
    ;
}

bool Version::operator==(Version const& rhs) const noexcept
{
    return this->major == rhs.major && this->minor && rhs.minor && this->patch == rhs.patch && this->pre_release == rhs.pre_release;
}

} // ompu
