#include "ComputationModel.hpp"

namespace Space
{
ComputationModel::ComputationModel(
        const Id<AreaModel>& a1,
        const Id<AreaModel>& a2,
        const SpaceModel& space,
        const Id<ComputationModel>& id,
        QObject* parent):
    IdentifiedObject{id, staticMetaObject.className(), parent},
    m_space{space},
    m_a1{a1},
    m_a2{a2}
{
    metadata.setName(QString("Computation.%1").arg(*this->id().val()));
}

ComputationModel::ComputationModel(
        const ComputationModel& source,
        const SpaceModel& space,
        const Id<ComputationModel>& id,
        QObject* parent):
    IdentifiedObject{id, staticMetaObject.className(), parent},
    m_space{space},
    m_addr{source.m_addr},
    m_a1{Id<AreaModel>{source.m_a1.val()}},
    m_a2{Id<AreaModel>{source.m_a2.val()}}
{

}
}
