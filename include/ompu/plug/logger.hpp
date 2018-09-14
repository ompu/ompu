#pragma once

#include "JuceHeader.h"

#include "saya/logger.hpp"

#include <sstream>


namespace ompu { namespace logger {

class DebugBuffer : public std::stringbuf
{
protected:
    virtual int sync() override
    {
        juce::Logger::outputDebugString(this->str());
        this->str({});
        return 0;
    }
};

}} // ompu

