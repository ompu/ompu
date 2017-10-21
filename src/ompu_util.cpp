#include "ompu/util.hpp"

#include <boost/config.hpp>

#include <iostream>
#include <iomanip>

#include <ctime>


namespace ompu { namespace util {

#pragma warning(push)
#if BOOST_MSVC
# pragma warning(disable: 4996)
#endif

std::ostream& operator<<(std::ostream& os, time_print const& v)
{
    auto const tc = std::chrono::system_clock::to_time_t(v.tp);
    return os << std::put_time(std::localtime(&tc), "%F %T %z");
}

#pragma warning(pop)

}} // ompu
