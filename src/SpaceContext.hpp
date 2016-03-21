#pragma once
#include <iscore/document/DocumentContext.hpp>
#include <Device/Node/DeviceNode.hpp>
#include <src/EditionSettings.hpp>

namespace Explorer
{
class DeviceDocumentPlugin;
}
namespace Space
{
class SpaceModel;
struct Context
{
    const iscore::DocumentContext& doc;
    const SpaceModel& space;
    const EditionSettings& settings;
    Explorer::DeviceDocumentPlugin& devices;
};

}

Q_DECLARE_METATYPE(Space::Tool)
