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
#include "ompu/plug/util.hpp"

#include <boost/range/adaptor/indexed.hpp>
#include <boost/algorithm/cxx11/any_of.hpp>
//[/Headers]

#include "config_window_audio.h"


//[MiscUserDefs] You can add your own user definitions and misc code here...
namespace ompu { namespace plug { namespace gui {
//[/MiscUserDefs]

//==============================================================================
ConfigWindowAudio::ConfigWindowAudio (App& app_)
    : AppComponent(app_)
{
    //[Constructor_pre] You can add your own custom stuff here..
    //[/Constructor_pre]

    setName ("ConfigWindowAudio");
    addAndMakeVisible (label2 = new Label ("new label",
                                           TRANS("Input channels:")));
    label2->setFont (Font (15.00f, Font::plain).withTypefaceStyle ("Regular"));
    label2->setJustificationType (Justification::centredLeft);
    label2->setEditable (false, false, false);
    label2->setColour (TextEditor::textColourId, Colours::black);
    label2->setColour (TextEditor::backgroundColourId, Colour (0x00000000));

    addAndMakeVisible (in_ch_ = new Viewport ("Input channels"));
    in_ch_->setScrollBarsShown (true, false);

    addAndMakeVisible (label3 = new Label ("new label",
                                           TRANS("Output channels:")));
    label3->setFont (Font (15.00f, Font::plain).withTypefaceStyle ("Regular"));
    label3->setJustificationType (Justification::centredLeft);
    label3->setEditable (false, false, false);
    label3->setColour (TextEditor::textColourId, Colours::black);
    label3->setColour (TextEditor::backgroundColourId, Colour (0x00000000));

    addAndMakeVisible (out_ch_ = new Viewport ("Output channels"));
    out_ch_->setScrollBarsShown (true, false);

    addAndMakeVisible (label = new Label ("new label",
                                          TRANS("Host device:")));
    label->setFont (Font (15.00f, Font::plain).withTypefaceStyle ("Regular"));
    label->setJustificationType (Justification::centredLeft);
    label->setEditable (false, false, false);
    label->setColour (TextEditor::textColourId, Colours::black);
    label->setColour (TextEditor::backgroundColourId, Colour (0x00000000));

    addAndMakeVisible (host_device_ = new ComboBox ("Audio device"));
    host_device_->setEditableText (false);
    host_device_->setJustificationType (Justification::centredLeft);
    host_device_->setTextWhenNothingSelected (String());
    host_device_->setTextWhenNoChoicesAvailable (TRANS("(no device available)"));
    host_device_->addListener (this);

    addAndMakeVisible (play_test_ = new TextButton ("new button"));
    play_test_->setButtonText (TRANS("Test"));
    play_test_->addListener (this);

    addAndMakeVisible (cfg_ = new TextButton ("new button"));
    cfg_->setButtonText (TRANS("Configure..."));
    cfg_->addListener (this);
    cfg_->setColour (TextButton::buttonColourId, Colour (0xff1a4663));

    addAndMakeVisible (reset_ = new TextButton ("new button"));
    reset_->setButtonText (TRANS("Reset"));
    reset_->addListener (this);
    reset_->setColour (TextButton::buttonColourId, Colours::crimson);

    addAndMakeVisible (label4 = new Label ("new label",
                                           TRANS("Sample rate:")));
    label4->setFont (Font (15.00f, Font::plain).withTypefaceStyle ("Regular"));
    label4->setJustificationType (Justification::centredLeft);
    label4->setEditable (false, false, false);
    label4->setColour (TextEditor::textColourId, Colours::black);
    label4->setColour (TextEditor::backgroundColourId, Colour (0x00000000));

    addAndMakeVisible (sample_rate_ = new ComboBox (String()));
    sample_rate_->setEditableText (false);
    sample_rate_->setJustificationType (Justification::centredLeft);
    sample_rate_->setTextWhenNothingSelected (String());
    sample_rate_->setTextWhenNoChoicesAvailable (TRANS("(not available)"));
    sample_rate_->addListener (this);

    addAndMakeVisible (label5 = new Label ("new label",
                                           TRANS("Buffer size:")));
    label5->setFont (Font (15.00f, Font::plain).withTypefaceStyle ("Regular"));
    label5->setJustificationType (Justification::centredLeft);
    label5->setEditable (false, false, false);
    label5->setColour (TextEditor::textColourId, Colours::black);
    label5->setColour (TextEditor::backgroundColourId, Colour (0x00000000));

    addAndMakeVisible (buffer_size_ = new ComboBox (String()));
    buffer_size_->setEditableText (false);
    buffer_size_->setJustificationType (Justification::centredLeft);
    buffer_size_->setTextWhenNothingSelected (String());
    buffer_size_->setTextWhenNoChoicesAvailable (TRANS("(not available)"));
    buffer_size_->addListener (this);


    //[UserPreSize]
    //[/UserPreSize]

    setSize (600, 400);


    //[Constructor] You can add your own custom stuff here..
    app().dtype->scanForDevices();

    auto const& names = app_.dtype->getDeviceNames();
    for (auto const& name : make_range(names) | boost::adaptors::indexed(0)) {
        l().info() << "found device [" << name.index() + 1 << "/" << names.size() << "]: '" << name.value() << "'" << std::endl;

        static auto const BAD_DRIVERS = {
            "ASIO DirectX Full Duplex Driver",
            "Generic Low Latency ASIO Driver",
        };

        if (boost::algorithm::any_of_equal(BAD_DRIVERS, name.value())) {
            // fuck this shit!
            l().warn() << "blacklisted driver, skipping: '" << name.value() << "'" << std::endl;
            continue;
        }

        host_device_->addItem(name.value(), from_index(name.index()).id());
    }

    dm().addChangeListener(this);
    //[/Constructor]
}

ConfigWindowAudio::~ConfigWindowAudio()
{
    //[Destructor_pre]. You can add your own custom destruction code here..
    //[/Destructor_pre]

    label2 = nullptr;
    in_ch_ = nullptr;
    label3 = nullptr;
    out_ch_ = nullptr;
    label = nullptr;
    host_device_ = nullptr;
    play_test_ = nullptr;
    cfg_ = nullptr;
    reset_ = nullptr;
    label4 = nullptr;
    sample_rate_ = nullptr;
    label5 = nullptr;
    buffer_size_ = nullptr;


    //[Destructor]. You can add your own custom destruction code here..
    //[/Destructor]
}

//==============================================================================
void ConfigWindowAudio::paint (Graphics& g)
{
    //[UserPrePaint] Add your own custom painting code here..
    //[/UserPrePaint]

    g.fillAll (Colour (0xff323e44));

    //[UserPaint] Add your own custom painting code here..
    //[/UserPaint]
}

void ConfigWindowAudio::resized()
{
    //[UserPreResize] Add your own custom resize code here..
    //[/UserPreResize]

    label2->setBounds (8, 208, 150, 24);
    in_ch_->setBounds (16, 232, 336, 81);
    label3->setBounds (360, 208, 150, 24);
    out_ch_->setBounds (368, 232, 336, 81);
    label->setBounds (8, 8, 150, 16);
    host_device_->setBounds (14, 30, 274, 24);
    play_test_->setBounds (248, 8, 40, 16);
    cfg_->setBounds (16, 61, 80, 16);
    reset_->setBounds (248, 60, 40, 16);
    label4->setBounds (8, 88, 150, 16);
    sample_rate_->setBounds (16, 110, 272, 24);
    label5->setBounds (8, 144, 150, 16);
    buffer_size_->setBounds (16, 166, 272, 24);
    //[UserResized] Add your own custom resize handling here..
    //[/UserResized]
}

void ConfigWindowAudio::comboBoxChanged (ComboBox* comboBoxThatHasChanged)
{
    //[UsercomboBoxChanged_Pre]
    auto* const dev = dm().getCurrentAudioDevice();

    using setup_type = juce::AudioDeviceManager::AudioDeviceSetup;
    std::unique_ptr<setup_type> setup;

    auto const get_setup = [this] {
        auto setup = std::make_unique<setup_type>();
        dm().getAudioDeviceSetup(*setup);

        return setup;
    };
    //[/UsercomboBoxChanged_Pre]

    if (comboBoxThatHasChanged == host_device_)
    {
        //[UserComboBoxCode_host_device_] -- add your combo box handling code here..
        auto const dev_name = app().dtype->getDeviceNames()[from_id(host_device_->getSelectedId()).index()];
        l().note() << "'" << dev_name << "'" << std::endl;
        l().info() << "device selected" << std::endl;

        if (void* curr_dev = static_cast<void*>(dm().getCurrentAudioDevice())) {
            l().info() << "closing current audio device (was " << curr_dev << ")" << std::endl;
            dm().closeAudioDevice();
        }

        l().info() << "initializing device manager..." << std::endl;

        setup = get_setup();
        setup->outputDeviceName = dev_name;
        if (!app().dtype->hasSeparateInputsAndOutputs()) {
            setup->inputDeviceName = setup->outputDeviceName;
        }

        setup->useDefaultInputChannels = true;
        setup->useDefaultOutputChannels = true;

        //setup->bufferSize = 64;
        //setup->sampleRate = 48000;
        //[/UserComboBoxCode_host_device_]
    }
    else if (comboBoxThatHasChanged == sample_rate_)
    {
        //[UserComboBoxCode_sample_rate_] -- add your combo box handling code here..
        auto const srates = dev->getAvailableSampleRates();
        auto const idx = sample_rate_->getSelectedId() - 1;

        // this must be O(N), but we don't care
        if (idx + 1 > srates.size()) {
            l().error() << "unknown sample rate selected!" << std::endl;
            dm().restartLastAudioDevice();

        } else {
            setup = get_setup();
            setup->sampleRate = srates[idx];
        }
        //[/UserComboBoxCode_sample_rate_]
    }
    else if (comboBoxThatHasChanged == buffer_size_)
    {
        //[UserComboBoxCode_buffer_size_] -- add your combo box handling code here..
        auto const bsizes = dev->getAvailableBufferSizes();
        auto const idx = buffer_size_->getSelectedId() - 1;

        // this must be O(N), but we don't care
        if (idx + 1 > bsizes.size()) {
            l().error() << "unknown buffer size selected!" << std::endl;
            dm().restartLastAudioDevice();

        } else {
            setup = get_setup();
            setup->bufferSize = bsizes[idx];
        }
        //[/UserComboBoxCode_buffer_size_]
    }

    //[UsercomboBoxChanged_Post]
    if (setup) {
        l().note() << *setup << std::endl;
        l().info() << "got a new setup, initializing device..." << std::endl;

        auto const msg = dm().initialise(
            2, // in ch
            2, // out ch
            nullptr /* cfg_audio_ */, // maybe-nullptr
            false, // select default on failure
            {}, // preferred device name
            setup.get() // preferred setup
        );

        if (!msg.isEmpty()) {
            l().note() << msg << std::endl;
            l().error() << "failed to initialize device" << std::endl;
        } else {
            l().info() << "device initialized" << std::endl;
        }
    }
    //[/UsercomboBoxChanged_Post]
}

void ConfigWindowAudio::buttonClicked (Button* buttonThatWasClicked)
{
    //[UserbuttonClicked_Pre]
    auto* const dev = dm().getCurrentAudioDevice();
    //[/UserbuttonClicked_Pre]

    if (buttonThatWasClicked == play_test_)
    {
        //[UserButtonCode_play_test_] -- add your button handler code here..
        l().info() << "playing test sound..." << std::endl;
        dm().playTestSound();
        //[/UserButtonCode_play_test_]
    }
    else if (buttonThatWasClicked == cfg_)
    {
        //[UserButtonCode_cfg_] -- add your button handler code here..
        if (!dev) {
            l().error() << "device not selected" << std::endl;
        } else {
            dev->showControlPanel();
        }
        //[/UserButtonCode_cfg_]
    }
    else if (buttonThatWasClicked == reset_)
    {
        //[UserButtonCode_reset_] -- add your button handler code here..
        l().warn() << "resetting audio device..." << std::endl;

        dm().closeAudioDevice();
        dm().restartLastAudioDevice();
        //[/UserButtonCode_reset_]
    }

    //[UserbuttonClicked_Post]
    //[/UserbuttonClicked_Post]
}



//[MiscUserCode] You can add your own definitions of your custom methods or any other code here...
void ConfigWindowAudio::changeListenerCallback(juce::ChangeBroadcaster* const src)
{
    if (src == std::addressof(dm())) {
        this->fetchSampleRate();
        this->fetchBufferSize();

    } else {
        l().warn() << "unknown source for changeListenerCallback()" << std::endl;
        return;
    }
}

void ConfigWindowAudio::fetchSampleRate()
{
    auto* const dev = dm().getCurrentAudioDevice();
    sample_rate_->clear(juce::dontSendNotification);

    auto const srate_curr = dev->getCurrentSampleRate();
    auto const srates = dev->getAvailableSampleRates();

    for (auto const& srate : make_range(srates) | boost::adaptors::indexed(0)) {
        auto const id = from_index(srate.index());
        sample_rate_->addItem(boost::str(boost::format("%.0f Hz") % srate.value()), id.id());

        if (srate.value() == srate_curr) {
            sample_rate_->setSelectedId(id.id(), juce::dontSendNotification);
        }
    }
}

void ConfigWindowAudio::fetchBufferSize()
{
    auto* const dev = dm().getCurrentAudioDevice();

    buffer_size_->clear(juce::dontSendNotification);

    auto const bsize_curr = dev->getCurrentBufferSizeSamples();
    auto const bsizes = dev->getAvailableBufferSizes();

    for (auto const& bsize : make_range(bsizes) | boost::adaptors::indexed(0)) {
        auto const id = from_index(bsize.index());
        buffer_size_->addItem(boost::str(boost::format("%d samples") % bsize.value()), id.id());

        if (bsize.value() == bsize_curr) {
            buffer_size_->setSelectedId(id.id(), juce::dontSendNotification);
        }
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

<JUCER_COMPONENT documentType="Component" className="ConfigWindowAudio" componentName="ConfigWindowAudio"
                 parentClasses="public ompu::plug::base::AppComponent&lt;juce::Component&gt;, public juce::ChangeListener"
                 constructorParams="App&amp; app_" variableInitialisers="AppComponent(app_)"
                 snapPixels="8" snapActive="1" snapShown="1" overlayOpacity="0.330"
                 fixedSize="0" initialWidth="600" initialHeight="400">
  <BACKGROUND backgroundColour="ff323e44"/>
  <LABEL name="new label" id="780e662dc6161216" memberName="label2" virtualName=""
         explicitFocusOrder="0" pos="8 208 150 24" edTextCol="ff000000"
         edBkgCol="0" labelText="Input channels:" editableSingleClick="0"
         editableDoubleClick="0" focusDiscardsChanges="0" fontname="Default font"
         fontsize="15" kerning="0" bold="0" italic="0" justification="33"/>
  <VIEWPORT name="Input channels" id="117afce5d748787" memberName="in_ch_"
            virtualName="" explicitFocusOrder="0" pos="16 232 336 81" vscroll="1"
            hscroll="0" scrollbarThickness="8" contentType="0" jucerFile=""
            contentClass="" constructorParams=""/>
  <LABEL name="new label" id="fbb6de719518f68e" memberName="label3" virtualName=""
         explicitFocusOrder="0" pos="360 208 150 24" edTextCol="ff000000"
         edBkgCol="0" labelText="Output channels:" editableSingleClick="0"
         editableDoubleClick="0" focusDiscardsChanges="0" fontname="Default font"
         fontsize="15" kerning="0" bold="0" italic="0" justification="33"/>
  <VIEWPORT name="Output channels" id="6c05ca9de5be4b15" memberName="out_ch_"
            virtualName="" explicitFocusOrder="0" pos="368 232 336 81" vscroll="1"
            hscroll="0" scrollbarThickness="8" contentType="0" jucerFile=""
            contentClass="" constructorParams=""/>
  <LABEL name="new label" id="e31782dbfd3f6f4d" memberName="label" virtualName=""
         explicitFocusOrder="0" pos="8 8 150 16" edTextCol="ff000000"
         edBkgCol="0" labelText="Host device:" editableSingleClick="0"
         editableDoubleClick="0" focusDiscardsChanges="0" fontname="Default font"
         fontsize="15" kerning="0" bold="0" italic="0" justification="33"/>
  <COMBOBOX name="Audio device" id="3aa28d6538e7d939" memberName="host_device_"
            virtualName="" explicitFocusOrder="0" pos="14 30 274 24" editable="0"
            layout="33" items="" textWhenNonSelected="" textWhenNoItems="(no device available)"/>
  <TEXTBUTTON name="new button" id="9b5aef6a206873a0" memberName="play_test_"
              virtualName="" explicitFocusOrder="0" pos="248 8 40 16" buttonText="Test"
              connectedEdges="0" needsCallback="1" radioGroupId="0"/>
  <TEXTBUTTON name="new button" id="b9c764fd9c44582a" memberName="cfg_" virtualName=""
              explicitFocusOrder="0" pos="16 61 80 16" bgColOff="ff1a4663"
              buttonText="Configure..." connectedEdges="0" needsCallback="1"
              radioGroupId="0"/>
  <TEXTBUTTON name="new button" id="7484aa7bb51bd03a" memberName="reset_" virtualName=""
              explicitFocusOrder="0" pos="248 60 40 16" bgColOff="ffdc143c"
              buttonText="Reset" connectedEdges="0" needsCallback="1" radioGroupId="0"/>
  <LABEL name="new label" id="e09f7bb189df5b5d" memberName="label4" virtualName=""
         explicitFocusOrder="0" pos="8 88 150 16" edTextCol="ff000000"
         edBkgCol="0" labelText="Sample rate:" editableSingleClick="0"
         editableDoubleClick="0" focusDiscardsChanges="0" fontname="Default font"
         fontsize="15" kerning="0" bold="0" italic="0" justification="33"/>
  <COMBOBOX name="" id="8eb65cf07fc25fdc" memberName="sample_rate_" virtualName=""
            explicitFocusOrder="0" pos="16 110 272 24" editable="0" layout="33"
            items="" textWhenNonSelected="" textWhenNoItems="(not available)"/>
  <LABEL name="new label" id="3df425cd6392d15c" memberName="label5" virtualName=""
         explicitFocusOrder="0" pos="8 144 150 16" edTextCol="ff000000"
         edBkgCol="0" labelText="Buffer size:" editableSingleClick="0"
         editableDoubleClick="0" focusDiscardsChanges="0" fontname="Default font"
         fontsize="15" kerning="0" bold="0" italic="0" justification="33"/>
  <COMBOBOX name="" id="e80ccd2b8ceeb43d" memberName="buffer_size_" virtualName=""
            explicitFocusOrder="0" pos="16 166 272 24" editable="0" layout="33"
            items="" textWhenNonSelected="" textWhenNoItems="(not available)"/>
</JUCER_COMPONENT>

END_JUCER_METADATA
*/
#endif


//[EndFile] You can add extra defines here...
//[/EndFile]
