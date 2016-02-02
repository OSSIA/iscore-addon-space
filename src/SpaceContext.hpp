#pragma once
#include <iscore/document/DocumentContext.hpp>
#include <Device/Node/DeviceNode.hpp>

namespace Explorer
{
class DeviceDocumentPlugin;
}
namespace Space
{
class SpaceModel;
struct AreaContext
{
    const iscore::DocumentContext& doc;
    const SpaceModel& space;
    Explorer::DeviceDocumentPlugin& devices;
};
}
