#include "ComputationModel.hpp"

namespace Space
{
ComputationModel::ComputationModel(
        const Id<AreaModel>& a1,
        const Id<AreaModel>& a2,
        const SpaceModel& space,
        const Id<ComputationModel>& id,
        QObject* parent):
    Entity{id, staticMetaObject.className(), parent},
    m_space{space},
    m_a1{a1},
    m_a2{a2}
{
    metadata().setInstanceName(*this);
}

ComputationModel::ComputationModel(
        const ComputationModel& source,
        const SpaceModel& space,
        const Id<ComputationModel>& id,
        QObject* parent):
    Entity{source, id, staticMetaObject.className(), parent},
    m_space{space},
    m_addr{source.m_addr},
    m_a1{Id<AreaModel>{source.m_a1.val()}},
    m_a2{Id<AreaModel>{source.m_a2.val()}}
{
    metadata().setInstanceName(*this);
}
}
