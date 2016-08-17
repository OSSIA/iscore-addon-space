#pragma once
#include <src/SpaceProcess.hpp>
#include <Process/LayerModel.hpp>

namespace Space
{
using Layer = Process::LayerModel_T<ProcessModel>;
}

DEFAULT_MODEL_METADATA(Space::Layer, "SpaceLayer")
