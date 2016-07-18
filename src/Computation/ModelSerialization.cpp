#include "ComputationModel.hpp"
template<>
void Visitor<Reader<DataStream>>::readFrom_impl(
        const Space::ComputationModel& area)
{
    // Save the parent class
    readFrom(static_cast<const IdentifiedObject<Space::ComputationModel>&>(area));
    m_stream << area.m_addr << area.m_a1 << area.m_a2;
}

template<>
void Visitor<Writer<DataStream>>::writeTo(
        Space::ComputationModel& area)
{
    m_stream >> area.m_addr >> area.m_a1 >> area.m_a2;
}

template<>
void Visitor<Reader<JSONObject>>::readFrom_impl(
        const Space::ComputationModel& area)
{
    // Save the parent class
    readFrom(static_cast<const IdentifiedObject<Space::ComputationModel>&>(area));
    m_obj[iscore::StringConstant().Address] = toJsonObject(area.m_addr);
    m_obj["A1"] = toJsonValue(area.m_a1);
    m_obj["A2"] = toJsonValue(area.m_a2);
}

template<>
void Visitor<Writer<JSONObject>>::writeTo(
        Space::ComputationModel& area)
{
    area.m_addr = fromJsonObject<State::Address>(m_obj[iscore::StringConstant().Address]);
    area.m_a1 = fromJsonValue<Id<Space::AreaModel>>(m_obj["A1"]);
    area.m_a2 = fromJsonValue<Id<Space::AreaModel>>(m_obj["A2"]);
}
