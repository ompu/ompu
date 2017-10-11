#pragma once

#include "ompu/music.hpp"

#include <boost/format.hpp>
#include <boost/io/ios_state.hpp>
#include <boost/core/ignore_unused.hpp>

#include <string>
#include <iosfwd>
#include <iomanip>


namespace ompu { namespace music {

namespace detail {

struct default_debug_format
{
    using format_type = boost::format;
    using string_type = std::string;

    static format_type sep()             { return format_type("\n%|84T-|\n"); }
    static string_type narrow_line()     { return {u8"|"}; }
    static string_type bold_line_left()  { return {"||:"}; }
    static string_type bold_line_right() { return {":||"}; }
    static format_type section()         { return format_type(bold_line_left() + u8" %|24t|\u266B \u266B %1% \u266B \u266B %|88t|" + bold_line_right()); }
    static format_type title()           { return format_type(bold_line_left() + u8" \u266B %|-74|" + bold_line_right()); }
    static format_type key_column()      { return format_type(u8"| \u266A %|-20|"); }
    static format_type value_column()    { return format_type(u8"| %|-54| |"); }
};

} // detail


template<class Format, class Context, class Enabled = void>
struct debug_printer;


template<class Format, class Ident, class KeyFeel>
struct debug_printer<
    Format, key_ident<Ident, KeyFeel>
>
{
    using context_type = key_ident<Ident, KeyFeel>;

    template<class... Degrees, class... Chords>
    static std::ostream& debug(std::ostream& os, std::tuple<degreed_chord<Degrees, Chords>...>)
    {
        saya::zed::blackhole{((
            os << "| " << std::left,
            debug_printer<Format, void>::debug(
                os,
                cvt::enharmonic_chord_ident_t<
                    cvt::interpret_on_key_t<context_type, Degrees>
                >{}
            ),
            os << std::left << Chords::ornament_name << " "
        ), nullptr)...};

        return os << "|";
    }
};


template<class Format>
struct debug_printer<
    Format, void
>
{
    template<class... Ts>
    static std::ostream& debug(std::ostream& os, std::tuple<Ts...>)
    {
        saya::zed::blackhole{(debug_printer<Format, void>::debug(os, Ts{}), nullptr)...};
        return os;
    }

    template<class... Mods>
    static std::ostream& debug(std::ostream& os, mod_set<Mods...>)
    {
        saya::zed::blackhole{(
            os << "| " << std::setw(4) << std::left << Mods::name, nullptr
        )...};
        return os << "|";
    }

    template<class Tone, class Mod>
    static std::ostream& debug(std::ostream& os, basic_ident<Tone, Mod>)
    {
        using ident_type = basic_ident<Tone, Mod>;
        return os << ident_type::name;
    }

    template<class... Tones, class... Mods>
    static std::ostream& debug(std::ostream& os, ident_set<basic_ident<Tones, Mods>...>)
    {
        saya::zed::blackhole{(
            os << "| " << std::setw(4) << std::left,
            debug_printer<Format, void>::debug(
                os, basic_ident<Tones, Mods>{}
            ), nullptr
        )...};
        return os << "|";
    }

    template<class ScaledAs, class... Tones, class... Mods>
    static std::ostream& debug(std::ostream& os, basic_scale<ScaledAs, ident_set<basic_ident<Tones, Mods>...>>)
    {
        using scale_type = basic_scale<ScaledAs, ident_set<basic_ident<Tones, Mods>...>>;

        os
            << Format::sep() << Format::title() % "Scale" << Format::sep()
            << Format::key_column() % "originally scaled as" << Format::value_column() % ScaledAs::name << "\n"
            << Format::key_column() % "tone mods"
        ;

        debug_printer<Format, void>::debug(os, mod_set<Mods...>{});

        os
            << "\n"
            << Format::key_column() % "canonical tones"
        ;

        debug_printer<Format, void>::debug(
            os, typename cvt::canonical_t<scale_type>::ident_set_type{}
        );
        return os << Format::sep();
    }

    template<class ScaledAs, class... Scales>
    static std::ostream& debug(std::ostream& os, dynamic_scale<ScaledAs, Scales...>)
    {
        using scale_type = dynamic_scale<ScaledAs, Scales...>;
        using upward_scale_type = typename scale_type::upward_scale_type;
        using downward_scale_type = typename scale_type::downward_scale_type;

        os
            << Format::sep() << Format::section() % "Dynamic scale" << Format::sep()
            << Format::key_column() % "scaled as" << Format::value_column() % ScaledAs::name << "\n"
            << Format::key_column() % "is dynamic?"
            << Format::value_column() % boost::io::group(std::boolalpha, scale_type::is_dynamic) << "\n"
        ;

        if (scale_type::is_dynamic) {
            os << Format::sep() << Format::section() % "Dynamic scale :: upward" << Format::sep();
            debug_printer<Format, void>::debug(os, upward_scale_type{});

            os << Format::sep() << Format::section() % "Dynamic scale :: downward" << Format::sep();
            debug_printer<Format, void>::debug(os, downward_scale_type{});

        } else {
            debug_printer<Format, void>::debug(os, upward_scale_type{});
        }
        return os << Format::sep();
    }

    template<class Ident, class KeyFeel>
    static std::ostream& debug(std::ostream& os, basic_key<key_ident<Ident, KeyFeel>>)
    {
        using key_ident_type = key_ident<Ident, KeyFeel>;
        using key_type = basic_key<key_ident_type>;

        os
            << Format::sep() << Format::section() % "Key" << Format::sep()
            << Format::key_column() % "name" << Format::value_column() % key_type::name << "\n"
            << Format::key_column() % "key sign" << Format::value_column() % key_type::key_sign_type::symbol << "\n"
            << Format::key_column() % "key sign mods (on C)"
        ;

        debug_printer<Format, void>::debug(os, typename key_type::key_sign_type::key_mod_set{}) << "\n";

        os
            << Format::key_column() % "diatonic chords"
        ;

        debug_printer<Format, key_ident_type>::debug(
            os, typename key_type::diatonic_chords_tetrad{}
        );

        os << Format::sep() << Format::title() % "Key scale" << Format::sep();
        debug_printer<Format, void>::debug(os, typename key_type::key_scale_type{});

        os << Format::sep() << Format::title() % "Key scale (Harmonic)" << Format::sep();
        debug_printer<Format, void>::debug(os, scales::make_harmonic<key_ident_type, KeyFeel>{});

        os << Format::sep() << Format::title() % "Key scale (Melodic)" << Format::sep();
        debug_printer<Format, void>::debug(os, scales::make_melodic<key_ident_type, KeyFeel>{});

        return os << Format::sep();
    }

    template<class Degree, class Chord>
    static std::ostream& debug(std::ostream& os, degreed_chord<Degree, Chord>)
    {
        return os
            << typename Format::format_type(Format::narrow_line() + " %s%s")
            % Degree::name
            % boost::io::group(
                std::setw(5), std::left,
                Chord::ornament_name
            )
        ;
    }
};


namespace detail {

template<class Format, class... DiatonicDegrees>
inline std::ostream& debug_help_impl(std::ostream& os, std::tuple<DiatonicDegrees...>)
{
    boost::io::ios_flags_saver ios_save_{os};
    boost::ignore_unused(ios_save_);

    os
        << Format::sep()
        << typename Format::format_type{u8"|| %|=88|||"} % u8"\u2669 \u266A \u266B ompu::music debug output(sampled) \u266B \u266A \u2669"
        << Format::sep()
    ;

    os << Format::sep() << Format::title() % "Diatonic chords (Major key)" << Format::sep();
    saya::zed::blackhole{((debug_printer<Format, void>::debug(os, diatonic_chords_t<key_feels::major, DiatonicDegrees, true>{})), nullptr)...};

    os << Format::sep() << Format::title() % "Diatonic chords (Minor key)" << Format::sep();
    saya::zed::blackhole{((debug_printer<Format, void>::debug(os, diatonic_chords_t<key_feels::minor, DiatonicDegrees, true>{})), nullptr)...};

    os << Format::sep() << Format::section() % "Major keys" << Format::sep();
    debug_printer<Format, void>::debug(
        os,
        std::tuple<
            keys::GMaj, keys::DMaj, keys::AMaj, keys::EMaj, keys::BMaj, keys::FsMaj, keys::CsMaj
        >{}
#if 0
        std::tuple<
            keys::CMaj, // natural
            keys::CsMaj, // transposed +1
            keys::DMaj, // sharped
            keys::BbMaj, // flatted
            keys::DbMaj // w/ double-flatted scale
        >{}
#endif
    );

    os << Format::sep() << Format::section() % "Minor keys" << Format::sep();
    debug_printer<Format, void>::debug(
        os,
        std::tuple<
            keys::Amin, // natural
            keys::Asmin, // transposed +1
            keys::Emin, // sharped
            keys::Gsmin, // w/ double-sharped scale
            keys::Bbmin // w/ natural-ed scale
        >{}
    );
    return os;
}

} // detail

template<class Format = detail::default_debug_format>
inline std::ostream& debug_help(std::ostream& os)
{
    return detail::debug_help_impl<Format>(os, all_diatonic_degrees{});
}

}} // ompu
