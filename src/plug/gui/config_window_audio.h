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

namespace ompu { namespace plug { namespace gui {
//[/Headers]



//==============================================================================
/**
                                                                    //[Comments]
    An auto-generated component, created by the Projucer.

    Describe your class and how it works here!
                                                                    //[/Comments]
*/
class ConfigWindowAudio  : public ompu::plug::base::AppComponent<juce::Component>,
                           public juce::ChangeListener,
                           public ComboBox::Listener,
                           public Button::Listener
{
public:
    //==============================================================================
    ConfigWindowAudio (App& app_);
    ~ConfigWindowAudio();

    //==============================================================================
    //[UserMethods]     -- You can add your own custom methods in this section.
    void changeListenerCallback(juce::ChangeBroadcaster* src) override;
    //[/UserMethods]

    void paint (Graphics& g) override;
    void resized() override;
    void comboBoxChanged (ComboBox* comboBoxThatHasChanged) override;
    void buttonClicked (Button* buttonThatWasClicked) override;



private:
    //[UserVariables]   -- You can add your own custom variables in this section.
    void fetchSampleRate();
    void fetchBufferSize();
    //[/UserVariables]

    //==============================================================================
    ScopedPointer<Label> label2;
    ScopedPointer<Viewport> in_ch_;
    ScopedPointer<Label> label3;
    ScopedPointer<Viewport> out_ch_;
    ScopedPointer<Label> label;
    ScopedPointer<ComboBox> host_device_;
    ScopedPointer<TextButton> play_test_;
    ScopedPointer<TextButton> cfg_;
    ScopedPointer<TextButton> reset_;
    ScopedPointer<Label> label4;
    ScopedPointer<ComboBox> sample_rate_;
    ScopedPointer<Label> label5;
    ScopedPointer<ComboBox> buffer_size_;


    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (ConfigWindowAudio)
};

//[EndFile] You can add extra defines here...
}}} // ompu
//[/EndFile]
