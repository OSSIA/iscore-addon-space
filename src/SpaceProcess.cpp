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
#include <iscore/serialization/VisitorCommon.hpp>
#include <src/Space/SpaceModel.hpp>

namespace Space
{
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
    Process::ProcessModel{duration, id, Metadata<ObjectKey_k, ProcessModel>::get(), parent},
    m_space{new SpaceModel{
            Id<SpaceModel>(0),
            this}},
    m_context{makeContext(doc, *this)}
{
    metadata().setName(QString("Space.%1").arg(*this->id().val()));
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
        const Id<Process::ProcessModel> &id,
        QObject *parent):
    Process::ProcessModel{source.duration(), id, Metadata<ObjectKey_k, ProcessModel>::get(), parent},
    m_space{new SpaceModel{
            *source.m_space,
            this}},
    m_context{makeContext(source.context().doc, *this)} // TODO this will break if for some reason we clone in another document
{
    metadata().setName(QString("Space.%1").arg(*this->id().val()));

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
}

}
