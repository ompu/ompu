#pragma once

#include "ompu/plug/gui/gui_fwd.hpp"
#include "ompu/plug/app.hpp"

#include "ompu/component_id.hpp"

#include "saya/logger.hpp"

#include "JuceHeader.h"


namespace ompu { namespace plug { namespace base {

template<class T = juce::Component>
class AppComponent : public T
{
public:
    using base_type = T;

    template<class... Args>
    explicit AppComponent(App& app, Args&&... args)
        : base_type(std::forward<Args>(args)...)
        , app_(app)
        , l_(app_.l())
    {
        l_.set_prompt_fetcher([this] () -> std::string { return as_component()->getName().toStdString(); });
    }

    juce::AudioDeviceManager const& dm() const noexcept { return app_.dm(); }
    juce::AudioDeviceManager& dm() noexcept { return app_.dm(); }

    App const& app() const noexcept { return app_; }
    App& app() noexcept { return app_; }

    saya::logger& l() noexcept { return l_; }
    //saya::logger const& l() const noexcept { return l_; }

private:
    T const* as_component() const noexcept { return static_cast<T const*>(this); }
    T* as_component() noexcept { return static_cast<T*>(this); }

    App& app_;
    saya::logger l_;
};

}}} // ompu
