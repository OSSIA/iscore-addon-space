#pragma once

#include <src/Area/Pointer/PointerAreaModel.hpp>
#include <src/Area/AreaView.hpp>
#include <src/Area/AreaFactory.hpp>
#include <src/Area/AreaPresenter.hpp>
namespace Space
{

using PointerAreaFactory = AreaFactory_T<
    PointerArea,
    PointerAreaModel,
    AreaPresenter_T<
        PointerArea,
        PointerAreaModel,
        PathAreaView>,
    PathAreaView>;
}
