#include "ompu/music/concrete/chord.hpp"

#include <boost/range/algorithm/count_if.hpp>
#include <boost/range/adaptor/indexed.hpp>

#include <boost/assert.hpp>


namespace ompu { namespace music { namespace concrete {

Chord::Chord(Note root_note) noexcept
    : notes_{std::move(root_note)}
    , tag_{std::addressof(*notes_.begin())}
{}

void Chord::set_root(Note root_note) noexcept
{
    // no re-assignment
    BOOST_ASSERT(!tag_.cn1);
    auto const inserted = notes_.emplace(std::move(root_note));
    BOOST_ASSERT(inserted.second);

    tag_.cn1 = std::addressof(*inserted.first);
}

void Chord::auto_set_root() noexcept
{
    BOOST_ASSERT(!notes_.empty());
    tag_.cn1 = std::addressof(*notes_.begin());
}

void Chord::add(Note note)
{
    BOOST_ASSERT(tag_.cn1);

    {
        auto const inserted = notes_.emplace(std::move(note));
        if (!inserted.second) {
            throw too_many_notes();
        }
    }

    reorganize();
}

void Chord::reorganize()
{
    BOOST_ASSERT(tag_.cn1);

    auto root_height = tag_.cn1->height();
    //auto root_octave = tag_.cn1->octave();

    if (is_on_chord()) {
        auto const on_root = notes_.cbegin();
        root_height = (tag_.cn1->height() % 12) + on_root->octave() * 12;
        //root_octave = on_root->octave();
    }

    // tag for current dynamic chord notes
    DynamicTagger dtag;

    for (auto const& note : notes_) {
        // skip root note
        auto const* const note_ptr = std::addressof(note);
        if (note_ptr == tag_.cn1) continue;

        bool const is_tension = (note.height() - root_height) >= 12;

        if (is_tension) {
            if (tensions_.size() >= 2) {
                throw too_many_tensions();
            }

            tensions_.push_back(note_ptr);
            continue;
        }

        auto const intv = make_interval(root_height, note.height());
        BOOST_ASSERT(intv.semitones() != 0 && !"zero interval on chord detection");

        if (dtag.cns[intv.semitones() - 1]) {
            throw disharmony_error();
        }
        dtag.cns[intv.semitones()] = note_ptr;
    }

    if (boost::range::count_if(dtag.cns, [] (auto const* const cn) -> bool {
        return cn;
    }) > 4) {
        throw too_many_notes();
    }

    for (auto const cn : dtag.cns | boost::adaptors::indexed(0)) {
        if (!cn.value()) continue;

        switch (cn.index()) {
        case 0:
            BOOST_ASSERT(false);
            throw;

        case 1: // b9
            // ignore
            break;

        case 2: // add9
            // ignore
            break;

        case 3: // m3
            tag_.cn3 = cn.value();
            break;

        case 4: // M3
            if (tag_.cn3) {
                throw disharmony_error("got M3 for minor chord");
            }
            tag_.cn3 = cn.value();
            break;

        case 5: // P4
            if (tag_.cn3) {
                throw disharmony_error("got P4 for triad");
            }
            break;

        case 6: // b5
            if (dtag.cns[5]) {
                throw disharmony_error("got b5 for sus4");
            }
            tag_.cn5 = cn.value();
            break;

        case 7: // P5
            if (tag_.cn5) {
                throw disharmony_error("got P5 for b5");
            }
            tag_.cn5 = cn.value();
            break;

        case 8: // m6
            if (dtag.cns[9] || dtag.cns[10] || dtag.cns[11]) {
                throw disharmony_error("got m6 for 7th chord");
            }
            tag_.cn6 = cn.value();
            break;

        case 9: // M6 or dim7
            if (dtag.cns[6]) { // already have b5?
                tag_.cn7 = cn.value();
            } else {
                tag_.cn6 = cn.value();
            }
            break;

        case 10: // m7
            if (tag_.cn6 || tag_.cn7) {
                throw disharmony_error("got extra m7 for already-6th-or-7th-chord");
            }
            tag_.cn7 = cn.value();
            break;

        case 11: // M7
            if (tag_.cn6 || tag_.cn7) {
                throw disharmony_error("got extra M7 for already-6th-or-7th-chord");
            }
            tag_.cn7 = cn.value();
            break;

        default:
            BOOST_ASSERT(false);
            throw;
        }
    } // dtag semitone detection
} // reorganize()


bool Chord::is_power_chord() const noexcept
{
    return !tag_.cn3 && !tag_.cn6 && !tag_.cn7;
}

bool Chord::is_triad() const noexcept
{
    return tag_.cn3 && !tag_.cn6 && !tag_.cn7;
}

bool Chord::is_tetrad() const noexcept
{
    return tag_.cn3 && !tag_.cn6 && tag_.cn7;
}

bool Chord::is_on_chord() const noexcept
{
    BOOST_ASSERT(tag_.cn1);
    return tag_.cn1 != std::addressof(*notes_.begin());
}


Note const& Chord::note_1() const noexcept
{
    BOOST_ASSERT(tag_.cn1); return *tag_.cn1;
}

Note const& Chord::note_3() const noexcept
{
    BOOST_ASSERT(tag_.cn3); return *tag_.cn3;
}

Note const& Chord::note_5() const noexcept
{
    BOOST_ASSERT(tag_.cn5); return *tag_.cn5;
}

Note const& Chord::note_6() const noexcept
{
    BOOST_ASSERT(tag_.cn6); return *tag_.cn6;
}

Note const& Chord::note_7() const noexcept
{
    BOOST_ASSERT(tag_.cn7); return *tag_.cn7;
}

std::vector<Note>
Chord::valid_notes() const
{
    std::vector<Note> res;
    res.reserve(5);

    res.push_back(note_1());
    if (tag_.cn3) { res.push_back(*tag_.cn3); }
    res.push_back(note_5());
    if (tag_.cn6) { res.push_back(*tag_.cn6); }
    if (tag_.cn7) { res.push_back(*tag_.cn7); }

    res.shrink_to_fit();
    return res;
}


}}} // ompu

