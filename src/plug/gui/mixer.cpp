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
//[/Headers]

#include "mixer.h"


//[MiscUserDefs] You can add your own user definitions and misc code here...
namespace ompu { namespace plug { namespace gui {
//[/MiscUserDefs]

//==============================================================================
Mixer::Mixer (App& app_)
    : AppComponent(app_)
{
    //[Constructor_pre] You can add your own custom stuff here..
    //[/Constructor_pre]

    setName ("Mixer");
    addAndMakeVisible (add_ch_ = new TextButton ("new button"));
    add_ch_->setButtonText (TRANS("+"));
    add_ch_->addListener (this);
    add_ch_->setColour (TextButton::buttonColourId, Colour (0xff586be9));

    addAndMakeVisible (channels_ = new Viewport ("new viewport"));
    channels_->setScrollBarsShown (false, true);
    channels_->setScrollBarThickness (12);


    //[UserPreSize]
    //[/UserPreSize]

    setSize (1600, 320);


    //[Constructor] You can add your own custom stuff here..
    //[/Constructor]
}

Mixer::~Mixer()
{
    //[Destructor_pre]. You can add your own custom destruction code here..
    //[/Destructor_pre]

    add_ch_ = nullptr;
    channels_ = nullptr;


    //[Destructor]. You can add your own custom destruction code here..
    //[/Destructor]
}

//==============================================================================
void Mixer::paint (Graphics& g)
{
    //[UserPrePaint] Add your own custom painting code here..
    //[/UserPrePaint]

    g.fillAll (Colour (0xff323e44));

    //[UserPaint] Add your own custom painting code here..
    //[/UserPaint]
}

void Mixer::resized()
{
    //[UserPreResize] Add your own custom resize code here..
    //[/UserPreResize]

    add_ch_->setBounds (proportionOfWidth (0.0075f), proportionOfHeight (0.0250f), proportionOfWidth (0.0150f), proportionOfHeight (0.0750f));
    channels_->setBounds (proportionOfWidth (0.0300f), proportionOfHeight (0.0000f), proportionOfWidth (0.9700f), proportionOfHeight (1.0000f));
    //[UserResized] Add your own custom resize handling here..
    //[/UserResized]
}

void Mixer::buttonClicked (Button* buttonThatWasClicked)
{
    //[UserbuttonClicked_Pre]
    //[/UserbuttonClicked_Pre]

    if (buttonThatWasClicked == add_ch_)
    {
        //[UserButtonCode_add_ch_] -- add your button handler code here..
        //[/UserButtonCode_add_ch_]
    }

    //[UserbuttonClicked_Post]
    //[/UserbuttonClicked_Post]
}



//[MiscUserCode] You can add your own definitions of your custom methods or any other code here...
}}} // ompu
//[/MiscUserCode]


//==============================================================================
#if 0
/*  -- Projucer information section --

    This is where the Projucer stores the metadata that describe this GUI layout, so
    make changes in here at your peril!

BEGIN_JUCER_METADATA

<JUCER_COMPONENT documentType="Component" className="Mixer" componentName="Mixer"
                 parentClasses="public ompu::plug::base::AppComponent&lt;juce::Component&gt;"
                 constructorParams="App&amp; app_" variableInitialisers="AppComponent(app_)"
                 snapPixels="8" snapActive="1" snapShown="1" overlayOpacity="0.330"
                 fixedSize="1" initialWidth="1600" initialHeight="320">
  <BACKGROUND backgroundColour="ff323e44"/>
  <TEXTBUTTON name="new button" id="18042c86f0ff4e22" memberName="add_ch_"
              virtualName="" explicitFocusOrder="0" pos="0.75% 2.5% 1.5% 7.5%"
              bgColOff="ff586be9" buttonText="+" connectedEdges="0" needsCallback="1"
              radioGroupId="0"/>
  <VIEWPORT name="new viewport" id="657506eda669105a" memberName="channels_"
            virtualName="" explicitFocusOrder="0" pos="3% 0% 97% 100%" vscroll="0"
            hscroll="1" scrollbarThickness="12" contentType="0" jucerFile=""
            contentClass="" constructorParams=""/>
</JUCER_COMPONENT>

END_JUCER_METADATA
*/
#endif


//[EndFile] You can add extra defines here...
//[/EndFile]
