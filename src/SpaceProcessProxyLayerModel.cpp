#include "SpaceProcessProxyLayerModel.hpp"
#include "SpaceLayerModel.hpp"
#include "SpaceProcessPanelProxy.hpp"

namespace Space
{

ProcessProxyLayer::ProcessProxyLayer(
        const Id<LayerModel>& id,
        const LayerModel &model,
        QObject *parent):
    LayerModel{id, staticMetaObject.className(), model.processModel(), parent},
    m_source{model}
{

}


void ProcessProxyLayer::serialize_impl(const VisitorVariant &) const
{
    ISCORE_TODO;
}

Process::LayerModelPanelProxy* ProcessProxyLayer::make_panelProxy(QObject *parent) const
{
    auto lm = new ProcessProxyLayer(Id<Process::LayerModel>(), *this, nullptr);
    return new ProcessPanelProxy{lm, parent};
}

}
