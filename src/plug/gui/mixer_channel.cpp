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

#include "mixer_channel.h"


//[MiscUserDefs] You can add your own user definitions and misc code here...
namespace ompu { namespace plug { namespace gui {
//[/MiscUserDefs]

//==============================================================================
MixerChannel::MixerChannel (App& app_)
    : AppComponent(app_)
{
    //[Constructor_pre] You can add your own custom stuff here..
    //[/Constructor_pre]

    setName ("MixerChannel");
    addAndMakeVisible (sends_ = new Viewport ("new viewport"));
    sends_->setScrollBarsShown (true, false);

    addAndMakeVisible (fader_ = new Slider ("new slider"));
    fader_->setRange (-48, 6, 0.01);
    fader_->setSliderStyle (Slider::LinearVertical);
    fader_->setTextBoxStyle (Slider::TextBoxAbove, false, 80, 20);
    fader_->addListener (this);

    addAndMakeVisible (show_plugins_ = new TextButton ("new button"));
    show_plugins_->setButtonText (TRANS(">"));
    show_plugins_->addListener (this);

    addAndMakeVisible (pan_ = new Slider ("new slider"));
    pan_->setRange (-100, 100, 1);
    pan_->setSliderStyle (Slider::LinearHorizontal);
    pan_->setTextBoxStyle (Slider::NoTextBox, false, 80, 20);
    pan_->setColour (Slider::trackColourId, Colour (0xff263238));
    pan_->setColour (Slider::rotarySliderFillColourId, Colour (0xff263238));
    pan_->addListener (this);

    addAndMakeVisible (name_ = new Label ("new label",
                                          TRANS("Unnamed")));
    name_->setFont (Font (Font::getDefaultSansSerifFontName(), 15.00f, Font::plain).withTypefaceStyle ("Regular"));
    name_->setJustificationType (Justification::centred);
    name_->setEditable (false, true, false);
    name_->setColour (Label::backgroundColourId, Colour (0x2dffffff));
    name_->setColour (Label::outlineColourId, Colour (0xff303057));
    name_->setColour (TextEditor::textColourId, Colours::black);
    name_->setColour (TextEditor::backgroundColourId, Colour (0x2dffffff));
    name_->addListener (this);

    addAndMakeVisible (midi_in_ = new TextButton ("new button"));
    midi_in_->setButtonText (TRANS("(MIDI in)"));
    midi_in_->addListener (this);
    midi_in_->setColour (TextButton::buttonColourId, Colour (0xff89ad66));

    addAndMakeVisible (inserts_ = new Viewport ("new viewport"));
    inserts_->setScrollBarsShown (true, false);

    addAndMakeVisible (label = new Label ("new label",
                                          TRANS("-----------------")));
    label->setFont (Font (Font::getDefaultSansSerifFontName(), 19.30f, Font::plain).withTypefaceStyle ("Regular"));
    label->setJustificationType (Justification::centred);
    label->setEditable (false, false, false);
    label->setColour (Label::textColourId, Colour (0xff276a7e));
    label->setColour (TextEditor::textColourId, Colours::black);
    label->setColour (TextEditor::backgroundColourId, Colour (0x00000000));


    //[UserPreSize]
    //[/UserPreSize]

    setSize (220, 320);


    //[Constructor] You can add your own custom stuff here..
    //[/Constructor]
}

MixerChannel::~MixerChannel()
{
    //[Destructor_pre]. You can add your own custom destruction code here..
    //[/Destructor_pre]

    sends_ = nullptr;
    fader_ = nullptr;
    show_plugins_ = nullptr;
    pan_ = nullptr;
    name_ = nullptr;
    midi_in_ = nullptr;
    inserts_ = nullptr;
    label = nullptr;


    //[Destructor]. You can add your own custom destruction code here..
    //[/Destructor]
}

//==============================================================================
void MixerChannel::paint (Graphics& g)
{
    //[UserPrePaint] Add your own custom painting code here..
    //[/UserPrePaint]

    g.fillAll (Colour (0xff323e44));

    //[UserPaint] Add your own custom painting code here..
    //[/UserPaint]
}

void MixerChannel::resized()
{
    //[UserPreResize] Add your own custom resize code here..
    //[/UserPreResize]

    sends_->setBounds (proportionOfWidth (0.3636f), proportionOfHeight (0.7500f), proportionOfWidth (0.6364f), proportionOfHeight (0.2500f));
    fader_->setBounds (proportionOfWidth (0.0500f), proportionOfHeight (0.1750f), proportionOfWidth (0.2546f), proportionOfHeight (0.7500f));
    show_plugins_->setBounds (proportionOfWidth (0.2546f), proportionOfHeight (0.8250f), proportionOfWidth (0.0727f), proportionOfHeight (0.0500f));
    pan_->setBounds (proportionOfWidth (0.0136f), proportionOfHeight (0.1000f), proportionOfWidth (0.3273f), proportionOfHeight (0.0750f));
    name_->setBounds (proportionOfWidth (0.0182f), proportionOfHeight (0.0156f), proportionOfWidth (0.3273f), proportionOfHeight (0.0750f));
    midi_in_->setBounds (proportionOfWidth (0.0227f), proportionOfHeight (0.9281f), proportionOfWidth (0.3091f), proportionOfHeight (0.0500f));
    inserts_->setBounds (proportionOfWidth (0.3636f), proportionOfHeight (0.0000f), proportionOfWidth (0.6364f), proportionOfHeight (0.7000f));
    label->setBounds (proportionOfWidth (0.4136f), proportionOfHeight (0.6844f), proportionOfWidth (0.5091f), proportionOfHeight (0.0750f));
    //[UserResized] Add your own custom resize handling here..
    //[/UserResized]
}

void MixerChannel::sliderValueChanged (Slider* sliderThatWasMoved)
{
    //[UsersliderValueChanged_Pre]
    //[/UsersliderValueChanged_Pre]

    if (sliderThatWasMoved == fader_)
    {
        //[UserSliderCode_fader_] -- add your slider handling code here..
        //[/UserSliderCode_fader_]
    }
    else if (sliderThatWasMoved == pan_)
    {
        //[UserSliderCode_pan_] -- add your slider handling code here..
        //[/UserSliderCode_pan_]
    }

    //[UsersliderValueChanged_Post]
    //[/UsersliderValueChanged_Post]
}

void MixerChannel::buttonClicked (Button* buttonThatWasClicked)
{
    //[UserbuttonClicked_Pre]
    //[/UserbuttonClicked_Pre]

    if (buttonThatWasClicked == show_plugins_)
    {
        //[UserButtonCode_show_plugins_] -- add your button handler code here..
        //[/UserButtonCode_show_plugins_]
    }
    else if (buttonThatWasClicked == midi_in_)
    {
        //[UserButtonCode_midi_in_] -- add your button handler code here..
        //[/UserButtonCode_midi_in_]
    }

    //[UserbuttonClicked_Post]
    //[/UserbuttonClicked_Post]
}

void MixerChannel::labelTextChanged (Label* labelThatHasChanged)
{
    //[UserlabelTextChanged_Pre]
    //[/UserlabelTextChanged_Pre]

    if (labelThatHasChanged == name_)
    {
        //[UserLabelCode_name_] -- add your label text handling code here..
        //[/UserLabelCode_name_]
    }

    //[UserlabelTextChanged_Post]
    //[/UserlabelTextChanged_Post]
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

<JUCER_COMPONENT documentType="Component" className="MixerChannel" componentName="MixerChannel"
                 parentClasses="public ompu::plug::base::AppComponent&lt;juce::Component&gt;"
                 constructorParams="App&amp; app_" variableInitialisers="AppComponent(app_)"
                 snapPixels="8" snapActive="1" snapShown="1" overlayOpacity="0.330"
                 fixedSize="1" initialWidth="220" initialHeight="320">
  <BACKGROUND backgroundColour="ff323e44"/>
  <VIEWPORT name="new viewport" id="9ec6038cca33b9a8" memberName="sends_"
            virtualName="" explicitFocusOrder="0" pos="36.364% 75% 63.636% 25%"
            vscroll="1" hscroll="0" scrollbarThickness="8" contentType="0"
            jucerFile="" contentClass="" constructorParams=""/>
  <SLIDER name="new slider" id="731079e894dcc613" memberName="fader_" virtualName=""
          explicitFocusOrder="0" pos="5% 17.5% 25.455% 75%" min="-48" max="6"
          int="0.010000000000000000208" style="LinearVertical" textBoxPos="TextBoxAbove"
          textBoxEditable="1" textBoxWidth="80" textBoxHeight="20" skewFactor="1"
          needsCallback="1"/>
  <TEXTBUTTON name="new button" id="a4dc95237f7d6ea8" memberName="show_plugins_"
              virtualName="" explicitFocusOrder="0" pos="25.455% 82.5% 7.273% 5%"
              buttonText="&gt;" connectedEdges="0" needsCallback="1" radioGroupId="0"/>
  <SLIDER name="new slider" id="35d5cdd2cfe806cd" memberName="pan_" virtualName=""
          explicitFocusOrder="0" pos="1.364% 10% 32.727% 7.5%" trackcol="ff263238"
          rotarysliderfill="ff263238" min="-100" max="100" int="1" style="LinearHorizontal"
          textBoxPos="NoTextBox" textBoxEditable="1" textBoxWidth="80"
          textBoxHeight="20" skewFactor="1" needsCallback="1"/>
  <LABEL name="new label" id="38cf911fbd11495f" memberName="name_" virtualName=""
         explicitFocusOrder="0" pos="1.818% 1.562% 32.727% 7.5%" bkgCol="2dffffff"
         outlineCol="ff303057" edTextCol="ff000000" edBkgCol="2dffffff"
         labelText="Unnamed" editableSingleClick="0" editableDoubleClick="1"
         focusDiscardsChanges="0" fontname="Default sans-serif font" fontsize="15"
         kerning="0" bold="0" italic="0" justification="36"/>
  <TEXTBUTTON name="new button" id="7932a00516ffec6a" memberName="midi_in_"
              virtualName="" explicitFocusOrder="0" pos="2.273% 92.812% 30.909% 5%"
              bgColOff="ff89ad66" buttonText="(MIDI in)" connectedEdges="0"
              needsCallback="1" radioGroupId="0"/>
  <VIEWPORT name="new viewport" id="51e5b98959ac8209" memberName="inserts_"
            virtualName="" explicitFocusOrder="0" pos="36.364% 0% 63.636% 70%"
            vscroll="1" hscroll="0" scrollbarThickness="8" contentType="0"
            jucerFile="" contentClass="" constructorParams=""/>
  <LABEL name="new label" id="6d1ef89698d246c9" memberName="label" virtualName=""
         explicitFocusOrder="0" pos="41.364% 68.438% 50.909% 7.5%" textCol="ff276a7e"
         edTextCol="ff000000" edBkgCol="0" labelText="-----------------"
         editableSingleClick="0" editableDoubleClick="0" focusDiscardsChanges="0"
         fontname="Default sans-serif font" fontsize="19.300000000000000711"
         kerning="0" bold="0" italic="0" justification="36"/>
</JUCER_COMPONENT>

END_JUCER_METADATA
*/
#endif


//[EndFile] You can add extra defines here...
//[/EndFile]
