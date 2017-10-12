#pragma once

#include "ompu/music/concrete/types.hpp"
#include "ompu/music/concrete/note.hpp"

#include "ompu/music/key.hpp"


namespace ompu { namespace music { namespace concrete {

class Key
{
public:
    enum class Feel
    {
        Major, Minor,
    };

    Key() noexcept = default;

    template<class Ident, class KeyFeel>
    explicit Key(basic_key<key_ident<Ident, KeyFeel>>) noexcept
        : nh_(make_note_height(Ident{}))
        , mod_(make_mod(typename Ident::mod_type{}))
        , feel_(std::is_same_v<KeyFeel, key_feels::major> ? Feel::Major : Feel::Minor)
    {}

    explicit Key(CanonicalNoteHeight nh, ModType mod, Feel feel) noexcept
        : nh_(nh), mod_(mod), feel_(feel)
    {}

    Key(Key const&) = default;
    Key(Key&&) noexcept = default;
    ~Key() = default;

    Key& operator=(Key const&) = default;
    Key& operator=(Key&&) noexcept = default;

    std::string name() const;

    note_height_type height() const noexcept { return static_cast<std::underlying_type_t<CanonicalNoteHeight>>(nh_) + mod_value(mod_); }
    Feel feel() const noexcept { return feel_; }

    bool is_major() const noexcept { return feel_ == Feel::Major; }
    bool is_minor() const noexcept { return feel_ == Feel::Minor; }

    friend std::ostream& operator<<(std::ostream& os, Key const& key);

private:
    CanonicalNoteHeight nh_;
    ModType mod_;
    Feel feel_;
};

std::ostream& operator<<(std::ostream& os, Key::Feel const& kf);

}}} // ompu
