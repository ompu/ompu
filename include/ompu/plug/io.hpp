#pragma once

#include "JuceHeader.h"

#include <iosfwd>


namespace ompu { namespace plug {

std::ostream& operator<<(std::ostream& os, juce::AudioDeviceManager::AudioDeviceSetup const& v);

}} // ompu
