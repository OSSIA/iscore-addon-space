#include "ComputationModel.hpp"

template<>
void Visitor<Reader<DataStream>>::readFrom_impl(
        const Space::ComputationModel& area)
{
    // Save the parent class
    readFrom(static_cast<const IdentifiedObject<Space::ComputationModel>&>(area));
    m_stream << area.m_a1 << area.m_a2;
}

template<>
void Visitor<Writer<DataStream>>::writeTo(
        Space::ComputationModel& area)
{
    m_stream >> area.m_a1 >> area.m_a2;
}

template<>
void Visitor<Reader<JSONObject>>::readFrom_impl(
        const Space::ComputationModel& area)
{
    // Save the parent class
    readFrom(static_cast<const IdentifiedObject<Space::ComputationModel>&>(area));
    m_obj["A1"] = toJsonValue(area.m_a1);
    m_obj["A2"] = toJsonValue(area.m_a2);
}

template<>
void Visitor<Writer<JSONObject>>::writeTo(
        Space::ComputationModel& area)
{
    area.m_a1 = fromJsonValue<Id<Space::AreaModel>>(m_obj["A1"]);
    area.m_a2 = fromJsonValue<Id<Space::AreaModel>>(m_obj["A2"]);
}

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
}
