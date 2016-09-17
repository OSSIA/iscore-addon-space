#pragma once
#include <src/SpaceProcess.hpp>
#include <Process/LayerModel.hpp>

namespace Space
{
using Layer = Process::LayerModel_T<ProcessModel>;
}

LAYER_METADATA(
        ,
        Space::Layer,
        "abe4d314-92dc-4746-a4a1-819d06341905",
        "SpaceLayer",
        "SpaceLayer"
        )
