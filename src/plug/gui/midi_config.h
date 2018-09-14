/*
  ==============================================================================

  This is an automatically generated GUI class created by the Projucer!

  Be careful when adding custom code to these files, as only the code within
  the "//[xyz]" and "//[/xyz]" sections will be retained when the file is loaded
  and re-saved.

  Created with Projucer version: 5.1.2

  ------------------------------------------------------------------------------

  The Projucer is part of the JUCE library - "Jules' Utility Class Extensions"
  Copyright (c) 2015 - ROLI Ltd.

  ==============================================================================
*/

#pragma once

//[Headers]     -- You can add your own extra header files here --
#include "ompu/plug/base/app_component.hpp"

#include "ompu/midi/midi_fwd.hpp"

#include <array>
#include <vector>

namespace ompu { namespace plug { namespace gui {
//[/Headers]



//==============================================================================
/**
                                                                    //[Comments]
    An auto-generated component, created by the Projucer.

    Describe your class and how it works here!
                                                                    //[/Comments]
*/
class MIDIConfig  : public ompu::plug::base::AppComponent<>,
                    public Button::Listener
{
public:
    //==============================================================================
    MIDIConfig (App& app_, ComponentID id);
    ~MIDIConfig();

    //==============================================================================
    //[UserMethods]     -- You can add your own custom methods in this section.
    //[/UserMethods]

    void paint (Graphics& g) override;
    void resized() override;
    void buttonClicked (Button* buttonThatWasClicked) override;



private:
    //[UserVariables]   -- You can add your own custom variables in this section.
    ComponentID id_;

    std::array<juce::Component const*, OMPU_MIDI_CH_MAX> chs_;
    //[/UserVariables]

    //==============================================================================
    ScopedPointer<ToggleButton> ch_1_;
    ScopedPointer<ToggleButton> ch_2_;
    ScopedPointer<ToggleButton> ch_3_;
    ScopedPointer<ToggleButton> ch_4_;
    ScopedPointer<ToggleButton> ch_5_;
    ScopedPointer<ToggleButton> ch_6_;
    ScopedPointer<ToggleButton> ch_7_;
    ScopedPointer<ToggleButton> ch_8_;
    ScopedPointer<ToggleButton> ch_9_;
    ScopedPointer<ToggleButton> ch_10_;
    ScopedPointer<ToggleButton> ch_11_;
    ScopedPointer<ToggleButton> ch_12_;
    ScopedPointer<ToggleButton> ch_13_;
    ScopedPointer<ToggleButton> ch_14_;
    ScopedPointer<ToggleButton> ch_15_;
    ScopedPointer<ToggleButton> ch_16_;
    ScopedPointer<ToggleButton> name_;


    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (MIDIConfig)
};

//[EndFile] You can add extra defines here...
}}} // ompu
//[/EndFile]
