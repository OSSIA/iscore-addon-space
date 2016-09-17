#pragma once
#include <Process/GenericProcessFactory.hpp>

#include <src/SpaceProcess.hpp>
#include <src/SpaceLayerModel.hpp>
#include <src/SpaceLayerPresenter.hpp>
#include <src/SpaceLayerView.hpp>
#include <iscore/document/DocumentInterface.hpp>
#include <Process/LayerModelPanelProxy.hpp>

namespace Process
{
template<>
Space::ProcessModel* Process::GenericProcessModelFactory<
    Space::ProcessModel>::make(
        const TimeValue& duration,
        const Id<Process::ProcessModel>& id,
        QObject* parent)
{
    auto& doc = iscore::IDocument::documentContext(*parent);
    return new Space::ProcessModel{doc, duration, id, parent};
}

template<>
Space::ProcessModel* Process::GenericProcessModelFactory<
    Space::ProcessModel>::load(
        const VisitorVariant& vis,
        QObject* parent)
{
    auto& doc = iscore::IDocument::documentContext(*parent);
    return deserialize_dyn(vis, [&] (auto&& deserializer)
    { return new Space::ProcessModel{doc, deserializer, parent}; });
}



}
namespace Space
{
using ProcessFactory = Process::GenericProcessModelFactory<
    Space::ProcessModel>;
using LayerFactory = Process::GenericLayerFactory<
Space::ProcessModel,
Space::Layer,
Space::LayerPresenter,
Space::LayerView,
Process::GraphicsViewLayerModelPanelProxy>;
}
