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
#include "logo.h"
//[/Headers]

#include "config_window_home.h"


//[MiscUserDefs] You can add your own user definitions and misc code here...
namespace ompu { namespace plug { namespace gui {
//[/MiscUserDefs]

//==============================================================================
ConfigWindowHome::ConfigWindowHome (App& app_)
    : AppComponent(app_)
{
    //[Constructor_pre] You can add your own custom stuff here..
    //[/Constructor_pre]

    setName ("ConfigWindowHome");
    addAndMakeVisible (logo_ = new ompu::plug::gui::Logo (app_));
    logo_->setName ("new component");


    //[UserPreSize]
    //[/UserPreSize]

    setSize (600, 400);


    //[Constructor] You can add your own custom stuff here..
    //[/Constructor]
}

ConfigWindowHome::~ConfigWindowHome()
{
    //[Destructor_pre]. You can add your own custom destruction code here..
    //[/Destructor_pre]

    logo_ = nullptr;


    //[Destructor]. You can add your own custom destruction code here..
    //[/Destructor]
}

//==============================================================================
void ConfigWindowHome::paint (Graphics& g)
{
    //[UserPrePaint] Add your own custom painting code here..
    //[/UserPrePaint]

    g.fillAll (Colour (0xffedf8fe));

    //[UserPaint] Add your own custom painting code here..
    //[/UserPaint]
}

void ConfigWindowHome::resized()
{
    //[UserPreResize] Add your own custom resize code here..
    //[/UserPreResize]

    logo_->setBounds (proportionOfWidth (0.3055f) - (proportionOfWidth (0.4607f) / 2), proportionOfHeight (0.0123f), proportionOfWidth (0.4607f), proportionOfHeight (0.4713f));
    //[UserResized] Add your own custom resize handling here..
    //[/UserResized]
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

<JUCER_COMPONENT documentType="Component" className="ConfigWindowHome" componentName="ConfigWindowHome"
                 parentClasses="public ompu::plug::base::AppComponent&lt;juce::Component&gt;"
                 constructorParams="App&amp; app_" variableInitialisers="AppComponent(app_)"
                 snapPixels="8" snapActive="1" snapShown="1" overlayOpacity="0.330"
                 fixedSize="0" initialWidth="600" initialHeight="400">
  <BACKGROUND backgroundColour="ffedf8fe"/>
  <GENERICCOMPONENT name="new component" id="e9340e0754d5002b" memberName="logo_"
                    virtualName="" explicitFocusOrder="0" pos="30.554%c 1.23% 46.07% 47.131%"
                    class="ompu::plug::gui::Logo" params="app_"/>
</JUCER_COMPONENT>

END_JUCER_METADATA
*/
#endif


//[EndFile] You can add extra defines here...
//[/EndFile]
