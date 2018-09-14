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
#include "midi_channel_list.h"
//[/Headers]

#include "config_window_midi.h"


//[MiscUserDefs] You can add your own user definitions and misc code here...
namespace ompu { namespace plug { namespace gui {
//[/MiscUserDefs]

//==============================================================================
ConfigWindowMIDI::ConfigWindowMIDI (App& app_)
    : AppComponent(app_)
{
    //[Constructor_pre] You can add your own custom stuff here..
    //[/Constructor_pre]

    setName ("ConfigWindowMIDI");
    addAndMakeVisible (label3 = new Label ("new label",
                                           TRANS("MIDI output:")));
    label3->setFont (Font (15.00f, Font::plain).withTypefaceStyle ("Regular"));
    label3->setJustificationType (Justification::centredLeft);
    label3->setEditable (false, false, false);
    label3->setColour (TextEditor::textColourId, Colours::black);
    label3->setColour (TextEditor::backgroundColourId, Colour (0x00000000));

    addAndMakeVisible (midi_out_ = new ComboBox ("new combo box"));
    midi_out_->setEditableText (false);
    midi_out_->setJustificationType (Justification::centredLeft);
    midi_out_->setTextWhenNothingSelected (String());
    midi_out_->setTextWhenNoChoicesAvailable (TRANS("(no choices)"));
    midi_out_->addListener (this);

    addAndMakeVisible (label2 = new Label ("new label",
                                           TRANS("MIDI input:")));
    label2->setFont (Font (15.00f, Font::plain).withTypefaceStyle ("Regular"));
    label2->setJustificationType (Justification::centredLeft);
    label2->setEditable (false, false, false);
    label2->setColour (TextEditor::textColourId, Colours::black);
    label2->setColour (TextEditor::backgroundColourId, Colour (0x00000000));

    addAndMakeVisible (midi_ins_ = new Viewport ("new viewport"));
    midi_ins_->setScrollBarsShown (true, false);
    midi_ins_->setScrollBarThickness (10);
    midi_ins_->setViewedComponent (new ompu::plug::gui::MIDIChannelList (app_));


    //[UserPreSize]
    //[/UserPreSize]

    setSize (600, 400);


    //[Constructor] You can add your own custom stuff here..
    dm().addChangeListener(this);
    //[/Constructor]
}

ConfigWindowMIDI::~ConfigWindowMIDI()
{
    //[Destructor_pre]. You can add your own custom destruction code here..
    //[/Destructor_pre]

    label3 = nullptr;
    midi_out_ = nullptr;
    label2 = nullptr;
    midi_ins_ = nullptr;


    //[Destructor]. You can add your own custom destruction code here..
    //[/Destructor]
}

//==============================================================================
void ConfigWindowMIDI::paint (Graphics& g)
{
    //[UserPrePaint] Add your own custom painting code here..
    //[/UserPrePaint]

    g.fillAll (Colour (0xff323e44));

    //[UserPaint] Add your own custom painting code here..
    //[/UserPaint]
}

void ConfigWindowMIDI::resized()
{
    //[UserPreResize] Add your own custom resize code here..
    //[/UserPreResize]

    label3->setBounds (8, 192, 150, 24);
    midi_out_->setBounds (16, 216, 240, 24);
    label2->setBounds (8, 8, 150, 24);
    midi_ins_->setBounds (16, 32, 472, 152);
    //[UserResized] Add your own custom resize handling here..
    //[/UserResized]
}

void ConfigWindowMIDI::comboBoxChanged (ComboBox* comboBoxThatHasChanged)
{
    //[UsercomboBoxChanged_Pre]
    //[/UsercomboBoxChanged_Pre]

    if (comboBoxThatHasChanged == midi_out_)
    {
        //[UserComboBoxCode_midi_out_] -- add your combo box handling code here..
        //[/UserComboBoxCode_midi_out_]
    }

    //[UsercomboBoxChanged_Post]
    //[/UsercomboBoxChanged_Post]
}



//[MiscUserCode] You can add your own definitions of your custom methods or any other code here...
void ConfigWindowMIDI::changeListenerCallback(juce::ChangeBroadcaster* const src)
{
    if (src != std::addressof(dm())) return;
}
}}} // ompu
//[/MiscUserCode]


//==============================================================================
#if 0
/*  -- Projucer information section --

    This is where the Projucer stores the metadata that describe this GUI layout, so
    make changes in here at your peril!

BEGIN_JUCER_METADATA

<JUCER_COMPONENT documentType="Component" className="ConfigWindowMIDI" componentName="ConfigWindowMIDI"
                 parentClasses="public juce::ChangeListener, public ompu::plug::base::AppComponent&lt;juce::Component&gt;"
                 constructorParams="App&amp; app_" variableInitialisers="AppComponent(app_)"
                 snapPixels="8" snapActive="1" snapShown="1" overlayOpacity="0.330"
                 fixedSize="0" initialWidth="600" initialHeight="400">
  <BACKGROUND backgroundColour="ff323e44"/>
  <LABEL name="new label" id="780e662dc6161216" memberName="label3" virtualName=""
         explicitFocusOrder="0" pos="8 192 150 24" edTextCol="ff000000"
         edBkgCol="0" labelText="MIDI output:" editableSingleClick="0"
         editableDoubleClick="0" focusDiscardsChanges="0" fontname="Default font"
         fontsize="15" kerning="0" bold="0" italic="0" justification="33"/>
  <COMBOBOX name="new combo box" id="28eab431dda1843b" memberName="midi_out_"
            virtualName="" explicitFocusOrder="0" pos="16 216 240 24" editable="0"
            layout="33" items="" textWhenNonSelected="" textWhenNoItems="(no choices)"/>
  <LABEL name="new label" id="f37dff64cda501e9" memberName="label2" virtualName=""
         explicitFocusOrder="0" pos="8 8 150 24" edTextCol="ff000000"
         edBkgCol="0" labelText="MIDI input:" editableSingleClick="0"
         editableDoubleClick="0" focusDiscardsChanges="0" fontname="Default font"
         fontsize="15" kerning="0" bold="0" italic="0" justification="33"/>
  <VIEWPORT name="new viewport" id="7c18345b89cf53b4" memberName="midi_ins_"
            virtualName="" explicitFocusOrder="0" pos="16 32 472 152" vscroll="1"
            hscroll="0" scrollbarThickness="10" contentType="2" jucerFile=""
            contentClass="ompu::plug::gui::MIDIChannelList" constructorParams="app_"/>
</JUCER_COMPONENT>

END_JUCER_METADATA
*/
#endif


//[EndFile] You can add extra defines here...
//[/EndFile]
