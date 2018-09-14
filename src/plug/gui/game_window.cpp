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
#include "ompu/plug/game/event_handler.hpp"
#include "ompu/plug/game/drawer.hpp"
#include "ompu/plug/game/updater.hpp"

#include "ompu/game/game_data_snapshot.hpp"
//[/Headers]

#include "game_window.h"


//[MiscUserDefs] You can add your own user definitions and misc code here...
namespace ompu { namespace plug { namespace gui {
//[/MiscUserDefs]

//==============================================================================
GameWindow::GameWindow (App& app_)
    : AppComponent(app_),
      game_(app_.game())
{
    //[Constructor_pre] You can add your own custom stuff here..
    //[/Constructor_pre]

    setName ("GameWindow");

    //[UserPreSize]
    //[/UserPreSize]

    setSize (1600, 1200);


    //[Constructor] You can add your own custom stuff here..
    app().juce_component(this);
    gd_ss_ = game_->async_snapshot();

    this->update();
    this->setFramesPerSecond(app().desired_fps());
    //[/Constructor]
}

GameWindow::~GameWindow()
{
    //[Destructor_pre]. You can add your own custom destruction code here..
    //[/Destructor_pre]



    //[Destructor]. You can add your own custom destruction code here..
    //[/Destructor]
}

//==============================================================================
void GameWindow::paint (Graphics& g)
{
    //[UserPrePaint] Add your own custom painting code here..
    //[/UserPrePaint]

    //[UserPaint] Add your own custom painting code here..
    app().juce_draw_context(std::addressof(g));
    game_->synced_draw(gd_ss_);
    //[/UserPaint]
}

void GameWindow::resized()
{
    //[UserPreResize] Add your own custom resize code here..
    //[/UserPreResize]

    //[UserResized] Add your own custom resize handling here..
    //[/UserResized]
}



//[MiscUserCode] You can add your own definitions of your custom methods or any other code here...
void GameWindow::update()
{
    gd_ss_ = game_->synced_update();
}

}}} // ompu
//[/MiscUserCode]


//==============================================================================
#if 0
/*  -- Projucer information section --

    This is where the Projucer stores the metadata that describe this GUI layout, so
    make changes in here at your peril!

BEGIN_JUCER_METADATA

<JUCER_COMPONENT documentType="Component" className="GameWindow" componentName="GameWindow"
                 parentClasses="public ompu::plug::base::AppComponent&lt;juce::AnimatedAppComponent&gt;, private plug::detail::game_access"
                 constructorParams="App&amp; app_" variableInitialisers="AppComponent(app_)&#10;game_(app_.game())"
                 snapPixels="8" snapActive="1" snapShown="1" overlayOpacity="0.330"
                 fixedSize="1" initialWidth="1600" initialHeight="1200">
  <BACKGROUND backgroundColour="0"/>
</JUCER_COMPONENT>

END_JUCER_METADATA
*/
#endif


//[EndFile] You can add extra defines here...
//[/EndFile]
