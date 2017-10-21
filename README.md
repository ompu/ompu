# ompu

![ompu](https://github.com/ompu/ompu-org-assets/raw/master/img/ompu-org/ompu-logo-2x-trans.png)

Music Game for Musicians - https://ompu.io/

Repository for [ompu/ompu](https://github.com/ompu/ompu), a.k.a. __*ompu-core*__.


## Status

Currently under **open alpha**.  
If you have any questions, [file an issue](https://github.com/ompu/ompu/issues) or please send an email to: support@ompu.io


## ompu::music Concepts

1. No heavy SFINAE, just specializations
2. No heavy preprocessors
3. Everything is __*static*__, determined at compile-time
4. Sequences are defined as Tuples and/or Integer-sequences


## Components

1. [Modern Music Theory & Church Mode components](https://github.com/ompu/ompu/wiki/Component-(Music)) (`ompu::music::*`)
2. File/stream I/O (`ompu::io::*`)
3. Network I/O (`ompu::net::*`)
4. Game graphics and geometry (`ompu::game::*`, `ompu::geo::*`)
5. MIDI (`ompu::midi::*`)
6. Concrete components for runtime data (`ompu::music::concrete::*`)
7. Bridge for 3rd party libraries (`ompu::juicy::*`)

Also see the [ompu Wiki](https://github.com/ompu/ompu/wiki).

---

## Roadmap (v1)

- [ ] The game
- [x] Basic music theory support
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

## Building

```bash
git submodule update --init
mkdir build
cd build
cmake -G "Visual Studio 15 2017 Win64" -DBOOST_ROOT=path/to/boost ..
```

## Requirements

- [saya](https://github.com/saya-io/saya) ~> __latest__
- [Boost C++ Libraries](http://www.boost.org/) ~> __1.65.1__
- [Sprout](https://github.com/bolero-MURAKAMI/Sprout) ~> __latest__
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
