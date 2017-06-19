#include "AddArea.hpp"

#include "src/Area/AreaModel.hpp"
#include "src/Area/Generic/GenericAreaModel.hpp"
#include "src/Area/AreaParser.hpp"
#include "src/Space/SpaceModel.hpp"

#include "src/Area/AreaFactory.hpp"
#include "src/Area/SingletonAreaFactoryList.hpp"

#include "src/SpaceProcess.hpp"
#include "src/Area/Generic/AreaComputer.hpp"

#include <iscore/application/ApplicationContext.hpp>
#include <boost/range/algorithm/find_if.hpp>
#include <iscore/tools/IdentifierGeneration.hpp>
#include <iscore/model/path/PathSerialization.hpp>
#include <iscore/plugins/customfactory/StringFactoryKeySerialization.hpp>

#include <src/Area/Circle/CircleAreaModel.hpp>
#include <src/Area/Pointer/PointerAreaModel.hpp>

namespace Space
{
AddArea::AddArea(const Space::ProcessModel& spacProcess,
                 UuidKey<AreaFactory> type,
                 const QStringList &area,
                 const QMap<Id<DimensionModel>, QString> &dimMap,
                 const QMap<QString, Device::FullAddressSettings> &addrMap):
    m_path{spacProcess},
    m_areaType{type},
    m_areaFormula{area},
    m_spaceMap{dimMap},
    m_symbolToAddressMap{addrMap}
{
    m_createdAreaId = getStrongId(spacProcess.areas);
}

void AddArea::undo(const iscore::DocumentContext& ctx) const
{
    auto& proc = m_path.find(ctx);
    proc.areas.remove(m_createdAreaId);
}

void AddArea::redo(const iscore::DocumentContext& ctx) const
{
    auto& proc = m_path.find(ctx);

    auto factory = context.interfaces<AreaFactoryList>().get(m_areaType);
    ISCORE_ASSERT(factory);

    AreaModel* ar = factory->make(m_areaFormula, proc.context(), m_createdAreaId, &proc);

    ar->setSpaceMapping(m_spaceMap);
    ar->setParameterMapping(m_symbolToAddressMap);

    proc.areas.add(ar);
}
void AddArea::serializeImpl(DataStreamInput &s) const
{
    s << m_path << m_createdAreaId << m_areaType << m_areaFormula << m_spaceMap << m_symbolToAddressMap;
}

void AddArea::deserializeImpl(DataStreamOutput &s)
{
    s >> m_path >> m_createdAreaId >> m_areaType >> m_areaFormula >> m_spaceMap >> m_symbolToAddressMap;
}
}
