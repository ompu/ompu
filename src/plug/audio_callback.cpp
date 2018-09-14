#include "ompu/plug/app.hpp"
#include "ompu/plug/audio_callback.hpp"

#include <boost/core/ignore_unused.hpp>


namespace ompu { namespace plug {

saya::logger& AudioCallback::l()
{
    return app_.l();
}

AudioCallback::AudioCallback(App& app)
    : app_(app)
{}

void AudioCallback::audioDeviceIOCallback(
    float const** in_data,
    int in_ch_num,
    float** out_data,
    int out_ch_num,
    int sample_num
) {
    boost::ignore_unused(in_data, in_ch_num, out_data, out_ch_num, sample_num);

    for (int out_ch = 0; out_ch < out_ch_num; ++out_ch) {
        std::memset(out_data[out_ch], 0, sample_num);
    }
}

void AudioCallback::audioDeviceAboutToStart(juce::AudioIODevice* dev)
{
    l().note() << "type: " << dev->getTypeName() << std::endl;
    l().note() << "name: " << dev->getName() << std::endl;
    l().note() << "sample rate: " << dev->getCurrentSampleRate() << std::endl;
    l().note() << "buffer size samples: " << dev->getCurrentBufferSizeSamples() << std::endl;
    l().info() << "audio device starting..." << std::endl;
}

void AudioCallback::audioDeviceStopped()
{
    l().warn() << "audio device stopped!" << std::endl;
}

void AudioCallback::audioDeviceError(juce::String const& msg)
{
    l().error() << msg << std::endl;
}

}} // ompu
