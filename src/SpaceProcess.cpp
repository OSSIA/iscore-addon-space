#include "SpaceProcess.hpp"
#include "SpaceLayerModel.hpp"
#include "Area/AreaParser.hpp"
#include "Space/SpaceModel.hpp"
#include "Area/Circle/CircleAreaModel.hpp"
#include "Area/Pointer/PointerAreaModel.hpp"
#include <src/Executor/ProcessExecutor.hpp>
#include <Explorer/DocumentPlugin/DeviceDocumentPlugin.hpp>
#include <src/ApplicationPlugin.hpp>
#include <src/Area/SingletonAreaFactoryList.hpp>
#include <src/Computation/ComputationFactoryList.hpp>
#include <iscore/plugins/documentdelegate/plugin/ElementPluginModelList.hpp>
#include <iscore/serialization/VisitorCommon.hpp>
#include <src/Space/SpaceModel.hpp>

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
    auto& al = context.components.factory<Space::AreaFactoryList>();
    for(; a_size --> 0;)
    {
        proc.areas.add(deserialize_interface(al, *this, proc.context(), &proc));
    }

    // Computations
    int32_t c_size;
    m_stream >> c_size;
    auto& csl = context.components.factory<Space::ComputationFactoryList>();
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
    auto& areas = context.components.factory<Space::AreaFactoryList>();
    for(const auto& ar : m_obj["Areas"].toArray())
    {
        Deserializer<JSONObject> ar_deser{ar.toObject()};
        proc.areas.add(deserialize_interface(areas, ar_deser, proc.context(), &proc));
    }

    // Computations
    auto& comps = context.components.factory<Space::ComputationFactoryList>();
    for(const auto& comp : m_obj["Computations"].toArray())
    {
        Deserializer<JSONObject> comp_deser{comp.toObject()};
        proc.computations.add(deserialize_interface(comps, comp_deser, proc.context(), &proc));
    }
}



namespace Space
{

void ProcessModel::serialize_impl(const VisitorVariant& vis) const
{
    serialize_dyn(vis, *this);
}

Process::LayerModel* ProcessModel::loadLayer_impl(
        const VisitorVariant& vis,
        QObject* parent)
{
    return deserialize_dyn(vis, [&] (auto&& deserializer)
    {
        auto proc = new LayerModel{
                        deserializer, *this, parent};

        return proc;
    });
}


Context makeContext(const iscore::DocumentContext &doc, ProcessModel &sp)
{
    return Context{doc, sp.space(), sp,
                doc.app.components.applicationPlugin<ApplicationPlugin>().settings(),
                doc.plugin<Explorer::DeviceDocumentPlugin>()};
}

ProcessModel::ProcessModel(
        const iscore::DocumentContext& doc,
        const TimeValue &duration,
        const Id<Process::ProcessModel> &id,
        QObject *parent):
    Process::ProcessModel{id, ProcessMetadata::objectName(), parent},
    m_space{new SpaceModel{
            Id<SpaceModel>(0),
            this}},
    m_context{makeContext(doc, *this)}
{
    metadata.setName(QString("Space.%1").arg(*this->id().val()));
    using namespace GiNaC;
    using namespace spacelib;

    auto x_dim = new DimensionModel{"x", Id<DimensionModel>{0}, m_space};
    x_dim->setMin(-100);
    x_dim->setMax(100);
    auto y_dim = new DimensionModel{"y", Id<DimensionModel>{1}, m_space};
    y_dim->setMin(-100);
    y_dim->setMax(100);

    m_space->addDimension(x_dim);
    m_space->addDimension(y_dim);

    auto vp = new ViewportModel{Id<ViewportModel>{0}, m_space};
    m_space->addViewport(vp);

    connect(m_space, &SpaceModel::spaceChanged,
            this, [=] () {
        for(auto& area : areas)
            area.areaChanged(area.currentMapping());
    });

    setDuration(duration);
}

ProcessModel::ProcessModel(
        const ProcessModel& source,
        const iscore::DocumentContext& doc,
        const Id<Process::ProcessModel> &id,
        QObject *parent):
    Process::ProcessModel{id, ProcessMetadata::objectName(), parent},
    m_space{new SpaceModel{
            *source.m_space,
            this}},
    m_context{makeContext(doc, *this)}
{
    metadata.setName(QString("Space.%1").arg(*this->id().val()));

    for(auto& area : source.areas)
    {
        this->areas.add(area.clone(m_context, area.id(), this));
    }
    for(auto& comp : source.computations)
    {
        this->computations.add(comp.clone(m_context, comp.id(), this));
    }

    connect(m_space, &SpaceModel::spaceChanged,
            this, [=] () {
        for(auto& area : areas)
            area.areaChanged(area.currentMapping());
    });

    setDuration(source.duration());
}


ProcessModel* ProcessModel::clone(
        const Id<Process::ProcessModel> &newId,
        QObject *newParent) const
{
    auto& doc = iscore::IDocument::documentContext(*newParent);
    return new ProcessModel{*this,  doc, newId, newParent};
}

UuidKey<Process::ProcessFactory>ProcessModel::concreteFactoryKey() const
{
    return ProcessMetadata::concreteFactoryKey();
}

QString ProcessModel::prettyName() const
{
    return tr("Space process");
}

void ProcessModel::setDurationAndScale(const TimeValue &newDuration)
{
    setDuration(newDuration);
    ISCORE_TODO;
}

void ProcessModel::setDurationAndGrow(const TimeValue &newDuration)
{
    setDuration(newDuration);
    ISCORE_TODO;
}

void ProcessModel::setDurationAndShrink(const TimeValue &newDuration)
{
    setDuration(newDuration);
    ISCORE_TODO;
}

void ProcessModel::reset()
{
    ISCORE_TODO;
}

ProcessStateDataInterface* ProcessModel::startStateData() const
{
    ISCORE_TODO;
    return nullptr;
}

ProcessStateDataInterface *ProcessModel::endStateData() const
{
    ISCORE_TODO;
    return nullptr;
}

Selection ProcessModel::selectableChildren() const
{
    ISCORE_TODO;
    return {};
}

Selection ProcessModel::selectedChildren() const
{
    ISCORE_TODO;
    return {};
}

void ProcessModel::setSelection(const Selection &s) const
{
    ISCORE_TODO;
}

Process::LayerModel *ProcessModel::makeLayer_impl(
        const Id<Process::LayerModel> &viewModelId,
        const QByteArray &constructionData,
        QObject *parent)
{
    return new LayerModel{viewModelId, *this, parent};
}

Process::LayerModel *ProcessModel::cloneLayer_impl(
        const Id<Process::LayerModel> &newId,
        const Process::LayerModel &source,
        QObject *parent)
{
    return new LayerModel{newId, *this, parent};
}


void ProcessModel::startExecution()
{
}

void ProcessModel::stopExecution()
{
    // Reset everything to the default values.
}
}
