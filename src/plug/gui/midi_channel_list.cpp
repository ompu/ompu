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

//[Headers] You can add your own extra header files here...
#include "ompu/plug/util.hpp"
#include "midi_config.h"

#include <boost/range/adaptor/indexed.hpp>
//[/Headers]

#include "midi_channel_list.h"


//[MiscUserDefs] You can add your own user definitions and misc code here...
namespace ompu { namespace plug { namespace gui {
//[/MiscUserDefs]

//==============================================================================
MIDIChannelList::MIDIChannelList (App& app_)
    : AppComponent(app_, juce::String{}, nullptr)
{
    //[Constructor_pre] You can add your own custom stuff here..
    //[/Constructor_pre]

    setName ("MIDIChannelList");

    //[UserPreSize]
    //[/UserPreSize]

    setSize (600, 400);


    //[Constructor] You can add your own custom stuff here..
    this->setModel(this);
    this->setColour(juce::ListBox::backgroundColourId, juce::Colour::fromRGBA(0, 0, 0, 160));
    dm().addChangeListener(this);
    //[/Constructor]
}

MIDIChannelList::~MIDIChannelList()
{
    //[Destructor_pre]. You can add your own custom destruction code here..
    //[/Destructor_pre]



    //[Destructor]. You can add your own custom destruction code here..
    //[/Destructor]
}

//==============================================================================
void MIDIChannelList::paint (Graphics& g)
{
    //[UserPrePaint] Add your own custom painting code here..
    //[/UserPrePaint]

    g.fillAll (Colour (0xff323e44));

    //[UserPaint] Add your own custom painting code here..
    //[/UserPaint]
}

void MIDIChannelList::resized()
{
    //[UserPreResize] Add your own custom resize code here..
    //[/UserPreResize]

    //[UserResized] Add your own custom resize handling here..
    {
        auto* const vp = this->getViewport();
        //vp->setSize(this->getWidth(), this->getHeight());
        vp->setBoundsInset(juce::BorderSize<int>{0});
    }
    //[/UserResized]
}



//[MiscUserCode] You can add your own definitions of your custom methods or any other code here...
int MIDIChannelList::getNumRows()
{
    return static_cast<int>(devs_.size());
}

void MIDIChannelList::paintListBoxItem(int row_num, juce::Graphics& g, int width, int height, bool is_selected)
{
    boost::ignore_unused(row_num, g, width, height, is_selected);
}

juce::Component* MIDIChannelList::refreshComponentForRow(int row_num, bool is_selected, juce::Component* target)
{
    boost::ignore_unused(is_selected);

    if (!(0 <= row_num && row_num < devs_.size())) return target;

    if (!target) {
        target = new MIDIConfig(app(), from_index(row_num));
    }

    return target;
}

void MIDIChannelList::changeListenerCallback(juce::ChangeBroadcaster* const src)
{
    if (src != std::addressof(dm())) return;

    auto const devs = juce::MidiInput::getDevices();
    devs_.clear();
    devs_.reserve(devs.size());

    for (auto const& dev_name : make_range(devs) | boost::adaptors::indexed(0)) {
        devs_.emplace_back(std::make_pair(from_index(dev_name.index()), dev_name.value().toStdString()));
    }

    this->updateContent();
}

}}} // ompu
//[/MiscUserCode]


//==============================================================================
#if 0
/*  -- Projucer information section --

    This is where the Projucer stores the metadata that describe this GUI layout, so
    make changes in here at your peril!

BEGIN_JUCER_METADATA

<JUCER_COMPONENT documentType="Component" className="MIDIChannelList" componentName="MIDIChannelList"
                 parentClasses="public juce::ChangeListener, public ompu::plug::base::AppComponent&lt;juce::ListBox&gt;, public juce::ListBoxModel"
                 constructorParams="App&amp; app_" variableInitialisers="AppComponent(app_, juce::String{}, nullptr)"
                 snapPixels="8" snapActive="1" snapShown="1" overlayOpacity="0.330"
                 fixedSize="0" initialWidth="600" initialHeight="400">
  <BACKGROUND backgroundColour="ff323e44"/>
</JUCER_COMPONENT>

END_JUCER_METADATA
*/
#endif


//[EndFile] You can add extra defines here...
//[/EndFile]
