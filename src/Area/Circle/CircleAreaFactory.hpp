#pragma once
#include "src/Area/AreaFactory.hpp"

#include "CircleAreaModel.hpp"
#include <src/Area/AreaView.hpp>
#include <src/Area/AreaPresenter.hpp>
namespace Space
{

using CircleAreaFactory = AreaFactory_T<
    CircleArea,
    CircleAreaModel,
    AreaPresenter_T<
        CircleArea,
        CircleAreaModel,
        PathAreaView>,
    PathAreaView>;
}
