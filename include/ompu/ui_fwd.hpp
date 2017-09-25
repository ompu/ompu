#pragma once

namespace ompu { namespace ui {

namespace tags {

struct synth_wheel_base {};

struct pitch_wheel {};
struct mod_wheel {};

} // tags

template<class, class T, T, T>
class SynthWheel;

using PitchWheel = SynthWheel<tags::pitch_wheel, int, -8192, 8191>;
using ModWheel = SynthWheel<tags::mod_wheel, unsigned, 0, 127>;

class Keyboard;

}} // ompu
