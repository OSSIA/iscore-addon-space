#include "AddComputation.hpp"
#include <src/SpaceProcess.hpp>
#include <src/Computation/ComputationModel.hpp>
#include <src/Computation/ComputationFactoryList.hpp>
#include <iscore/tools/IdentifierGeneration.hpp>
#include <iscore/model/path/PathSerialization.hpp>

namespace Space
{
AddComputation::AddComputation(
    const Space::ProcessModel& spacProcess,
    UuidKey<ComputationFactory> type,
    const Id<AreaModel>& a1,
    const Id<AreaModel>& a2,
    const State::Address& addr):
  m_path{spacProcess},
  m_type{type},
  m_area1{a1},
  m_area2{a2},
  m_addr{addr}
{
    m_createdCompId = getStrongId(spacProcess.computations);
}

void AddComputation::undo(const iscore::DocumentContext& ctx) const
{
    auto& proc = m_path.find(ctx);
    proc.computations.remove(m_createdCompId);
}

void AddComputation::redo(const iscore::DocumentContext& ctx) const
{
    auto& proc = m_path.find(ctx);

    auto factory = context.interfaces<ComputationFactoryList>().get(m_type);
    ISCORE_ASSERT(factory);

    ComputationModel* comp = factory->make(
                             proc.areas.at(m_area1),
                             proc.areas.at(m_area2),
                             proc.context(),
                             m_createdCompId,
                             &proc);

    comp->setAddress(m_addr);

    proc.computations.add(comp);
}
void AddComputation::serializeImpl(DataStreamInput &s) const
{
    s << m_path << m_createdCompId << m_type << m_area1 << m_area2 << m_addr;
}

void AddComputation::deserializeImpl(DataStreamOutput &s)
{
    s >> m_path >> m_createdCompId >> m_type >> m_area1 >> m_area2 >> m_addr;
}
}
