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
#include "config_window_home.h"
#include "config_window_app.h"
#include "config_window_midi.h"
#include "config_window_audio.h"
//[/Headers]

#include "config_window.h"


//[MiscUserDefs] You can add your own user definitions and misc code here...
namespace ompu { namespace plug { namespace gui {
//[/MiscUserDefs]

//==============================================================================
ConfigWindow::ConfigWindow (App& app_)
    : AppComponent(app_)
{
    //[Constructor_pre] You can add your own custom stuff here..
    //[/Constructor_pre]

    setName ("ConfigWindow");
    addAndMakeVisible (tabs_ = new TabbedComponent (TabbedButtonBar::TabsAtLeft));
    tabs_->setTabBarDepth (80);
    tabs_->addTab (TRANS("Home"), Colours::lightgrey, new ompu::plug::gui::ConfigWindowHome (app_), true);
    tabs_->addTab (TRANS("Application"), Colours::lightgrey, new ompu::plug::gui::ConfigWindowApp (app_), true);
    tabs_->addTab (TRANS("MIDI"), Colours::lightgrey, new ompu::plug::gui::ConfigWindowMIDI (app_), true);
    tabs_->addTab (TRANS("Audio"), Colours::lightgrey, new ompu::plug::gui::ConfigWindowAudio (app_), true);
    tabs_->setCurrentTabIndex (0);


    //[UserPreSize]
    //[/UserPreSize]

    setSize (1600, 1000);


    //[Constructor] You can add your own custom stuff here..
    //[/Constructor]
}

ConfigWindow::~ConfigWindow()
{
    //[Destructor_pre]. You can add your own custom destruction code here..
    //[/Destructor_pre]

    tabs_ = nullptr;


    //[Destructor]. You can add your own custom destruction code here..
    //[/Destructor]
}

//==============================================================================
void ConfigWindow::paint (Graphics& g)
{
    //[UserPrePaint] Add your own custom painting code here..
    //[/UserPrePaint]

    g.fillAll (Colour (0xff323e44));

    //[UserPaint] Add your own custom painting code here..
    //[/UserPaint]
}

void ConfigWindow::resized()
{
    //[UserPreResize] Add your own custom resize code here..
    //[/UserPreResize]

    tabs_->setBounds (proportionOfWidth (0.0000f), proportionOfHeight (0.0000f), proportionOfWidth (1.0000f), proportionOfHeight (1.0000f));
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

<JUCER_COMPONENT documentType="Component" className="ConfigWindow" componentName="ConfigWindow"
                 parentClasses="public ompu::plug::base::AppComponent&lt;juce::Component&gt;"
                 constructorParams="App&amp; app_" variableInitialisers="AppComponent(app_)"
                 snapPixels="8" snapActive="1" snapShown="1" overlayOpacity="0.330"
                 fixedSize="0" initialWidth="1600" initialHeight="1000">
  <BACKGROUND backgroundColour="ff323e44"/>
  <TABBEDCOMPONENT name="new tabbed component" id="16526c96f1ca7f50" memberName="tabs_"
                   virtualName="" explicitFocusOrder="0" pos="0% 0% 100% 100%" orientation="left"
                   tabBarDepth="80" initialTab="0">
    <TAB name="Home" colour="ffd3d3d3" useJucerComp="0" contentClassName="ompu::plug::gui::ConfigWindowHome"
         constructorParams="app_" jucerComponentFile=""/>
    <TAB name="Application" colour="ffd3d3d3" useJucerComp="0" contentClassName="ompu::plug::gui::ConfigWindowApp"
         constructorParams="app_" jucerComponentFile=""/>
    <TAB name="MIDI" colour="ffd3d3d3" useJucerComp="0" contentClassName="ompu::plug::gui::ConfigWindowMIDI"
         constructorParams="app_" jucerComponentFile=""/>
    <TAB name="Audio" colour="ffd3d3d3" useJucerComp="0" contentClassName="ompu::plug::gui::ConfigWindowAudio"
         constructorParams="app_" jucerComponentFile=""/>
  </TABBEDCOMPONENT>
</JUCER_COMPONENT>

END_JUCER_METADATA
*/
#endif


//[EndFile] You can add extra defines here...
//[/EndFile]
