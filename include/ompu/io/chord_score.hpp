#pragma once

#include "ompu/music/concrete/key.hpp"
#include "ompu/music/concrete/chord_identity.hpp"

#include <boost/variant/variant.hpp>
#include <boost/variant/recursive_wrapper.hpp>
#include <boost/optional.hpp>
#include <boost/preprocessor/seq/enum.hpp>


#include <deque>
#include <utility>
#include <string>



namespace ompu { namespace io {

namespace score_data {

struct LylicFragment
{
    boost::optional<music::concrete::ChordIdentity>
    chord; // maybe-inherited

    std::string lylic;
};

struct Blob
{
    std::deque<LylicFragment>
    frags;
};


#define OMPU_IO_SCORE_DATA_DIRECTIVE_TYPES \
    (new_song) \
    \
    (title) \
    (subtitle) \
    (artist) \
    (composer) \
    (lyricist) \
    (copyright) \
    (album) \
    (year) \
    (key) \
    (time) \
    (tempo) \
    (duration) \
    (capo) \
    (meta) \
    \
    (comment) \
    (comment_italic) \
    (comment_box) \
    (chorus) \
    (image) \
    \
    (start_of_chorus) \
    (end_of_chorus) \
    (start_of_verse) \
    (end_of_verse) \
    (start_of_tab) \
    (end_of_tab) \
    (start_of_grid) \
    (end_of_grid) \
    \
    (define) \
    (chord) \
    \
    (textfont) \
    (textsize) \
    (textcolour) \
    (chordfont) \
    (chordsize) \
    (chordcolour) \
    (tabfont) \
    (tabsize) \
    (tabcolour) \
    \
    (new_page) \
    (new_physical_page) \
    (column_break) \
    \
    (grid) \
    (no_grid) \
    (titles) \
    (columns)


enum class DirectiveType
{
    unknown = 0,
    extension, // x_*

    BOOST_PP_SEQ_ENUM(OMPU_IO_SCORE_DATA_DIRECTIVE_TYPES)
};

struct Directive
{
    DirectiveType type;
    boost::optional<std::string> value;

    boost::variant<
        boost::recursive_wrapper<Directive>,
        Blob
    > body;
};

struct Comment
{
    std::string text;
};

} // score_data


struct ChordScore
{
    using any_content_type = boost::variant<
        score_data::Blob, score_data::Directive,
        score_data::Comment
    >;

    std::deque<any_content_type>
    contents;
};

}} // ompu
