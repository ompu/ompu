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
#include "ompu/component_id.hpp"

#include <vector>
#include <utility>

namespace ompu { namespace plug { namespace gui {
//[/Headers]



//==============================================================================
/**
                                                                    //[Comments]
    An auto-generated component, created by the Projucer.

    Describe your class and how it works here!
                                                                    //[/Comments]
*/
class MIDIChannelList  : public juce::ChangeListener,
                         public ompu::plug::base::AppComponent<juce::ListBox>,
                         public juce::ListBoxModel
{
public:
    //==============================================================================
    MIDIChannelList (App& app_);
    ~MIDIChannelList();

    //==============================================================================
    //[UserMethods]     -- You can add your own custom methods in this section.
    void changeListenerCallback(juce::ChangeBroadcaster* src) override;

    int getNumRows() override;
    void paintListBoxItem(int row_num, juce::Graphics& g, int width, int height, bool is_selected) override;
    juce::Component* refreshComponentForRow(int row_num, bool is_selected, juce::Component* target) override;
    //[/UserMethods]

    void paint (Graphics& g) override;
    void resized() override;



private:
    //[UserVariables]   -- You can add your own custom variables in this section.
    std::vector<std::pair<ComponentID, std::string>>
    devs_;
    //[/UserVariables]

    //==============================================================================


    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (MIDIChannelList)
};

//[EndFile] You can add extra defines here...
}}} // ompu
//[/EndFile]
