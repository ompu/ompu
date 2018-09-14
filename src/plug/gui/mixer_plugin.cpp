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

#include "mixer_plugin.h"


//[MiscUserDefs] You can add your own user definitions and misc code here...
namespace ompu { namespace plug { namespace gui {
//[/MiscUserDefs]

//==============================================================================
MixerPlugin::MixerPlugin (App& app_)
    : AppComponent(app_)
{
    //[Constructor_pre] You can add your own custom stuff here..
    //[/Constructor_pre]

    setName ("MixerPlugin");
    addAndMakeVisible (name_ = new Label ("new label",
                                          TRANS("(unnamed)")));
    name_->setFont (Font (Font::getDefaultSansSerifFontName(), 14.00f, Font::plain).withTypefaceStyle ("Regular"));
    name_->setJustificationType (Justification::centredLeft);
    name_->setEditable (false, false, false);
    name_->setColour (Label::backgroundColourId, Colour (0xff323e44));
    name_->setColour (TextEditor::textColourId, Colours::black);
    name_->setColour (TextEditor::backgroundColourId, Colour (0x00000000));

    addAndMakeVisible (power_ = new ToggleButton ("new toggle button"));
    power_->setButtonText (String());
    power_->addListener (this);


    //[UserPreSize]
    //[/UserPreSize]

    setSize (120, 28);


    //[Constructor] You can add your own custom stuff here..
    //[/Constructor]
}

MixerPlugin::~MixerPlugin()
{
    //[Destructor_pre]. You can add your own custom destruction code here..
    //[/Destructor_pre]

    name_ = nullptr;
    power_ = nullptr;


    //[Destructor]. You can add your own custom destruction code here..
    //[/Destructor]
}

//==============================================================================
void MixerPlugin::paint (Graphics& g)
{
    //[UserPrePaint] Add your own custom painting code here..
    //[/UserPrePaint]

    g.fillAll (Colour (0xff323e44));

    //[UserPaint] Add your own custom painting code here..
    //[/UserPaint]
}

void MixerPlugin::resized()
{
    //[UserPreResize] Add your own custom resize code here..
    //[/UserPreResize]

    name_->setBounds (proportionOfWidth (0.1833f), proportionOfHeight (0.0000f), proportionOfWidth (0.8167f), proportionOfHeight (1.0000f));
    power_->setBounds (proportionOfWidth (0.0333f), proportionOfHeight (0.5000f) - (proportionOfHeight (0.7857f) / 2), proportionOfWidth (0.1500f), proportionOfHeight (0.7857f));
    //[UserResized] Add your own custom resize handling here..
    //[/UserResized]
}

void MixerPlugin::buttonClicked (Button* buttonThatWasClicked)
{
    //[UserbuttonClicked_Pre]
    //[/UserbuttonClicked_Pre]

    if (buttonThatWasClicked == power_)
    {
        //[UserButtonCode_power_] -- add your button handler code here..
        //[/UserButtonCode_power_]
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

<JUCER_COMPONENT documentType="Component" className="MixerPlugin" componentName="MixerPlugin"
                 parentClasses="public ompu::plug::base::AppComponent&lt;juce::Component&gt;"
                 constructorParams="App&amp; app_" variableInitialisers="AppComponent(app_)"
                 snapPixels="8" snapActive="1" snapShown="1" overlayOpacity="0.330"
                 fixedSize="1" initialWidth="120" initialHeight="28">
  <BACKGROUND backgroundColour="ff323e44"/>
  <LABEL name="new label" id="81e9c61756cc2c63" memberName="name_" virtualName=""
         explicitFocusOrder="0" pos="18.333% 0% 81.667% 100%" bkgCol="ff323e44"
         edTextCol="ff000000" edBkgCol="0" labelText="(unnamed)" editableSingleClick="0"
         editableDoubleClick="0" focusDiscardsChanges="0" fontname="Default sans-serif font"
         fontsize="14" kerning="0" bold="0" italic="0" justification="33"/>
  <TOGGLEBUTTON name="new toggle button" id="d18ccebbb403862" memberName="power_"
                virtualName="" explicitFocusOrder="0" pos="3.333% 50%c 15% 78.571%"
                buttonText="" connectedEdges="0" needsCallback="1" radioGroupId="0"
                state="0"/>
</JUCER_COMPONENT>

END_JUCER_METADATA
*/
#endif


//[EndFile] You can add extra defines here...
//[/EndFile]
