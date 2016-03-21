#pragma once

#include <Process/ModelMetadata.hpp>
#include <iscore/tools/IdentifiedObject.hpp>
#include <iscore/component/Component.hpp>
#include <State/Address.hpp>
namespace Space
{
using Computation = std::function<double()>;
class AreaModel;
class SpaceModel;
// Maps addresses / values to the parameter of an Computation
class ComputationModel :
        public IdentifiedObject<ComputationModel>
{
        Q_OBJECT
        ISCORE_SERIALIZE_FRIENDS(Space::ComputationModel, DataStream)
        ISCORE_SERIALIZE_FRIENDS(Space::ComputationModel, JSONObject)
    public:
        ModelMetadata metadata;
        iscore::Components components;
        ComputationModel(
                const Id<AreaModel>& a1,
                const Id<AreaModel>& a2,
                const SpaceModel& space,
                const Id<ComputationModel>&,
                QObject* parent);

        State::Address address() const
        { return m_addr; }

        const auto& space() const
        { return m_space; }

        virtual double result() const = 0;

    private:
        const SpaceModel& m_space;
        State::Address m_addr;
        Id<AreaModel> m_a1, m_a2;
};
}

Q_DECLARE_METATYPE(Id<Space::ComputationModel>)
