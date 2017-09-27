#pragma once

#include <boost/integer.hpp>
#include <boost/core/ignore_unused.hpp>

#include <ratio>
#include <tuple>
#include <type_traits>


namespace ompu {
namespace geo {

template<
    class XRatio,
    class YRatio
>
class RatioPoint
{
public:
    using x_ratio = XRatio;
    using y_ratio = YRatio;
};

template<
    class WRatio,
    class HRatio
>
class RatioSize
{
public:
    using w_ratio = WRatio;
    using h_ratio = HRatio;
};

template<
    class Point,
    class Size
>
class RatioRect
{
public:
    using point_ratio = Point;
    using x_ratio = typename Point::x_ratio;
    using y_ratio = typename Point::y_ratio;

    using size_ratio = Size;
    using w_ratio = typename Size::w_ratio;
    using h_ratio = typename Size::h_ratio;
};

template<class T, class R>
inline constexpr T
static_ratio(T v) noexcept
{
    return v * (T(R::num) / R::den);
}

template<
    class XRatioPerStep,
    class YRatioPerStep,
    class WRatioPerStep,
    class HRatioPerStep
>
class RatioCalculator
{
public:
#if 0
    template<class Pos, class T, class StepT>
    static constexpr Pos
    pos_for_step(T x, T y, T w, T h, StepT step_x, StepT step_y = step_x) noexcept
    {
        return Pos{
            x + static_ratio<XRatioPerStep, T>(w) * step_x,
            y + static_ratio<YRatioPerStep, T>(h) * step_y
        };
    }

    template<class Size, class T, class StepT>
    static constexpr Size
    size_for_step(T, T, T w, T h, StepT step_w, StepT step_h = step_w) noexcept
    {
        return Size{
            static_ratio<WRatioPerStep, T>(w) * step_w,
            static_ratio<HRatioPerStep, T>(h) * step_h
        };
    }
#endif

    template<class Rect, class T>
    static constexpr Rect
    rect_for_step(T x, T y, T w, T h) noexcept
    {
        return Rect{
            x + static_ratio<T, XRatioPerStep>(w),
            y + static_ratio<T, YRatioPerStep>(h),
            static_ratio<T, WRatioPerStep>(w),
            static_ratio<T, HRatioPerStep>(h)
        };
    }
};

}
} // ompu
