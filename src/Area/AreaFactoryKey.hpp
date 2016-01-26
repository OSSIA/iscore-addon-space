#pragma once
#include <QMetaType>
#include <iscore/plugins/customfactory/UuidKey.hpp>
namespace Space
{
class AreaModel;
using AreaFactoryKey = UuidKey<AreaModel>;
}
Q_DECLARE_METATYPE(Space::AreaFactoryKey)
