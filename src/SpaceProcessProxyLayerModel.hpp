#pragma once
#include <Process/LayerModel.hpp>
namespace Process { class LayerModel; }

namespace Space
{
class ProcessPanelProxy;
class ProcessProxyLayer : public Process::LayerModel
{
        Q_OBJECT
    public:
        ProcessProxyLayer(
                const Id<Process::LayerModel>&,
                const Process::LayerModel& model,
                QObject* parent);

        void serialize_impl(const VisitorVariant &) const override;
        Process::LayerModelPanelProxy* make_panelProxy(QObject *parent) const override;

    private:
        const Process::LayerModel& m_source;
};
}
