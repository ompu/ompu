#pragma once

#include "JuceHeader.h"

namespace ompu { namespace plug {

class DeviceManager : public juce::AudioDeviceManager
{
public:
    virtual void createAudioDeviceTypes(juce::OwnedArray<juce::AudioIODeviceType>& dtypes) override
    {
        dtypes.add(juce::AudioIODeviceType::createAudioIODeviceType_ASIO());
        dtypes.add(juce::AudioIODeviceType::createAudioIODeviceType_CoreAudio());
        dtypes.add(juce::AudioIODeviceType::createAudioIODeviceType_JACK());
        dtypes.add(juce::AudioIODeviceType::createAudioIODeviceType_ALSA());
    }

private:
};

}} // ompu
