#pragma once

namespace ompu { namespace game {

struct game_error;

class GameData;
class GameDataSnapshot;

#define OMPU_GAME_TEMPLATE_PARAMS \
      class Updater \
    , class Drawer

// class Game;

class Event;
class EventHandler;

class GC;

}} // ompu
