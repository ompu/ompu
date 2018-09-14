#include "ompu/music/concrete/chord.hpp"


namespace ompu { namespace music { namespace concrete {

Chord::Chord(ChordIdentity ident, Note root)
    : ident_(std::move(ident))
    , root_(std::move(root))
{}

}}} // ompu
