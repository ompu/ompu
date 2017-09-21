#pragma once

#include <iosfwd>
#include <chrono>


namespace ompu { namespace util {

class time_print
{
public:
    using tp_type = std::chrono::system_clock::time_point;
    explicit time_print(tp_type tp)
        : tp(std::move(tp))
    {}

    std::chrono::system_clock::time_point
    tp;
};

std::ostream& operator<<(std::ostream& os, time_print const& v);

}} // ompu
