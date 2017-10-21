#pragma once

#include "saya/bridge.hpp"
#include <boost/filesystem/path.hpp>

namespace ompu_test {

class bridge : public saya::bridge<bridge>
{
public:
    void validate(vm_type const&) override {}

//#undef SAYA_BRIDGE_OPTION_GROUP
//#define SAYA_BRIDGE_OPTION_GROUP (general, "General options")

#undef SAYA_BRIDGE_OPTION_GROUP
#define SAYA_BRIDGE_OPTION_GROUP (misc, "Miscellaneous options")
    SAYA_BRIDGE_OPTION_DEFAULTED(std::string, gtest_color, "yes", "gtest_color", "Enables Google Test color (yes/no)")

#undef SAYA_BRIDGE_OPTION_GROUP
};

} // saya_test
