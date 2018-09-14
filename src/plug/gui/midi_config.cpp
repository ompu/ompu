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
#include "ompu/plug/app.hpp"
#include "ompu/midi/filter.hpp"

#include <boost/range/adaptor/indexed.hpp>

#include <boost/preprocessor/arithmetic/add.hpp>
#include <boost/preprocessor/repeat.hpp>
#include <boost/preprocessor/cat.hpp>
//[/Headers]

#include "midi_config.h"


//[MiscUserDefs] You can add your own user definitions and misc code here...
namespace ompu { namespace plug { namespace gui {
//[/MiscUserDefs]

//==============================================================================
MIDIConfig::MIDIConfig (App& app_, ComponentID id)
    : AppComponent(app_), id_(std::move(id))
{
    //[Constructor_pre] You can add your own custom stuff here..
    //[/Constructor_pre]

    setName ("MIDIConfig");
    addAndMakeVisible (ch_1_ = new ToggleButton ("1"));
    ch_1_->setButtonText (String());
    ch_1_->addListener (this);
    ch_1_->setToggleState (true, dontSendNotification);

    addAndMakeVisible (ch_2_ = new ToggleButton ("2"));
    ch_2_->setButtonText (String());
    ch_2_->addListener (this);
    ch_2_->setToggleState (true, dontSendNotification);

    addAndMakeVisible (ch_3_ = new ToggleButton ("3"));
    ch_3_->setButtonText (String());
    ch_3_->addListener (this);
    ch_3_->setToggleState (true, dontSendNotification);

    addAndMakeVisible (ch_4_ = new ToggleButton ("4"));
    ch_4_->setButtonText (String());
    ch_4_->addListener (this);
    ch_4_->setToggleState (true, dontSendNotification);

    addAndMakeVisible (ch_5_ = new ToggleButton ("5"));
    ch_5_->setButtonText (String());
    ch_5_->addListener (this);
    ch_5_->setToggleState (true, dontSendNotification);

    addAndMakeVisible (ch_6_ = new ToggleButton ("6"));
    ch_6_->setButtonText (String());
    ch_6_->addListener (this);
    ch_6_->setToggleState (true, dontSendNotification);

    addAndMakeVisible (ch_7_ = new ToggleButton ("7"));
    ch_7_->setButtonText (String());
    ch_7_->addListener (this);
    ch_7_->setToggleState (true, dontSendNotification);

    addAndMakeVisible (ch_8_ = new ToggleButton ("8"));
    ch_8_->setButtonText (String());
    ch_8_->addListener (this);
    ch_8_->setToggleState (true, dontSendNotification);

    addAndMakeVisible (ch_9_ = new ToggleButton ("9"));
    ch_9_->setButtonText (String());
    ch_9_->addListener (this);
    ch_9_->setToggleState (true, dontSendNotification);

    addAndMakeVisible (ch_10_ = new ToggleButton ("10"));
    ch_10_->setButtonText (String());
    ch_10_->addListener (this);
    ch_10_->setToggleState (true, dontSendNotification);

    addAndMakeVisible (ch_11_ = new ToggleButton ("11"));
    ch_11_->setButtonText (String());
    ch_11_->addListener (this);
    ch_11_->setToggleState (true, dontSendNotification);

    addAndMakeVisible (ch_12_ = new ToggleButton ("12"));
    ch_12_->setButtonText (String());
    ch_12_->addListener (this);
    ch_12_->setToggleState (true, dontSendNotification);

    addAndMakeVisible (ch_13_ = new ToggleButton ("13"));
    ch_13_->setButtonText (String());
    ch_13_->addListener (this);
    ch_13_->setToggleState (true, dontSendNotification);

    addAndMakeVisible (ch_14_ = new ToggleButton ("14"));
    ch_14_->setButtonText (String());
    ch_14_->addListener (this);
    ch_14_->setToggleState (true, dontSendNotification);

    addAndMakeVisible (ch_15_ = new ToggleButton ("15"));
    ch_15_->setButtonText (String());
    ch_15_->addListener (this);
    ch_15_->setToggleState (true, dontSendNotification);

    addAndMakeVisible (ch_16_ = new ToggleButton ("16"));
    ch_16_->setButtonText (String());
    ch_16_->addListener (this);
    ch_16_->setToggleState (true, dontSendNotification);

    addAndMakeVisible (name_ = new ToggleButton ("new toggle button"));
    name_->setButtonText (TRANS("(invalid device)"));
    name_->addListener (this);


    //[UserPreSize]
    //chs_.reserve(OMPU_MIDI_CH_MAX);

#define OMPU_DEF(z, i, data) \
    chs_[i] = this-> BOOST_PP_CAT(BOOST_PP_CAT(ch_, BOOST_PP_ADD(i, 1)), _) .get();

    BOOST_PP_REPEAT(OMPU_MIDI_CH_MAX, OMPU_DEF, _)
#undef OMPU_DEF
    //[/UserPreSize]

    setSize (600, 400);


    //[Constructor] You can add your own custom stuff here..
    name_->setButtonText(juce::MidiInput::getDevices()[id.index()]);
    //[/Constructor]
}

MIDIConfig::~MIDIConfig()
{
    //[Destructor_pre]. You can add your own custom destruction code here..
    //[/Destructor_pre]

    ch_1_ = nullptr;
    ch_2_ = nullptr;
    ch_3_ = nullptr;
    ch_4_ = nullptr;
    ch_5_ = nullptr;
    ch_6_ = nullptr;
    ch_7_ = nullptr;
    ch_8_ = nullptr;
    ch_9_ = nullptr;
    ch_10_ = nullptr;
    ch_11_ = nullptr;
    ch_12_ = nullptr;
    ch_13_ = nullptr;
    ch_14_ = nullptr;
    ch_15_ = nullptr;
    ch_16_ = nullptr;
    name_ = nullptr;


    //[Destructor]. You can add your own custom destruction code here..
    //[/Destructor]
}

//==============================================================================
void MIDIConfig::paint (Graphics& g)
{
    //[UserPrePaint] Add your own custom painting code here..
    boost::ignore_unused(g);
    //[/UserPrePaint]

    g.fillAll (Colour (0xff323e44));

    //[UserPaint] Add your own custom painting code here..
    //[/UserPaint]
}

void MIDIConfig::resized()
{
    //[UserPreResize] Add your own custom resize code here..
    //[/UserPreResize]

    ch_1_->setBounds (152, 0, 28, 22);
    ch_2_->setBounds (152 + 16, 0 + 0, roundFloatToInt (28 * 1.0000f), roundFloatToInt (22 * 1.0000f));
    ch_3_->setBounds (152 + 32, 0 + 0, roundFloatToInt (28 * 1.0000f), roundFloatToInt (22 * 1.0000f));
    ch_4_->setBounds (152 + 48, 0 + 0, roundFloatToInt (28 * 1.0000f), roundFloatToInt (22 * 1.0000f));
    ch_5_->setBounds (152 + 72, 0 + 0, roundFloatToInt (28 * 1.0000f), roundFloatToInt (22 * 1.0000f));
    ch_6_->setBounds (152 + 88, 0 + 0, roundFloatToInt (28 * 1.0000f), roundFloatToInt (22 * 1.0000f));
    ch_7_->setBounds (152 + 104, 0 + 0, roundFloatToInt (28 * 1.0000f), roundFloatToInt (22 * 1.0000f));
    ch_8_->setBounds (152 + 120, 0 + 0, roundFloatToInt (28 * 1.0000f), roundFloatToInt (22 * 1.0000f));
    ch_9_->setBounds (152 + 144, 0 + 0, roundFloatToInt (28 * 1.0000f), roundFloatToInt (22 * 1.0000f));
    ch_10_->setBounds (152 + 160, 0 + 0, roundFloatToInt (28 * 1.0000f), roundFloatToInt (22 * 1.0000f));
    ch_11_->setBounds (152 + 176, 0 + 0, roundFloatToInt (28 * 1.0000f), roundFloatToInt (22 * 1.0000f));
    ch_12_->setBounds (152 + 192, 0 + 0, roundFloatToInt (28 * 1.0000f), roundFloatToInt (22 * 1.0000f));
    ch_13_->setBounds (152 + 216, 0 + 0, roundFloatToInt (28 * 1.0000f), roundFloatToInt (22 * 1.0000f));
    ch_14_->setBounds (152 + 232, 0 + 0, roundFloatToInt (28 * 1.0000f), roundFloatToInt (22 * 1.0000f));
    ch_15_->setBounds (152 + 248, 0 + 0, roundFloatToInt (28 * 1.0000f), roundFloatToInt (22 * 1.0000f));
    ch_16_->setBounds (152 + 264, 0 + 0, roundFloatToInt (28 * 1.0000f), roundFloatToInt (22 * 1.0000f));
    name_->setBounds (0, 0, 150, 22);
    //[UserResized] Add your own custom resize handling here..
    //[/UserResized]
}

void MIDIConfig::buttonClicked (Button* buttonThatWasClicked)
{
    //[UserbuttonClicked_Pre]
    //[/UserbuttonClicked_Pre]

    if (buttonThatWasClicked == ch_1_)
    {
        //[UserButtonCode_ch_1_] -- add your button handler code here..
        //[/UserButtonCode_ch_1_]
    }
    else if (buttonThatWasClicked == ch_2_)
    {
        //[UserButtonCode_ch_2_] -- add your button handler code here..
        //[/UserButtonCode_ch_2_]
    }
    else if (buttonThatWasClicked == ch_3_)
    {
        //[UserButtonCode_ch_3_] -- add your button handler code here..
        //[/UserButtonCode_ch_3_]
    }
    else if (buttonThatWasClicked == ch_4_)
    {
        //[UserButtonCode_ch_4_] -- add your button handler code here..
        //[/UserButtonCode_ch_4_]
    }
    else if (buttonThatWasClicked == ch_5_)
    {
        //[UserButtonCode_ch_5_] -- add your button handler code here..
        //[/UserButtonCode_ch_5_]
    }
    else if (buttonThatWasClicked == ch_6_)
    {
        //[UserButtonCode_ch_6_] -- add your button handler code here..
        //[/UserButtonCode_ch_6_]
    }
    else if (buttonThatWasClicked == ch_7_)
    {
        //[UserButtonCode_ch_7_] -- add your button handler code here..
        //[/UserButtonCode_ch_7_]
    }
    else if (buttonThatWasClicked == ch_8_)
    {
        //[UserButtonCode_ch_8_] -- add your button handler code here..
        //[/UserButtonCode_ch_8_]
    }
    else if (buttonThatWasClicked == ch_9_)
    {
        //[UserButtonCode_ch_9_] -- add your button handler code here..
        //[/UserButtonCode_ch_9_]
    }
    else if (buttonThatWasClicked == ch_10_)
    {
        //[UserButtonCode_ch_10_] -- add your button handler code here..
        //[/UserButtonCode_ch_10_]
    }
    else if (buttonThatWasClicked == ch_11_)
    {
        //[UserButtonCode_ch_11_] -- add your button handler code here..
        //[/UserButtonCode_ch_11_]
    }
    else if (buttonThatWasClicked == ch_12_)
    {
        //[UserButtonCode_ch_12_] -- add your button handler code here..
        //[/UserButtonCode_ch_12_]
    }
    else if (buttonThatWasClicked == ch_13_)
    {
        //[UserButtonCode_ch_13_] -- add your button handler code here..
        //[/UserButtonCode_ch_13_]
    }
    else if (buttonThatWasClicked == ch_14_)
    {
        //[UserButtonCode_ch_14_] -- add your button handler code here..
        //[/UserButtonCode_ch_14_]
    }
    else if (buttonThatWasClicked == ch_15_)
    {
        //[UserButtonCode_ch_15_] -- add your button handler code here..
        //[/UserButtonCode_ch_15_]
    }
    else if (buttonThatWasClicked == ch_16_)
    {
        //[UserButtonCode_ch_16_] -- add your button handler code here..
        //[/UserButtonCode_ch_16_]
    }
    else if (buttonThatWasClicked == name_)
    {
        //[UserButtonCode_name_] -- add your button handler code here..
        ompu::midi::Filter filter;

        for (auto const& ch : chs_ | boost::adaptors::indexed(0)) {
            filter.channel[ch.index()] = !ch.value()->isEnabled();
        }

        app().set_midi_input(name_->isEnabled(), id_, std::move(filter));
        //[/UserButtonCode_name_]
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

<JUCER_COMPONENT documentType="Component" className="MIDIConfig" componentName="MIDIConfig"
                 parentClasses="public ompu::plug::base::AppComponent&lt;&gt;"
                 constructorParams="App&amp; app_, ComponentID id" variableInitialisers="AppComponent(app_), id_(std::move(id))"
                 snapPixels="8" snapActive="1" snapShown="1" overlayOpacity="0.330"
                 fixedSize="1" initialWidth="600" initialHeight="400">
  <BACKGROUND backgroundColour="ff323e44"/>
  <TOGGLEBUTTON name="1" id="c113f2028cc97575" memberName="ch_1_" virtualName=""
                explicitFocusOrder="0" pos="152 0 28 22" buttonText="" connectedEdges="0"
                needsCallback="1" radioGroupId="0" state="1"/>
  <TOGGLEBUTTON name="2" id="5aaee45f44d081e0" memberName="ch_2_" virtualName=""
                explicitFocusOrder="0" pos="16 0 100% 100%" posRelativeX="c113f2028cc97575"
                posRelativeY="c113f2028cc97575" posRelativeW="c113f2028cc97575"
                posRelativeH="c113f2028cc97575" buttonText="" connectedEdges="0"
                needsCallback="1" radioGroupId="0" state="1"/>
  <TOGGLEBUTTON name="3" id="e8f7e5108782305d" memberName="ch_3_" virtualName=""
                explicitFocusOrder="0" pos="32 0 100% 100%" posRelativeX="c113f2028cc97575"
                posRelativeY="c113f2028cc97575" posRelativeW="c113f2028cc97575"
                posRelativeH="c113f2028cc97575" buttonText="" connectedEdges="0"
                needsCallback="1" radioGroupId="0" state="1"/>
  <TOGGLEBUTTON name="4" id="646993cd3eab5515" memberName="ch_4_" virtualName=""
                explicitFocusOrder="0" pos="48 0 100% 100%" posRelativeX="c113f2028cc97575"
                posRelativeY="c113f2028cc97575" posRelativeW="c113f2028cc97575"
                posRelativeH="c113f2028cc97575" buttonText="" connectedEdges="0"
                needsCallback="1" radioGroupId="0" state="1"/>
  <TOGGLEBUTTON name="5" id="a8737dd4c7145309" memberName="ch_5_" virtualName=""
                explicitFocusOrder="0" pos="72 0 100% 100%" posRelativeX="c113f2028cc97575"
                posRelativeY="c113f2028cc97575" posRelativeW="c113f2028cc97575"
                posRelativeH="c113f2028cc97575" buttonText="" connectedEdges="0"
                needsCallback="1" radioGroupId="0" state="1"/>
  <TOGGLEBUTTON name="6" id="66365c1a6e41bd06" memberName="ch_6_" virtualName=""
                explicitFocusOrder="0" pos="88 0 100% 100%" posRelativeX="c113f2028cc97575"
                posRelativeY="c113f2028cc97575" posRelativeW="c113f2028cc97575"
                posRelativeH="c113f2028cc97575" buttonText="" connectedEdges="0"
                needsCallback="1" radioGroupId="0" state="1"/>
  <TOGGLEBUTTON name="7" id="3f7aa9dc6e0a5558" memberName="ch_7_" virtualName=""
                explicitFocusOrder="0" pos="104 0 100% 100%" posRelativeX="c113f2028cc97575"
                posRelativeY="c113f2028cc97575" posRelativeW="c113f2028cc97575"
                posRelativeH="c113f2028cc97575" buttonText="" connectedEdges="0"
                needsCallback="1" radioGroupId="0" state="1"/>
  <TOGGLEBUTTON name="8" id="a79dc952297fa847" memberName="ch_8_" virtualName=""
                explicitFocusOrder="0" pos="120 0 100% 100%" posRelativeX="c113f2028cc97575"
                posRelativeY="c113f2028cc97575" posRelativeW="c113f2028cc97575"
                posRelativeH="c113f2028cc97575" buttonText="" connectedEdges="0"
                needsCallback="1" radioGroupId="0" state="1"/>
  <TOGGLEBUTTON name="9" id="c0e5f339c3a7d53a" memberName="ch_9_" virtualName=""
                explicitFocusOrder="0" pos="144 0 100% 100%" posRelativeX="c113f2028cc97575"
                posRelativeY="c113f2028cc97575" posRelativeW="c113f2028cc97575"
                posRelativeH="c113f2028cc97575" buttonText="" connectedEdges="0"
                needsCallback="1" radioGroupId="0" state="1"/>
  <TOGGLEBUTTON name="10" id="be296add8400779b" memberName="ch_10_" virtualName=""
                explicitFocusOrder="0" pos="160 0 100% 100%" posRelativeX="c113f2028cc97575"
                posRelativeY="c113f2028cc97575" posRelativeW="c113f2028cc97575"
                posRelativeH="c113f2028cc97575" buttonText="" connectedEdges="0"
                needsCallback="1" radioGroupId="0" state="1"/>
  <TOGGLEBUTTON name="11" id="8263e45f7b997138" memberName="ch_11_" virtualName=""
                explicitFocusOrder="0" pos="176 0 100% 100%" posRelativeX="c113f2028cc97575"
                posRelativeY="c113f2028cc97575" posRelativeW="c113f2028cc97575"
                posRelativeH="c113f2028cc97575" buttonText="" connectedEdges="0"
                needsCallback="1" radioGroupId="0" state="1"/>
  <TOGGLEBUTTON name="12" id="3f7081e74746645d" memberName="ch_12_" virtualName=""
                explicitFocusOrder="0" pos="192 0 100% 100%" posRelativeX="c113f2028cc97575"
                posRelativeY="c113f2028cc97575" posRelativeW="c113f2028cc97575"
                posRelativeH="c113f2028cc97575" buttonText="" connectedEdges="0"
                needsCallback="1" radioGroupId="0" state="1"/>
  <TOGGLEBUTTON name="13" id="4ed759ce65c57ac" memberName="ch_13_" virtualName=""
                explicitFocusOrder="0" pos="216 0 100% 100%" posRelativeX="c113f2028cc97575"
                posRelativeY="c113f2028cc97575" posRelativeW="c113f2028cc97575"
                posRelativeH="c113f2028cc97575" buttonText="" connectedEdges="0"
                needsCallback="1" radioGroupId="0" state="1"/>
  <TOGGLEBUTTON name="14" id="bbbe6abf53681fd8" memberName="ch_14_" virtualName=""
                explicitFocusOrder="0" pos="232 0 100% 100%" posRelativeX="c113f2028cc97575"
                posRelativeY="c113f2028cc97575" posRelativeW="c113f2028cc97575"
                posRelativeH="c113f2028cc97575" buttonText="" connectedEdges="0"
                needsCallback="1" radioGroupId="0" state="1"/>
  <TOGGLEBUTTON name="15" id="291d8e56cee5d1c" memberName="ch_15_" virtualName=""
                explicitFocusOrder="0" pos="248 0 100% 100%" posRelativeX="c113f2028cc97575"
                posRelativeY="c113f2028cc97575" posRelativeW="c113f2028cc97575"
                posRelativeH="c113f2028cc97575" buttonText="" connectedEdges="0"
                needsCallback="1" radioGroupId="0" state="1"/>
  <TOGGLEBUTTON name="16" id="4f53c36cf24fa6e0" memberName="ch_16_" virtualName=""
                explicitFocusOrder="0" pos="264 0 100% 100%" posRelativeX="c113f2028cc97575"
                posRelativeY="c113f2028cc97575" posRelativeW="c113f2028cc97575"
                posRelativeH="c113f2028cc97575" buttonText="" connectedEdges="0"
                needsCallback="1" radioGroupId="0" state="1"/>
  <TOGGLEBUTTON name="new toggle button" id="3a9ea107fa0da6ad" memberName="name_"
                virtualName="" explicitFocusOrder="0" pos="0 0 150 22" buttonText="(invalid device)"
                connectedEdges="0" needsCallback="1" radioGroupId="0" state="0"/>
</JUCER_COMPONENT>

END_JUCER_METADATA
*/
#endif


//[EndFile] You can add extra defines here...
//[/EndFile]
