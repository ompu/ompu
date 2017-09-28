# ompu

![ompu](https://github.com/ompu/ompu-org-assets/raw/master/img/ompu-org/ompu-logo-2x-trans.png)

Music Game for Musicians - https://ompu.io/

## Status

Currently under **closed alpha**.  
If you have any questions, please send an email to: support@ompu.io

## Requirements

- [saya](https://github.com/saya-io/saya) ~> __latest__
- [Boost C++ Libraries](http://www.boost.org/) ~> __1.65.1__
- Decent C++ compiler >= __C++17__
  - Windows: Visual Studio ~> __2017__
  - Mac: Not tested; future support for clang
  - Linux: Not tested; future support for clang

Note: while ompu is fully independent from [The JUCE library](https://www.juce.com/) (~> __v5.1.2__), the API is designed to play nicely with JUCE.

## Requirements (definitions)

### All

|name|value|
|---|---|
|`BOOST_SCOPE_EXIT_CONFIG_USE_LAMBDAS`|`1`|

### MSVC only

|name|value|
|---|---|
|NOMINMAX|(no value)|
|`_HAS_AUTO_PTR_ETC`|`1`|

## About

- Website: https://ompu.io/
- Original source: https://github.com/ompu/ompu
- License: [MIT](LICENSE)

