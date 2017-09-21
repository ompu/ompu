#pragma once

namespace ompu { namespace game {

struct game_error;

class GameData;
class GameDataSnapshot;

#define OMPU_GAME_TEMPLATE_PARAMS \
      class SceneUpdateVisitor \
    , class SceneDrawVisitor

// class Game;

class Event;
class EventHandler;

class GC;

}} // ompu
