#pragma once

#include <boost/system/error_code.hpp>
#include <boost/config.hpp>

#include <system_error>
#include <stdexcept>

#if BOOST_MSVC
#include <windows.h>
#endif

namespace ompu { namespace plug {

[[noreturn]]
inline void throw_system_error()
{
    throw std::system_error{errno, std::system_category()};
}

[[noreturn]]
inline void throw_windows_error()
{
    throw std::system_error{static_cast<int>(::GetLastError()), std::system_category()};
}

inline boost::system::error_code make_windows_error()
{
    return boost::system::error_code{static_cast<int>(::GetLastError()), boost::system::system_category()};
}

}} // ompu
