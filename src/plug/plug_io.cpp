#include "ompu/plug/io.hpp"

#include <boost/format.hpp>

#include <iostream>


namespace ompu { namespace plug {

std::ostream& operator<<(std::ostream& os, juce::AudioDeviceManager::AudioDeviceSetup const& v)
{
    return os
        << boost::format(
            "[general]\n"
            "sample rate: %.0f\n"
            "buffer size: %d\n"
            "\n"
            "[input]\n"
            "device: %s\n"
            "channels: %s\n"
            "\n"
            "[output]\n"
            "device: %s\n"
            "channels: %s\n"
        )
        % v.sampleRate
        % v.bufferSize
        % v.inputDeviceName
        % (v.useDefaultInputChannels ? std::string{"(use default)"} : v.inputChannels.toString(2).toStdString())
        % v.outputDeviceName
        % (v.useDefaultOutputChannels ? std::string{"(use default)"} : v.outputChannels.toString(2).toStdString())
    ;
}


}} // ompu
