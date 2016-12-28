#include "SpaceProcess.hpp"
#include <src/Area/SingletonAreaFactoryList.hpp>
#include <src/Computation/ComputationFactoryList.hpp>

template <>
void DataStreamReader::read(
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

template <>
void DataStreamWriter::write(
        Space::ProcessModel& proc)
{
    writeTo(proc.space());

    // Areas
    int32_t a_size;
    m_stream >> a_size;
    auto& al = components.interfaces<Space::AreaFactoryList>();
    for(; a_size --> 0;)
    {
        auto area = deserialize_interface(al, *this, proc.context(), &proc);
        if(area)
            proc.areas.add(area);
        else
            ISCORE_TODO;
    }

    // Computations
    int32_t c_size;
    m_stream >> c_size;
    auto& csl = components.interfaces<Space::ComputationFactoryList>();
    for(; c_size --> 0;)
    {
        auto comp = deserialize_interface(csl, *this, proc.context(), &proc);
        if(comp)
            proc.computations.add(comp);
        else
            ISCORE_TODO;
    }

    checkDelimiter();
}



template <>
void JSONObjectReader::read(
        const Space::ProcessModel& proc)
{
    obj["Space"] = toJsonObject(proc.space());
    obj["Areas"] = toJsonArray(proc.areas);
    obj["Computations"] = toJsonArray(proc.computations);
}

template <>
void JSONObjectWriter::write(
        Space::ProcessModel& proc)
{
    JSONObject::Deserializer obj{obj["Space"].toObject()};
    obj.writeTo(proc.space());

    // Areas
    auto& areas = components.interfaces<Space::AreaFactoryList>();
    for(const auto& ar : obj["Areas"].toArray())
    {
        JSONObject::Deserializer ar_deser{ar.toObject()};
        auto area = deserialize_interface(areas, ar_deser, proc.context(), &proc);
        if(area)
            proc.areas.add(area);
        else
            ISCORE_TODO;
    }

    // Computations
    auto& comps = components.interfaces<Space::ComputationFactoryList>();
    for(const auto& comp : obj["Computations"].toArray())
    {
        JSONObject::Deserializer comp_deser{comp.toObject()};
        auto deser = deserialize_interface(comps, comp_deser, proc.context(), &proc);
        if(deser)
            proc.computations.add(deser);
        else
            ISCORE_TODO;
    }
}


