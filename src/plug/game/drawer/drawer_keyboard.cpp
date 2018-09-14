#include "ompu/plug/game/drawer.hpp"

#include "ompu/ui/keyboard.hpp"
#include "ompu/music/key.hpp"

#include "saya/zed/blackhole.hpp"

#include <boost/format.hpp>
#include <boost/range/irange.hpp>
#include <boost/range/adaptor/indexed.hpp>

#include <boost/core/ignore_unused.hpp>

#include "JuceHeader.h"


namespace ompu { namespace plug { namespace game {

namespace {

template<std::size_t I>
std::nullptr_t sep_step_single(juce::Graphics& g, juce::Rectangle<float> const& rc)
{
    using ompu::ui::Keyboard;
    namespace kbs = ompu::ui::keyboards;

    using rcalc_type = ompu::geo::RatioCalculator<
        kbs::clearance_x_ratio_t<Keyboard, I>,
        std::ratio<0, 1>,
        typename Keyboard::clearance_ratio::w_ratio,
        typename Keyboard::clearance_ratio::h_ratio
    >;

    auto const rc_key = rcalc_type::rect_for_step<juce::Rectangle<float>>(
        rc.getX(), rc.getY(),
        rc.getWidth(), rc.getHeight()
    );
    g.fillRect(rc_key);

    return nullptr;
}

template<std::size_t... Is>
void sep_step(juce::Graphics& g, juce::Rectangle<float> const& rc, std::index_sequence<Is...>)
{
    saya::zed::blackhole{(sep_step_single<Is>(g, rc), 0)...};
}

namespace detail {

template<std::size_t I, class Enabled = void>
struct note_info
{
    static void draw(juce::Graphics&, juce::Rectangle<float> const&) {}
};

template<std::size_t I>
struct note_info<I, std::enable_if_t<midi::key_octave_index_v<I> == 0>>
{
    static void draw(juce::Graphics& g, juce::Rectangle<float> const& rc)
    {
        g.setColour(juce::Colour::fromRGBA(0, 0, 0, 200));
        g.drawText(
            "*",
            rc,
            juce::Justification::centredTop
        );
    }
};

template<std::size_t I>
struct note_info<I, std::enable_if_t<I == midi::Spec::A4>>
{
    static void draw(juce::Graphics& g, juce::Rectangle<float> const& rc)
    {
        g.setColour(juce::Colour::fromRGBA(0, 0, 0, 240));
        g.drawText(
            "A4",
            rc,
            juce::Justification::centredTop
        );
    }
};

} // detail

template<std::size_t I>
std::nullptr_t white_key_step_single(juce::Graphics& g, juce::Rectangle<float> const& rc, ompu::ui::Keyboard const& c)
{
    using ompu::ui::Keyboard;
    namespace kbs = ompu::ui::keyboards;

    using rcalc_type = ompu::geo::RatioCalculator<
        kbs::white_key_x_ratio_t<Keyboard, I>,
        std::ratio<0, 1>,
        typename Keyboard::white_key_ratio::w_ratio,
        typename Keyboard::white_key_ratio::h_ratio
    >;

    auto const rc_key = rcalc_type::rect_for_step<juce::Rectangle<float>>(
        rc.getX(), rc.getY(),
        rc.getWidth(), rc.getHeight()
    );

    auto const& key = c.nth_key<I>();

#if 0
    // (optimized out)
    g.setColour(juce::Colour::fromRGB(240, 240, 240));
    g.fillRect(rc_key);
#endif

    if (key.is_pressed()) {
        g.setColour(juce::Colour::fromRGBA(0, 0, 0, 20));
        g.drawRect(rc_key, rc_key.getWidth() * .1f);
    }

    // -------------------------------------
#if 0
    juce::Font const f{juce::Font::getDefaultSansSerifFontName(), 16, juce::Font::FontStyleFlags::bold};
    g.setFont(f);

    detail::note_info<kbs::to_midi_index_v<Keyboard, I>>::draw(
        g,
        decltype(rc_key){
            rc_key.getX(), rc_key.getY() + rc_key.getHeight() * .9f,
            rc_key.getWidth(), rc_key.getHeight()
        }
    );
#endif
    return nullptr;
}

template<std::size_t... Is>
void white_key_step(
    juce::Graphics& g, juce::Rectangle<float> const& rc, ompu::ui::Keyboard const& c,
    std::index_sequence<Is...>
) {
    saya::zed::blackhole{(white_key_step_single<Is>(g, rc, c), 0)...};
}

template<std::size_t I>
std::nullptr_t black_key_step_single(juce::Graphics& g, juce::Rectangle<float> const& rc, ompu::ui::Keyboard const& c)
{
    using ompu::ui::Keyboard;
    namespace kbs = ompu::ui::keyboards;

    using rcalc_type = ompu::geo::RatioCalculator<
        kbs::black_key_x_ratio_t<Keyboard, I>,
        std::ratio<0, 1>,
        typename Keyboard::black_key_ratio::w_ratio,
        typename Keyboard::black_key_ratio::h_ratio
    >;

    auto const rc_key = rcalc_type::rect_for_step<juce::Rectangle<float>>(
        rc.getX(), rc.getY(),
        rc.getWidth(), rc.getHeight()
    );
    g.setColour(juce::Colour::fromRGB(20, 20, 20));
    g.fillRect(rc_key);

    auto const& key = c.nth_key<I>();

    if (key.is_pressed()) {
        g.setColour(juce::Colour::fromRGBA(255, 255, 255, 200));
        g.drawRect(rc_key, rc_key.getWidth() * .1f);
    }

#if 0
    char const* note_name = nullptr;
    switch (ZeroI % 5) {
    case 0:
        note_name = "C#"; break;
    case 1:
        note_name = "D#"; break;
    case 2:
        note_name = "F#"; break;
    case 3:
        note_name = "G#"; break;
    case 4:
        note_name = "A#"; break;
    }

    auto const text_w = rc_key.getWidth() * 5.f;
    juce::Rectangle<float> const rc_text{
        rc_key.getX(),
        rc_key.getY() + rc_key.getHeight() * .2f,
        text_w,
        rc_key.getHeight() * .2f
    };


    g.setColour(juce::Colour::fromRGB(255, 255, 255));
    g.drawText(
        note_name,
        rc_text,
        juce::Justification::topLeft
    );
#endif

    return nullptr;
}

template<std::size_t... Is>
void black_key_step(
    juce::Graphics& g, juce::Rectangle<float> const& rc, ompu::ui::Keyboard const& c,
    std::index_sequence<Is...>
)
{
    saya::zed::blackhole{(black_key_step_single<Is>(g, rc, c), 0)...};
}

} // anon


void Drawer::partial(ompu::ui::Keyboard const& c) const
{
    using boost::adaptors::indexed;
    static auto const BLACK = juce::Colour::fromRGB(20, 20, 20);
    static auto const WHITE = juce::Colour::fromRGB(240, 240, 240);
    static auto const SEP_BLACK = juce::Colour::fromRGBA(0, 0, 0, 40);

    static float const ROOT_WIDTH = 1.f;
    static float const ROOT_HEIGHT = .15f;

    // -----------------------------------------

    juce::Rectangle<float> const rc_root{
        float(jc_->getPosition().x),
        jc_->getY() + jc_->getHeight() * (1 - ROOT_HEIGHT),
        float(jc_->getWidth()),
        jc_->getHeight() * ROOT_HEIGHT
    };

    // master bg
    g_->setColour(BLACK);
    g_->fillRect(rc_root);

    using ompu::ui::Keyboard;
    namespace kbs = ompu::ui::keyboards;

    auto const rc_key_root = rc_root.reduced(
        rc_root.proportionOfWidth(.05f),
        rc_root.proportionOfHeight(.02f)
    );

    g_->setColour(WHITE);
    g_->fillRect(rc_key_root);

    // white keys
    white_key_step(
        *g_, rc_key_root, c,
        kbs::make_white_key_sequence<Keyboard>()
    );

    // key seps
    g_->setColour(SEP_BLACK);
    sep_step(*g_, rc_key_root, kbs::make_clearance_sequence<Keyboard>{});

    // black keys
    black_key_step(
        *g_, rc_key_root, c,
        kbs::make_black_key_sequence<Keyboard>()
    );
}

}}} // ompu
