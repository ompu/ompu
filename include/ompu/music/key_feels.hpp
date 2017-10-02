#pragma once

#include "ompu/music_fwd.hpp"


namespace ompu { namespace music {

namespace key_feels {

struct Major {};
struct Minor {};

} // key_feels


template<class KeyFeel> struct is_major_feel;
template<> struct is_major_feel<key_feels::Major> : std::true_type {};
template<> struct is_major_feel<key_feels::Minor> : std::false_type {};
template<class KeyFeel> constexpr auto is_major_feel_v = is_major_feel<KeyFeel>::value;

template<class KeyFeel> struct is_minor_feel;
template<> struct is_minor_feel<key_feels::Major> : std::false_type {};
template<> struct is_minor_feel<key_feels::Minor> : std::true_type {};
template<class KeyFeel> constexpr auto is_minor_feel_v = is_minor_feel<KeyFeel>::value;

}} // ompu
