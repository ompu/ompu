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
#include "ompu/ompu_lib.hpp"
//[/Headers]

#include "logo.h"


//[MiscUserDefs] You can add your own user definitions and misc code here...
namespace ompu { namespace plug { namespace gui {
//[/MiscUserDefs]

//==============================================================================
Logo::Logo (App& app_)
    : AppComponent(app_)
{
    //[Constructor_pre] You can add your own custom stuff here..
    //[/Constructor_pre]

    setName ("Logo");
    addAndMakeVisible (version_group_ = new GroupComponent ("new group",
                                                            TRANS("Version")));
    version_group_->setColour (GroupComponent::outlineColourId, Colour (0xff343839));
    version_group_->setColour (GroupComponent::textColourId, Colours::black);

    addAndMakeVisible (app_version_label_ = new Label ("app version",
                                                       TRANS("Application")));
    app_version_label_->setFont (Font (Font::getDefaultSansSerifFontName(), 16.00f, Font::plain).withTypefaceStyle ("Bold"));
    app_version_label_->setJustificationType (Justification::centredLeft);
    app_version_label_->setEditable (false, false, false);
    app_version_label_->setColour (Label::textColourId, Colours::black);
    app_version_label_->setColour (TextEditor::textColourId, Colours::black);
    app_version_label_->setColour (TextEditor::backgroundColourId, Colour (0x00000000));
    app_version_label_->setColour (TextEditor::highlightColourId, Colour (0xffe0ec65));

    addAndMakeVisible (app_version_ = new Label ("app version",
                                                 TRANS("(unknown version)")));
    app_version_->setFont (Font (Font::getDefaultSansSerifFontName(), 16.00f, Font::plain).withTypefaceStyle ("Regular"));
    app_version_->setJustificationType (Justification::centredRight);
    app_version_->setEditable (false, false, false);
    app_version_->setColour (Label::textColourId, Colour (0xff1e1e1e));
    app_version_->setColour (TextEditor::textColourId, Colours::black);
    app_version_->setColour (TextEditor::backgroundColourId, Colour (0x00000000));
    app_version_->setColour (TextEditor::highlightColourId, Colour (0xffe0ec65));

    addAndMakeVisible (core_version_ = new Label ("ompu core version",
                                                  TRANS("(unknown version)")));
    core_version_->setFont (Font (Font::getDefaultSansSerifFontName(), 16.00f, Font::plain).withTypefaceStyle ("Regular"));
    core_version_->setJustificationType (Justification::centredRight);
    core_version_->setEditable (false, false, false);
    core_version_->setColour (Label::textColourId, Colour (0xff1e1e1e));
    core_version_->setColour (TextEditor::textColourId, Colours::black);
    core_version_->setColour (TextEditor::backgroundColourId, Colour (0x00000000));
    core_version_->setColour (TextEditor::highlightColourId, Colour (0xffe0ec65));

    addAndMakeVisible (app_version_label_2 = new Label ("app version",
                                                        TRANS("ompu (core)")));
    app_version_label_2->setFont (Font (Font::getDefaultSansSerifFontName(), 16.00f, Font::plain).withTypefaceStyle ("Bold"));
    app_version_label_2->setJustificationType (Justification::centredLeft);
    app_version_label_2->setEditable (false, false, false);
    app_version_label_2->setColour (Label::textColourId, Colours::black);
    app_version_label_2->setColour (TextEditor::textColourId, Colours::black);
    app_version_label_2->setColour (TextEditor::backgroundColourId, Colour (0x00000000));
    app_version_label_2->setColour (TextEditor::highlightColourId, Colour (0xffe0ec65));

    drawable1 = Drawable::createFromImageData (BinaryData::ompulogo2xtrans_png, BinaryData::ompulogo2xtrans_pngSize);

    //[UserPreSize]
    //[/UserPreSize]

    setSize (720, 405);


    //[Constructor] You can add your own custom stuff here..
    app_version_->setText(app().version_set().at(plug::app_versions::ompu_app{}).str(), juce::dontSendNotification);
    core_version_->setText(app().version_set().at(plug::app_versions::ompu_core{}).str(), juce::dontSendNotification);
    //[/Constructor]
}

Logo::~Logo()
{
    //[Destructor_pre]. You can add your own custom destruction code here..
    //[/Destructor_pre]

    version_group_ = nullptr;
    app_version_label_ = nullptr;
    app_version_ = nullptr;
    core_version_ = nullptr;
    app_version_label_2 = nullptr;
    drawable1 = nullptr;


    //[Destructor]. You can add your own custom destruction code here..
    //[/Destructor]
}

//==============================================================================
void Logo::paint (Graphics& g)
{
    //[UserPrePaint] Add your own custom painting code here..
#pragma warning(push)
#pragma warning(disable:4244)
    //[/UserPrePaint]

    {
        int x = 0, y = 0, width = proportionOfWidth (1.0000f), height = proportionOfHeight (1.0000f);
        //[UserPaintCustomArguments] Customize the painting arguments here..
        //[/UserPaintCustomArguments]
        g.setColour (Colours::black);
        jassert (drawable1 != 0);
        if (drawable1 != 0)
            drawable1->drawWithin (g, Rectangle<float> (x, y, width, height),
                                   RectanglePlacement::centred | RectanglePlacement::onlyReduceInSize, 1.000f);
    }

    //[UserPaint] Add your own custom painting code here..
#pragma warning(pop)
    //[/UserPaint]
}

void Logo::resized()
{
    //[UserPreResize] Add your own custom resize code here..
    //[/UserPreResize]

    version_group_->setBounds (proportionOfWidth (0.9700f) - 256, proportionOfHeight (0.9300f) - 72, 256, 72);
    app_version_label_->setBounds ((proportionOfWidth (0.9700f) - 256) + roundFloatToInt (256 * 0.0313f), (proportionOfHeight (0.9300f) - 72) + 16, roundFloatToInt (256 * 0.4219f), 20);
    app_version_->setBounds ((proportionOfWidth (0.9700f) - 256) + roundFloatToInt (256 * 0.9688f) - (roundFloatToInt (256 * 0.4805f)), ((proportionOfHeight (0.9300f) - 72) + 16) + 0, roundFloatToInt (256 * 0.4805f), 20);
    core_version_->setBounds ((proportionOfWidth (0.9700f) - 256) + roundFloatToInt (256 * 0.9688f) - (roundFloatToInt (256 * 0.4805f)), ((proportionOfHeight (0.9300f) - 72) + 40) + 0, roundFloatToInt (256 * 0.4805f), 20);
    app_version_label_2->setBounds ((proportionOfWidth (0.9700f) - 256) + roundFloatToInt (256 * 0.0313f), (proportionOfHeight (0.9300f) - 72) + 40, roundFloatToInt (256 * 0.4219f), 20);
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

<JUCER_COMPONENT documentType="Component" className="Logo" componentName="Logo"
                 parentClasses="public plug::base::AppComponent&lt;&gt;" constructorParams="App&amp; app_"
                 variableInitialisers="AppComponent(app_)" snapPixels="8" snapActive="1"
                 snapShown="1" overlayOpacity="0.330" fixedSize="1" initialWidth="720"
                 initialHeight="405">
  <BACKGROUND backgroundColour="0">
    <IMAGE pos="0 0 100% 100%" resource="BinaryData::ompulogo2xtrans_png"
           opacity="1" mode="2"/>
  </BACKGROUND>
  <GROUPCOMPONENT name="new group" id="45d556c7c6855a77" memberName="version_group_"
                  virtualName="" explicitFocusOrder="0" pos="96.944%r 93.086%r 256 72"
                  outlinecol="ff343839" textcol="ff000000" title="Version"/>
  <LABEL name="app version" id="ad79f80e41aa12e0" memberName="app_version_label_"
         virtualName="" explicitFocusOrder="0" pos="3.125% 16 42.188% 20"
         posRelativeX="45d556c7c6855a77" posRelativeY="45d556c7c6855a77"
         posRelativeW="45d556c7c6855a77" posRelativeH="45d556c7c6855a77"
         textCol="ff000000" edTextCol="ff000000" edBkgCol="0" hiliteCol="ffe0ec65"
         labelText="Application" editableSingleClick="0" editableDoubleClick="0"
         focusDiscardsChanges="0" fontname="Default sans-serif font" fontsize="16"
         kerning="0" bold="1" italic="0" justification="33" typefaceStyle="Bold"/>
  <LABEL name="app version" id="99c989014eee541a" memberName="app_version_"
         virtualName="" explicitFocusOrder="0" pos="96.875%r 0 48.047% 20"
         posRelativeX="45d556c7c6855a77" posRelativeY="ad79f80e41aa12e0"
         posRelativeW="45d556c7c6855a77" posRelativeH="45d556c7c6855a77"
         textCol="ff1e1e1e" edTextCol="ff000000" edBkgCol="0" hiliteCol="ffe0ec65"
         labelText="(unknown version)" editableSingleClick="0" editableDoubleClick="0"
         focusDiscardsChanges="0" fontname="Default sans-serif font" fontsize="16"
         kerning="0" bold="0" italic="0" justification="34"/>
  <LABEL name="ompu core version" id="de3bf2029b99edc1" memberName="core_version_"
         virtualName="" explicitFocusOrder="0" pos="96.875%r 0 48.047% 20"
         posRelativeX="45d556c7c6855a77" posRelativeY="acb99b3bf93f3022"
         posRelativeW="45d556c7c6855a77" textCol="ff1e1e1e" edTextCol="ff000000"
         edBkgCol="0" hiliteCol="ffe0ec65" labelText="(unknown version)"
         editableSingleClick="0" editableDoubleClick="0" focusDiscardsChanges="0"
         fontname="Default sans-serif font" fontsize="16" kerning="0"
         bold="0" italic="0" justification="34"/>
  <LABEL name="app version" id="acb99b3bf93f3022" memberName="app_version_label_2"
         virtualName="" explicitFocusOrder="0" pos="3.125% 40 42.188% 20"
         posRelativeX="45d556c7c6855a77" posRelativeY="45d556c7c6855a77"
         posRelativeW="45d556c7c6855a77" textCol="ff000000" edTextCol="ff000000"
         edBkgCol="0" hiliteCol="ffe0ec65" labelText="ompu (core)" editableSingleClick="0"
         editableDoubleClick="0" focusDiscardsChanges="0" fontname="Default sans-serif font"
         fontsize="16" kerning="0" bold="1" italic="0" justification="33"
         typefaceStyle="Bold"/>
</JUCER_COMPONENT>

END_JUCER_METADATA
*/
#endif


//[EndFile] You can add extra defines here...
//[/EndFile]
