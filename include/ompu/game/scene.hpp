#pragma once

#include "ompu/game/game_fwd.hpp"

#include <boost/variant/variant.hpp>
#include <boost/variant/static_visitor.hpp>

#include <boost/preprocessor/stringize.hpp>
#include <boost/preprocessor/seq/for_each.hpp>
#include <boost/preprocessor/seq/enum.hpp>


namespace ompu { namespace game {

namespace scenes {

#define OMPU_GAME_SCENES_DEF \
    (Noop)(Single)
    // (Session)

#define OMPU_DEF(r, data, elem) \
    struct elem {};

BOOST_PP_SEQ_FOR_EACH(OMPU_DEF, _, OMPU_GAME_SCENES_DEF)
#undef OMPU_DEF

using all_type = boost::variant<
    BOOST_PP_SEQ_ENUM(OMPU_GAME_SCENES_DEF)
>;

} // scenes

class SceneNameVisitor : public boost::static_visitor<char const*>
{
public:
    using return_type = char const*;

#define OMPU_DEF(r, data, elem) \
    return_type operator()(scenes::elem const&) const noexcept \
    { \
        return BOOST_PP_STRINGIZE(elem); \
    }

    BOOST_PP_SEQ_FOR_EACH(OMPU_DEF, _, OMPU_GAME_SCENES_DEF)
#undef OMPU_DEF
};

}} // ompu
