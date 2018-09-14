#pragma once

#include "ompu/plug/app_fwd.hpp"

#include "JuceHeader.h"


namespace ompu { namespace plug { namespace gui {

class ApocalypseIndicator;

class Navbar;
class Logo;

class MIDIChannelList;
class MIDIConfig;

class ConfigWindow;
class ConfigWindowHome;
class ConfigWindowApp;
class ConfigWindowMIDI;
class ConfigWindowAudio;

class MixerChannel;
class MixerPlugin;
class Mixer;

}}} // ompu

#if !defined(OMPU_PLUG_NO_JUCE_USING_DECLARATIONS)
namespace ompu { namespace plug { namespace gui {

// FUCK!

using ::juce::roundFloatToInt;

using ::juce::ScopedPointer;
using ::juce::String;

using ::juce::Drawable;
using ::juce::TabbedComponent;
using ::juce::GroupComponent;

using ::juce::Button;
using ::juce::ToggleButton;
using ::juce::TextButton;

using ::juce::Label;
using ::juce::ComboBox;
using ::juce::Viewport;
using ::juce::TextEditor;
using ::juce::TabbedButtonBar;
using ::juce::Slider;

using ::juce::Point;
using ::juce::Rectangle;
using ::juce::RectanglePlacement;

using ::juce::Graphics;
using ::juce::Font;
using ::juce::Colour;
using ::juce::Colours;
using ::juce::Justification;

using ::juce::dontSendNotification;

}}} // ompu

#endif // OMPU_PLUG_NO_JUCE_USING_DECLARATIONS
