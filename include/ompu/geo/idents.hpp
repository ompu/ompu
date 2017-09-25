#pragma once

#include "ompu/geo/geo.hpp"


namespace ompu { namespace geo { namespace idents {

struct Box
{
    geo::Point pt;
    geo::Size size;

    Box() noexcept = default;
    Box(geo::Point pt, geo::Size size) noexcept;

    double x() const noexcept;
    double y() const noexcept;

protected:
    void cache_ident();

private:
    geo::Box cache_;
};

struct Circle
{
    geo::Point pt;
    geo::CircleSize size;

    Circle() noexcept = default;
    Circle(geo::Point pt, geo::CircleSize size) noexcept;

    double x() const noexcept;
    double y() const noexcept;

protected:
    void cache_ident();

private:
    // geo::MultiPoint cache_;
};

}}} // ompu
