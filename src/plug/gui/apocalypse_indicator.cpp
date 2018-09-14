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

#include "apocalypse_indicator.h"


//[MiscUserDefs] You can add your own user definitions and misc code here...
namespace ompu { namespace plug { namespace gui {
//[/MiscUserDefs]

//==============================================================================
ApocalypseIndicator::ApocalypseIndicator (App& app_)
    : AppComponent(app_)
{
    //[Constructor_pre] You can add your own custom stuff here..
    //[/Constructor_pre]

    setName ("Apocalypse");
    addAndMakeVisible (apocalypse_trigger_ = new TextButton ("Configure"));
    apocalypse_trigger_->setButtonText (TRANS("Configuration needed!"));
    apocalypse_trigger_->addListener (this);
    apocalypse_trigger_->setColour (TextButton::buttonColourId, Colour (0xffa4845c));


    //[UserPreSize]
    //[/UserPreSize]

    setSize (640, 360);


    //[Constructor] You can add your own custom stuff here..
    //[/Constructor]
}

ApocalypseIndicator::~ApocalypseIndicator()
{
    //[Destructor_pre]. You can add your own custom destruction code here..
    //[/Destructor_pre]

    apocalypse_trigger_ = nullptr;


    //[Destructor]. You can add your own custom destruction code here..
    //[/Destructor]
}

//==============================================================================
void ApocalypseIndicator::paint (Graphics& g)
{
    //[UserPrePaint] Add your own custom painting code here..
    //[/UserPrePaint]

    g.fillAll (Colour (0x8f000000));

    //[UserPaint] Add your own custom painting code here..
    //[/UserPaint]
}

void ApocalypseIndicator::resized()
{
    //[UserPreResize] Add your own custom resize code here..
    //[/UserPreResize]

    apocalypse_trigger_->setBounds (8, 8, 150, 24);
    //[UserResized] Add your own custom resize handling here..
    //[/UserResized]
}

void ApocalypseIndicator::buttonClicked (Button* buttonThatWasClicked)
{
    //[UserbuttonClicked_Pre]
    //[/UserbuttonClicked_Pre]

    if (buttonThatWasClicked == apocalypse_trigger_)
    {
        //[UserButtonCode_apocalypse_trigger_] -- add your button handler code here..
        //[/UserButtonCode_apocalypse_trigger_]
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

<JUCER_COMPONENT documentType="Component" className="ApocalypseIndicator" componentName="Apocalypse"
                 parentClasses="public ompu::plug::base::AppComponent&lt;juce::Component&gt;, public juce::ChangeBroadcaster"
                 constructorParams="App&amp; app_" variableInitialisers="AppComponent(app_)"
                 snapPixels="8" snapActive="1" snapShown="1" overlayOpacity="0.330"
                 fixedSize="1" initialWidth="640" initialHeight="360">
  <BACKGROUND backgroundColour="8f000000"/>
  <TEXTBUTTON name="Configure" id="6495fe84e7aadc37" memberName="apocalypse_trigger_"
              virtualName="" explicitFocusOrder="0" pos="8 8 150 24" bgColOff="ffa4845c"
              buttonText="Configuration needed!" connectedEdges="0" needsCallback="1"
              radioGroupId="0"/>
</JUCER_COMPONENT>

END_JUCER_METADATA
*/
#endif


//[EndFile] You can add extra defines here...
//[/EndFile]
