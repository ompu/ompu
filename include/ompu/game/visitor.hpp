#pragma once

#include "ompu/game/game_fwd.hpp"
#include "ompu/game/scene.hpp"

#include <boost/variant/static_visitor.hpp>


namespace ompu { namespace game {

using update_visitor_return_type = scenes::all_type;
using draw_visitor_return_type = void;

//
// All visitor classes should be in the signature below.
//
// Also: don't forget to:
//       #include "ompu/game/game_data.hpp"
//       #include "ompu/game/scene.hpp"
//       because these classes are incomplete types here!
//

#if 0 // begin visitor example -----------------------
class MySceneVisitor
    : boost::static_visitor<ompu::game::(update|draw)_visitor_return_type>
{
public:
    explicit MyVisitor(ompu::game::GameData /* maybe-const, depends on (update|draw) */ * gd)
        : gd_(gd) // this is the GameData snapshot for 'current' iteration,
                  // can be referenced (or modified) in thread-safe manner
    {}

    constexpr static char const* name() noexcept { return "Your friendly name"; }

    ompu::game::(update|draw)_visitor_return_type
        operator()(/* tagged by `ompu::game::scenes::*` */ const& /* discarded */) const
    {
        // your code...

        // if this visitor is for *Update* operation,
        // you must return the next `State` object
        //
        //  return ompu::game::scenes::Noop{};
        //
    }

    // scene dispatcher continues....

private:
    ompu::game::GameData /* maybe-const */ * gd_;
};
#endif // end visitor example -----------------------

}} // ompu
