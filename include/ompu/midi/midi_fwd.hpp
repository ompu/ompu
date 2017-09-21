#pragma once

#define OMPU_MIDI_CH_MAX 16

#define OMPU_MIDI_IN_BUF_NUM 256
#define OMPU_MIDI_IN_SYSEX_BUF_NUM 256


namespace ompu { namespace midi {

class Message;

#if 0
class Sysex;
#endif

class Filter;
class FilterSet;

class Track;

}} // ompu
