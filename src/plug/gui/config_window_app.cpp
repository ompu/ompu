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
#include "ompu/plug/host/host_app.hpp"
//[/Headers]

#include "config_window_app.h"


//[MiscUserDefs] You can add your own user definitions and misc code here...
namespace ompu { namespace plug { namespace gui {
//[/MiscUserDefs]

//==============================================================================
ConfigWindowApp::ConfigWindowApp (App& app_)
    : AppComponent(app_)
{
    //[Constructor_pre] You can add your own custom stuff here..
    //[/Constructor_pre]

    setName ("ConfigWindowApp");
    addAndMakeVisible (settings_loc_label_ = new Label ("new label",
                                                        TRANS("Application settings location")));
    settings_loc_label_->setFont (Font (15.00f, Font::plain).withTypefaceStyle ("Regular"));
    settings_loc_label_->setJustificationType (Justification::centredLeft);
    settings_loc_label_->setEditable (false, false, false);
    settings_loc_label_->setColour (TextEditor::textColourId, Colours::black);
    settings_loc_label_->setColour (TextEditor::backgroundColourId, Colour (0x00000000));

    addAndMakeVisible (settings_loc_ = new Label ("new label",
                                                  TRANS("(undefined)")));
    settings_loc_->setFont (Font (15.00f, Font::plain).withTypefaceStyle ("Regular"));
    settings_loc_->setJustificationType (Justification::centredLeft);
    settings_loc_->setEditable (false, false, false);
    settings_loc_->setColour (TextEditor::textColourId, Colours::black);
    settings_loc_->setColour (TextEditor::backgroundColourId, Colour (0x00000000));

    addAndMakeVisible (settings_loc_open_ = new TextButton ("new button"));
    settings_loc_open_->setButtonText (TRANS("Open"));
    settings_loc_open_->addListener (this);


    //[UserPreSize]
    //[/UserPreSize]

    setSize (600, 400);


    //[Constructor] You can add your own custom stuff here..
    settings_loc_->setText({}, juce::NotificationType::dontSendNotification);
    host::getApp().addChangeListener(this);
    //[/Constructor]
}

ConfigWindowApp::~ConfigWindowApp()
{
    //[Destructor_pre]. You can add your own custom destruction code here..
    //[/Destructor_pre]

    settings_loc_label_ = nullptr;
    settings_loc_ = nullptr;
    settings_loc_open_ = nullptr;


    //[Destructor]. You can add your own custom destruction code here..
    //[/Destructor]
}

//==============================================================================
void ConfigWindowApp::paint (Graphics& g)
{
    //[UserPrePaint] Add your own custom painting code here..
    //[/UserPrePaint]

    g.fillAll (Colour (0xff323e44));

    //[UserPaint] Add your own custom painting code here..
    //[/UserPaint]
}

void ConfigWindowApp::resized()
{
    //[UserPreResize] Add your own custom resize code here..
    //[/UserPreResize]

    settings_loc_label_->setBounds (proportionOfWidth (0.0055f), proportionOfHeight (0.0082f), 224, 24);
    settings_loc_->setBounds (proportionOfWidth (0.0055f) + 224, proportionOfHeight (0.0082f) + 0, 224, 24);
    settings_loc_open_->setBounds ((proportionOfWidth (0.0055f) + 224) + 224 - -42, (proportionOfHeight (0.0082f) + 0) + 24 / 2 - (24 / 2), 64, 24);
    //[UserResized] Add your own custom resize handling here..
    //[/UserResized]
}

void ConfigWindowApp::buttonClicked (Button* buttonThatWasClicked)
{
    //[UserbuttonClicked_Pre]
    //[/UserbuttonClicked_Pre]

    if (buttonThatWasClicked == settings_loc_open_)
    {
        //[UserButtonCode_settings_loc_open_] -- add your button handler code here..
        juce::File{(host::getApp().config_path() / ".").string()}.revealToUser();
        //[/UserButtonCode_settings_loc_open_]
    }

    //[UserbuttonClicked_Post]
    //[/UserbuttonClicked_Post]
}



//[MiscUserCode] You can add your own definitions of your custom methods or any other code here...
void ConfigWindowApp::changeListenerCallback(juce::ChangeBroadcaster* const src)
{
    auto const& host_app = host::getApp();

    if (src == std::addressof(host_app)) {
        settings_loc_->setText(host_app.config_path().string(), juce::dontSendNotification);
    }
}

}}} // ompu
//[/MiscUserCode]


//==============================================================================
#if 0
/*  -- Projucer information section --

    This is where the Projucer stores the metadata that describe this GUI layout, so
    make changes in here at your peril!

BEGIN_JUCER_METADATA

<JUCER_COMPONENT documentType="Component" className="ConfigWindowApp" componentName="ConfigWindowApp"
                 parentClasses="public ompu::plug::base::AppComponent&lt;juce::Component&gt;, public juce::ChangeListener"
                 constructorParams="App&amp; app_" variableInitialisers="AppComponent(app_)"
                 snapPixels="8" snapActive="1" snapShown="1" overlayOpacity="0.330"
                 fixedSize="0" initialWidth="600" initialHeight="400">
  <BACKGROUND backgroundColour="ff323e44"/>
  <LABEL name="new label" id="ac70aa6aa5ee03a6" memberName="settings_loc_label_"
         virtualName="" explicitFocusOrder="0" pos="0.547% 0.82% 224 24"
         edTextCol="ff000000" edBkgCol="0" labelText="Application settings location"
         editableSingleClick="0" editableDoubleClick="0" focusDiscardsChanges="0"
         fontname="Default font" fontsize="15" kerning="0" bold="0" italic="0"
         justification="33"/>
  <LABEL name="new label" id="3aa4fa622096037e" memberName="settings_loc_"
         virtualName="" explicitFocusOrder="0" pos="0R 0 224 24" posRelativeX="ac70aa6aa5ee03a6"
         posRelativeY="ac70aa6aa5ee03a6" edTextCol="ff000000" edBkgCol="0"
         labelText="(undefined)" editableSingleClick="0" editableDoubleClick="0"
         focusDiscardsChanges="0" fontname="Default font" fontsize="15"
         kerning="0" bold="0" italic="0" justification="33"/>
  <TEXTBUTTON name="new button" id="dee76baff16d4a6a" memberName="settings_loc_open_"
              virtualName="" explicitFocusOrder="0" pos="-42R 0Cc 64 24" posRelativeX="3aa4fa622096037e"
              posRelativeY="3aa4fa622096037e" buttonText="Open" connectedEdges="0"
              needsCallback="1" radioGroupId="0"/>
</JUCER_COMPONENT>

END_JUCER_METADATA
*/
#endif


//[EndFile] You can add extra defines here...
//[/EndFile]
