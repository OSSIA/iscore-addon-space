#include "SpaceProcess.hpp"
#include <src/Area/SingletonAreaFactoryList.hpp>
#include <src/Computation/ComputationFactoryList.hpp>
template<>
void Visitor<Reader<DataStream>>::readFrom_impl(
        const Space::ProcessModel& proc)
{
    // Space definition
    readFrom(proc.space());

    // Arease
    m_stream << (int32_t)proc.areas.size();
    for(const auto& area : proc.areas)
    {
        readFrom(area);
    }

    // Computations
    m_stream << (int32_t)proc.computations.size();
    for(const auto& comp : proc.computations)
    {
        readFrom(comp);
    }

    insertDelimiter();
}

template<>
void Visitor<Writer<DataStream>>::writeTo(
        Space::ProcessModel& proc)
{
    writeTo(proc.space());

    // Areas
    int32_t a_size;
    m_stream >> a_size;
    auto& al = components.factory<Space::AreaFactoryList>();
    for(; a_size --> 0;)
    {
        proc.areas.add(deserialize_interface(al, *this, proc.context(), &proc));
    }

    // Computations
    int32_t c_size;
    m_stream >> c_size;
    auto& csl = components.factory<Space::ComputationFactoryList>();
    for(; c_size --> 0;)
    {
        proc.computations.add(deserialize_interface(csl, *this, proc.context(), &proc));
    }

    checkDelimiter();
}




template<>
void Visitor<Reader<JSONObject>>::readFrom_impl(
        const Space::ProcessModel& proc)
{
    m_obj["Space"] = toJsonObject(proc.space());
    m_obj["Areas"] = toJsonArray(proc.areas);
    m_obj["Computations"] = toJsonArray(proc.computations);
}

template<>
void Visitor<Writer<JSONObject>>::writeTo(
        Space::ProcessModel& proc)
{
    Deserializer<JSONObject> obj{m_obj["Space"].toObject()};
    obj.writeTo(proc.space());

    // Areas
    auto& areas = components.factory<Space::AreaFactoryList>();
    for(const auto& ar : m_obj["Areas"].toArray())
    {
        Deserializer<JSONObject> ar_deser{ar.toObject()};
        proc.areas.add(deserialize_interface(areas, ar_deser, proc.context(), &proc));
    }

    // Computations
    auto& comps = components.factory<Space::ComputationFactoryList>();
    for(const auto& comp : m_obj["Computations"].toArray())
    {
        Deserializer<JSONObject> comp_deser{comp.toObject()};
        proc.computations.add(deserialize_interface(comps, comp_deser, proc.context(), &proc));
    }
}


