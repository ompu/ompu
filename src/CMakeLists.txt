set(OMPU_COMPILE_DEFINITIONS "")

# ------------------------------------------------------
set(OMPU_SRCS "")
function(ompu_src)
  set(srcs "")
  foreach(arg IN LISTS ARGV)
    list(APPEND srcs "${arg}.cpp")
  endforeach()
  set(OMPU_SRCS ${OMPU_SRCS} ${srcs} PARENT_SCOPE)
endfunction()

ompu_src(
  "ompu_io"
  "ompu_lib"
  "ompu_util"
  "component_id"

  "version/version"
  "version/version_set"

  "ui/component"
  "ui/synth/wheel"
  "ui/key_status"
  "ui/keyboard"
  "ui/fletboard"
  "ui/guitar"
  "ui/bass"

  "geo/geo"
  "geo/idents"
  "geo/models"
  "geo/strats"

  "midi/message"
  "midi/sysex"
  "midi/filter"
  "midi/track"
  "midi/midi_io"

  "music/concrete/error"
  "music/concrete/chord_identity"
  "music/concrete/chord"
  "music/concrete/chord_detector"
  "music/concrete/context"
  "music/concrete/interval"
  "music/concrete/key"
  "music/concrete/note"
  "music/concrete/types"
  "music/concrete/chord_score"

  "game/database"
  "game/error"
  "game/event"
  "game/event_handler"
  "game/game_data"
  "game/game_data_snapshot"
  "game/gc"
  "game/scheduler"
  "game/session"

  "io/in/chord_pro"
  "io/out/chord_pro"
  "io/reader"
  "io/writer"
)
add_library(ompu STATIC "${OMPU_SRCS}")


if(MSVC)
  link_directories("${BOOST_ROOT}/lib")
endif()

target_include_directories(ompu SYSTEM BEFORE PUBLIC "${OMPU_SYSTEM_INCLUDE_DIRS}")
target_include_directories(ompu BEFORE PUBLIC "${OMPU_INCLUDE_DIRS}")
target_link_libraries(ompu PUBLIC "${OMPU_LIBS}")
