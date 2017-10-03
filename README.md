# ompu

![ompu](https://github.com/ompu/ompu-org-assets/raw/master/img/ompu-org/ompu-logo-2x-trans.png)

Music Game for Musicians - https://ompu.io/

Repository for [ompu/ompu](https://github.com/ompu/ompu), a.k.a. __*ompu-core*__.


## Status

Currently under **open alpha**.  
If you have any questions, [file an issue](https://github.com/ompu/ompu/issues) or please send an email to: support@ompu.io


## Concepts

1. Free, [open-source licensed (MIT)](LICENSE)
2. Everything is a __*Tag object*__, which basically means:
   1. All components (i.e. classes) are designed to be lightweight enough to instantiate for heavy runtime data structures
3. Everything can be __*static*__,  based on compile-time type-traits
   1. __*Abstract components*__ are defined as `std::tuple<Ts...>` and/or `std::integer_sequence<T, T...>` sequences
   2. __*Concrete components*__ are defined as `boost::variant<Ts...>` sequences
4. Everything can be __*statically dynamic*__, based on compile-time __*Tag Dispatch*__
   1. This can be achieved with __*concrete components*__ and `boost::static_visitor<your_return_type>`

The fundamental components are:

1. [Modern Music Theory & Church Mode components](https://github.com/ompu/ompu/wiki/Component-(Music)) (`ompu::music::*`)

All concepts apply to every classes in this library, with some exceptions:

1. Components to deal specifically with runtime I/O and graphics:
   1. I/O related (e.g. `ompu::net::*`)
   2. Game related (e.g. `ompu::game::*`, `ompu::geo::*`)
2. Components to deal with runtime data:
   1. MIDI related (`ompu::midi::*`)
3. Concrete components (e.g. `ompu::music::concretes::*`)
4. Perpetuation interfaces

Also see the [ompu Wiki](https://github.com/ompu/ompu/wiki).

---

## Roadmap (v1)

- [ ] The game
- [ ] Basic music theory support
- [ ] BPM detection
- [ ] Key detection
- [ ] Chord detection
- [ ] Scale suggestion

## Roadmap (v-future)

- [ ] Auto generation for musical scores
- [ ] Network session (live mode)
- [ ] Network session (session mode)
- [ ] OSC protocol support
- [ ] ompu hardware


## Requirements

- [saya](https://github.com/saya-io/saya) ~> __latest__
- [Boost C++ Libraries](http://www.boost.org/) ~> __1.65.1__
- Decent C++ compiler >= __C++17__
  - Windows: Visual Studio ~> __2017__
  - Mac: Not tested; future support for clang
  - Linux: Not tested; future support for clang


## Compatibility information

Tested with 3rd party libraries listed below:

> Note: This section aims to provide some real-world usage compatibility;
> ompu has *zero dependencies* to these libraries.
> Please do not file ompu-related tickets to any other 3rd libraries!

- [JUCE library](https://www.juce.com/) (~> __v5.1.2__)


## Requirements (definitions)

### All

|name|value|
|---|---|
|`BOOST_SCOPE_EXIT_CONFIG_USE_LAMBDAS`|`1`|

### MSVC only

|name|value|
|---|---|
|`NOMINMAX`|(no value)|
|`_HAS_AUTO_PTR_ETC`|`1`|

---

## Links

- ompu project home: https://ompu.io/
- Official source code repo: https://github.com/ompu/ompu
- ompu forum: (coming soon)

## About

- Project page: https://ompu.io/
- Original source: https://github.com/ompu/ompu
- License: [MIT](LICENSE)
