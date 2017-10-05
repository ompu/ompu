#pragma once

#if 0

#include "ompu/music/track.hpp"

#include <memory>


namespace ompu { namespace game {

class Session
{
public:
    void set_track(std::shared_ptr<music::Track> track);

private:
    std::shared_ptr<music::Track> track_;
};


}} // ompu

#endif
