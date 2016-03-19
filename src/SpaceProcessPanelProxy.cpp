#include "SpaceProcessPanelProxy.hpp"

#include <src/SpaceProcessProxyLayerModel.hpp>

namespace Space
{
ProcessPanelProxy::ProcessPanelProxy(
        ProcessProxyLayerModel* vm,
        QObject *parent):
    LayerModelPanelProxy{parent},
    m_layerImpl{vm},
    m_widg{new QWidget}
{
    m_layerImpl->setParent(this);
}

const Process::LayerModel& ProcessPanelProxy::layer()
{
    return *m_layerImpl;
}

QWidget*ProcessPanelProxy::widget() const
{
    return m_widg;
}

}
