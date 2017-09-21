#include "ompu/game/game_data_snapshot.hpp"
#include "ompu/game/game_data.hpp"

#include <boost/core/ignore_unused.hpp>


namespace ompu { namespace game {

GameDataSnapshot::GameDataSnapshot(GameData const* gd)
    : scene(gd->scene())
    , snapshot_at(std::chrono::steady_clock::now())
    , fps(gd->fps())
{
    boost::ignore_unused(gd);

    // TODO: reserve() data
}

}} // ompu
