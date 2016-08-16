#pragma once
#include <Process/LayerModelPanelProxy.hpp>

namespace Process { class LayerModel; }
namespace Space
{
class ProcessProxyLayer;
class ProcessPanelProxy : public Process::LayerModelPanelProxy
{
    public:
        ProcessPanelProxy(
                ProcessProxyLayer* vm,
                QObject* parent);

        // Can return the same view model, or a new one.
        const Process::LayerModel& layer() final override;
        QWidget* widget() const final override;

    private:
        ProcessProxyLayer* m_layerImpl{};
        QWidget* m_widg{};

};
}
