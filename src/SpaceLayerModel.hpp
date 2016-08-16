#pragma once
#include <Process/LayerModel.hpp>

namespace Space
{
class ProcessModel;
class Layer : public Process::LayerModel
{
        Q_OBJECT
    public:
        using model_type = Space::ProcessModel;
        Layer(
                Space::ProcessModel&,
                const Id<Process::LayerModel>&,
                QObject* parent);

        Layer(
                const Space::Layer&,
                Space::ProcessModel&,
                const Id<Process::LayerModel>&,
                QObject* parent);

        template<typename Impl>
        explicit Layer(
                Deserializer<Impl>& vis,
                Process::ProcessModel& model,
                QObject* parent) :
            Process::LayerModel {vis, model, parent}
        {
            vis.writeTo(*this);
        }

        void serialize_impl(const VisitorVariant &) const override;
};
}
