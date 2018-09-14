#include "ompu/music/concrete/chord.hpp"

#include <boost/range/algorithm/count_if.hpp>
#include <boost/range/adaptor/indexed.hpp>

#include <boost/format.hpp>
#include <boost/assert.hpp>

#include <sstream>
#include <iostream>


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
            if (tensions_.size() >= 1) {
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

    // auto-assign trivial tensions
    if (dtag.cns[14]) {
        tag_.cn9 = dtag.cns[14];
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
            if (dtag.cns[1]) {
                throw disharmony_error();
            }
            break;

        case 3: // m3
            if (dtag.cns[2]) {
                throw disharmony_error();
            }
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


bool Chord::is_valid() const noexcept
{
    // a chord must at least be a power chord
    return notes_.size() >= 2 && tag_.cn1 && tag_.cn5;
}

bool Chord::is_power_chord() const noexcept
{
    return !tag_.cn3 && !tag_.cn6 && !tag_.cn7;
}

bool Chord::is_triad() const noexcept
{
    return notes_.size() == 3;
}

bool Chord::is_tetrad() const noexcept
{
    return notes_.size() == 4;
}

bool Chord::is_on_chord() const noexcept
{
    BOOST_ASSERT(tag_.cn1);
    return tag_.cn1 != std::addressof(*notes_.begin());
}

bool Chord::is_sixth_chord() const noexcept
{
    return tag_.cn6;
}

bool Chord::is_seventh_chord() const noexcept
{
    return tag_.cn7;
}


bool Chord::is_major() const noexcept
{
    BOOST_ASSERT(tag_.cn3);
    return distance_from_root(tag_.cn3) == 4;
}
bool Chord::is_minor() const noexcept
{
    BOOST_ASSERT(tag_.cn3);
    return distance_from_root(tag_.cn3) == 3;
}

bool Chord::has_b5() const noexcept
{
    BOOST_ASSERT(tag_.cn5);
    return distance_from_root(tag_.cn5) == 6;
}
bool Chord::has_aug5() const noexcept
{
    BOOST_ASSERT(tag_.cn5);
    return distance_from_root(tag_.cn5) == 8;
}
bool Chord::has_dim7() const noexcept
{
    BOOST_ASSERT(tag_.cn7);
    return distance_from_root(tag_.cn7) == 9;
}
bool Chord::has_m7() const noexcept
{
    BOOST_ASSERT(tag_.cn7);
    return distance_from_root(tag_.cn7) == 10;
}
bool Chord::has_M7() const noexcept
{
    BOOST_ASSERT(tag_.cn7);
    return distance_from_root(tag_.cn7) == 11;
}
bool Chord::has_add9() const noexcept
{
    BOOST_ASSERT(is_valid());
    return tag_.cn9;
}
bool Chord::has_tension() const noexcept
{
    BOOST_ASSERT(is_valid());
    return !tensions_.empty();
}


Note const& Chord::bass() const noexcept
{
    BOOST_ASSERT(!notes_.empty());
    BOOST_ASSERT(tag_.cn1);
    return *notes_.cbegin();
}

Note const& Chord::note_1() const noexcept
{
    BOOST_ASSERT(!notes_.empty());
    BOOST_ASSERT(tag_.cn1); return *tag_.cn1;
}

Note const& Chord::note_3() const noexcept
{
    BOOST_ASSERT(!notes_.empty());
    BOOST_ASSERT(tag_.cn3); return *tag_.cn3;
}

Note const& Chord::note_5() const noexcept
{
    BOOST_ASSERT(!notes_.empty());
    BOOST_ASSERT(tag_.cn5); return *tag_.cn5;
}

Note const& Chord::note_6() const noexcept
{
    BOOST_ASSERT(!notes_.empty());
    BOOST_ASSERT(tag_.cn6); return *tag_.cn6;
}

Note const& Chord::note_7() const noexcept
{
    BOOST_ASSERT(!notes_.empty());
    BOOST_ASSERT(tag_.cn7); return *tag_.cn7;
}

std::vector<Note>
Chord::valid_notes() const
{
    BOOST_ASSERT(!notes_.empty());
    BOOST_ASSERT(tag_.cn1);

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

note_height_type Chord::distance_from_root(Note const* to_note) const noexcept
{
    BOOST_ASSERT(is_valid());
    return note_1().height() - to_note->height();
}

char const* Chord::the_7_or_9() const noexcept
{
    BOOST_ASSERT(is_valid());
    BOOST_ASSERT(tag_.cn7);
    return has_add9() ? "9" : "7";
}


char const* Chord::the_tension() const noexcept
{
    BOOST_ASSERT(has_tension());
    switch (distance_from_root(tensions_.front())) {
    case 13:
        return "b9";

    // case 14:
        // return "add9";

    case 17:
        return "11";

    case 18:
        return "#11";

    case 21:
        return "b13";

    case 22:
        return "13";

    case 23:
        return "#13";
    }
    BOOST_ASSERT(!"unhandled tension");
    return "";
}


std::ostream& operator<<(std::ostream& os, Chord const& ch)
{
    BOOST_ASSERT(ch.is_valid());

    // always print the root
    os << ch.note_1();

    // C5
    if (ch.is_power_chord()) {
        return os << "5";
    }

    if (ch.is_seventh_chord()) {
        if (ch.has_dim7()) {
            BOOST_ASSERT(ch.is_minor());
            BOOST_ASSERT(ch.has_b5());
            os << "dim7";

        } else if (ch.has_m7()) {
            if (ch.is_major()) {
                if (ch.has_b5()) {
                    os << ch.the_7_or_9() << "-5";

                } else if (ch.has_aug5()) {
                    os << "aug" << ch.the_7_or_9();

                } else {
                    os << ch.the_7_or_9() << ch.the_tension();
                }

            } else {
                os << "m" << ch.the_7_or_9();
            }

        } else if (ch.has_M7()) {
            if (ch.is_major()) {
                os << "M" << ch.the_7_or_9();
            } else {
                os << "mM" << ch.the_7_or_9();
            }
        }

    } else if (ch.is_sixth_chord()) {
        if (ch.is_minor()) {
            os << "m";
        }
        os << "6";

        if (ch.has_add9()) {
            os << "9";
        }

    } else {
        if (ch.has_b5()) {
            if (ch.is_major()) {
                os << "b5";
            } else {
                os << "dim";
            }
        } else if (ch.has_aug5()) {
            BOOST_ASSERT(ch.is_major());
            os << "aug";

        } else {
            if (ch.is_major()) {
                // just major
            } else {
                os << "m";
            }
        }
    }

    std::ostringstream oss;
    oss << "[BUG] attempt to print an unhandled chord type (";
    oss << "notes: [";
    for (auto const& note : ch.notes_) {
        oss << " " << note.height();
    }
    oss << "])";
    throw std::logic_error(oss.str());
    BOOST_ASSERT(false);
}

}}} // ompu

