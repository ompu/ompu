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
#include "ompu/plug/gui/gui_fwd.hpp"

#include "JuceHeader.h"

namespace ompu { namespace plug { namespace gui {
//[/Headers]



//==============================================================================
/**
                                                                    //[Comments]
    An auto-generated component, created by the Projucer.

    Describe your class and how it works here!
                                                                    //[/Comments]
*/
class MixerChannel  : public ompu::plug::base::AppComponent<juce::Component>,
                      public Slider::Listener,
                      public Button::Listener,
                      public Label::Listener
{
public:
    //==============================================================================
    MixerChannel (App& app_);
    ~MixerChannel();

    //==============================================================================
    //[UserMethods]     -- You can add your own custom methods in this section.
    //[/UserMethods]

    void paint (Graphics& g) override;
    void resized() override;
    void sliderValueChanged (Slider* sliderThatWasMoved) override;
    void buttonClicked (Button* buttonThatWasClicked) override;
    void labelTextChanged (Label* labelThatHasChanged) override;



private:
    //[UserVariables]   -- You can add your own custom variables in this section.
    //[/UserVariables]

    //==============================================================================
    ScopedPointer<Viewport> sends_;
    ScopedPointer<Slider> fader_;
    ScopedPointer<TextButton> show_plugins_;
    ScopedPointer<Slider> pan_;
    ScopedPointer<Label> name_;
    ScopedPointer<TextButton> midi_in_;
    ScopedPointer<Viewport> inserts_;
    ScopedPointer<Label> label;


    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (MixerChannel)
};

//[EndFile] You can add extra defines here...
}}} // ompu
//[/EndFile]
