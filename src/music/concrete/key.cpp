#include "ompu/music/concrete/key.hpp"


#include <boost/format.hpp>


namespace ompu { namespace music { namespace concrete {

std::string Key::name() const
{
    return boost::str(boost::format("%s%s %s") % nh_ % mod_ % feel_);
}

std::ostream& operator<<(std::ostream& os, Key const& key)
{
    return os << key.nh_ << key.mod_ << " " << key.feel_;
}

std::ostream& operator<<(std::ostream& os, Key::Feel const& kf)
{
    switch (kf) {
    case Key::Feel::Major: return os << "Major";
    case Key::Feel::Minor: return os << "Minor";
    default: throw std::logic_error("unhandled Key::Feel value");
    }
}


}}} // ompu
