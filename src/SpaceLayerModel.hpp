#pragma once
#include <Process/LayerModel.hpp>

namespace Space
{
class ProcessModel;
class LayerModel : public Process::LayerModel
{
        Q_OBJECT
    public:
        using model_type = Space::ProcessModel;
        LayerModel(
                Space::ProcessModel&,
                const Id<Process::LayerModel>&,
                QObject* parent);

        LayerModel(
                const Space::LayerModel&,
                Space::ProcessModel&,
                const Id<Process::LayerModel>&,
                QObject* parent);

        template<typename Impl>
        explicit LayerModel(
                Deserializer<Impl>& vis,
                Process::ProcessModel& model,
                QObject* parent) :
            Process::LayerModel {vis, model, parent}
        {
            vis.writeTo(*this);
        }

        void serialize(const VisitorVariant &) const override;
        Process::LayerModelPanelProxy *make_panelProxy(QObject *parent) const override;
};
}
