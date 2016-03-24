#pragma once
#include <src/Area/AreaFactory.hpp>
#include <src/Area/AreaView.hpp>
#include <src/Area/AreaPresenter.hpp>
#include <src/Area/Rectangle/Area.hpp>
namespace Space
{

using RectangleAreaFactory = AreaFactory_T<
    RectangleArea,
    RectangleAreaModel,
    AreaPresenter_T<
        RectangleArea,
        RectangleAreaModel,
        PathAreaView>,
    PathAreaView>;
}
