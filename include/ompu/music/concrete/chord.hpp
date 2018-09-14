#include "ompu/music/concrete/error.hpp"
#include "ompu/music/concrete/note.hpp"
#include "ompu/music/concrete/chord_identity.hpp"


namespace ompu { namespace music { namespace concrete {

class Chord
{
public:
    explicit Chord(ChordIdentity ident, Note root);

    ChordIdentity const& ident() const noexcept { return ident_; }
    Note const& root() const noexcept { return root_; }

private:
    ChordIdentity ident_;
    Note root_;
};

}}} // ompu
