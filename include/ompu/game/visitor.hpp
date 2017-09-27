#pragma once

#include "ompu/game/game_fwd.hpp"
#include "ompu/game/scene.hpp"

#include <boost/variant/static_visitor.hpp>

#include <memory>


namespace ompu { namespace game {

using updater_game_data_type = GameDataSnapshot*;
using updater_return_type = scenes::all_type;

using drawer_game_data_type = std::unique_ptr<GameDataSnapshot>;
using drawer_return_type = void;


//
// All visitor classes should be in the signature below.
//
// Also: don't forget to:
//       #include "ompu/game/game_data_snapshot.hpp"
//       - because these classes are incomplete types here!
//

#if 0
// begin visitor example -----------------------
class MySceneVisitor
    : boost::static_visitor<ompu::game::(updater|drawer)_return_type>
{
public:
    constexpr static char const* name() noexcept { return "Your friendly name"; }

    // this is the snapshot for 'current' iteration,
    // can be referenced (or modified) in thread-safe manner
    void game_data(ompu::game::(updater|drawer)_game_data_type gd)
    {
        this->gd_ = std::move(gd);
    }

    ompu::game::(updater|drawer)_return_type
    operator()(/* tagged by `ompu::game::scenes::*` */ const& /* discarded */) const
    {
        // your code...

        //
        // you can use your real 'context' if you want:
        //   my_ctx_type* const real_ctx = reinterpret_cast<my_ctx_type*>(ctx_);
        //

        //
        // return the next `Scene` object to change the game scene:
        //
        //   return gd_->scene; // same as current scene
        //
        //     ...or...
        //
        //   return ompu::game::scenes::Single{}; // next scene
        //
        //     ...or...
        //
        //   return; // void, in *Draw* operation
        //
    }

    // scene dispatcher continues....

private:
    ompu::game::(updater|drawer)_game_data_type gd_;
};
// end visitor example -----------------------
#endif

}} // ompu
