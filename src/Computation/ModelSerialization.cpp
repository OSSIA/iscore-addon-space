#include "ComputationModel.hpp"

template <>
void DataStreamReader::read(
        const Space::ComputationModel& area)
{
    m_stream << area.m_addr << area.m_a1 << area.m_a2;
}

template <>
void DataStreamWriter::writeTo(
        Space::ComputationModel& area)
{
    m_stream >> area.m_addr >> area.m_a1 >> area.m_a2;
}

template <>
void JSONObjectReader::read(
        const Space::ComputationModel& area)
{
    obj[strings.Address] = toJsonObject(area.m_addr);
    obj["A1"] = toJsonValue(area.m_a1);
    obj["A2"] = toJsonValue(area.m_a2);
}

template <>
void JSONObjectWriter::writeTo(
        Space::ComputationModel& area)
{
    area.m_addr = fromJsonObject<State::Address>(obj[strings.Address]);
    area.m_a1 = fromJsonValue<Id<Space::AreaModel>>(obj["A1"]);
    area.m_a2 = fromJsonValue<Id<Space::AreaModel>>(obj["A2"]);
}
