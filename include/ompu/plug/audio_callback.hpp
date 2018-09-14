#pragma once

#include "ompu/plug/app_fwd.hpp"

#include "saya/logger.hpp"

#include "JuceHeader.h"


namespace ompu { namespace plug {

class AudioCallback : public juce::AudioIODeviceCallback
{
public:
    explicit AudioCallback(App& app);

    virtual void audioDeviceIOCallback(
        float const** in_data,
        int in_ch_num,
        float** out_data,
        int out_ch_num,
        int sample_num
    ) override;

    virtual void audioDeviceAboutToStart(juce::AudioIODevice* dev) override;
    virtual void audioDeviceStopped() override;
    virtual void audioDeviceError(juce::String const& msg) override;

private:
    saya::logger& l();

    App& app_;
};

}} // ompu
