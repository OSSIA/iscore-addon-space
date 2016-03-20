#pragma once
#include <memory>
#include <Space/computation.hpp>
#include <Process/ModelMetadata.hpp>
#include <iscore/tools/IdentifiedObject.hpp>
#include <iscore/component/Component.hpp>
namespace Space
{
class SpaceModel;
// Maps addresses / values to the parameter of an Computation
class ComputationModel : public IdentifiedObject<ComputationModel>
{
        Q_OBJECT
    public:
        ModelMetadata metadata;
        iscore::Components components;
        using Computation = std::function<double()>;
        ComputationModel(
                QObject* computationContext,
                const Computation& comp,
                const SpaceModel& space,
                const Id<ComputationModel>&,
                QObject* parent);

        const auto& computation() const
        { return m_fun; }


        const auto& space() const
        { return m_space; }

    private:
        QObject* m_context{};
        Computation m_fun;
        const SpaceModel& m_space;
};
}

Q_DECLARE_METATYPE(Id<Space::ComputationModel>)
