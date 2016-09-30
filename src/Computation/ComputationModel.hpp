#pragma once

#include <iscore/model/Entity.hpp>
#include <iscore/tools/IdentifiedObject.hpp>
#include <iscore/component/Component.hpp>
#include <State/Message.hpp>
#include <iscore/plugins/customfactory/SerializableInterface.hpp>
#include <src/SpaceContext.hpp>
namespace Space
{
using Computation = std::function<double()>;
class AreaModel;
class SpaceModel;
class ComputationFactory;
// Maps addresses / values to the parameter of an Computation
class ComputationModel :
        public iscore::Entity<ComputationModel>,
        public iscore::SerializableInterface<ComputationFactory>
{
        Q_OBJECT
        ISCORE_SERIALIZE_FRIENDS(Space::ComputationModel, DataStream)
        ISCORE_SERIALIZE_FRIENDS(Space::ComputationModel, JSONObject)
    public:
        ComputationModel(
                const Id<AreaModel>& a1,
                const Id<AreaModel>& a2,
                const SpaceModel& space,
                const Id<ComputationModel>&,
                QObject* parent);

        ComputationModel(
                const ComputationModel& source,
                const SpaceModel& space,
                const Id<ComputationModel>&,
                QObject* parent);

        template<typename Impl>
        ComputationModel(Deserializer<Impl>& vis,
                  const SpaceModel& space,
                  QObject* parent) :
            Entity{vis, parent},
            m_space{space}
        {
            vis.writeTo(*this);
        }

        virtual ComputationModel* clone(
                const Space::Context& space,
                const Id<ComputationModel>& newId,
                QObject* parent) const = 0;

        const State::Address& address() const
        { return m_addr; }
        void setAddress(const State::Address& addr)
        { m_addr = addr; }

        const auto& space() const
        { return m_space; }

        virtual State::Value result() const = 0;

        State::Message message() const
        { return State::Message{m_addr, result()}; }

        auto firstArea() const {
            return m_a1;
        }
        auto secondArea() const {
            return m_a2;
        }

    protected:
        const SpaceModel& m_space;
        State::Address m_addr;
        Id<AreaModel> m_a1, m_a2;
};
}

Q_DECLARE_METATYPE(Id<Space::ComputationModel>)

#define COMPUTATION_METADATA(Export, Model, Uuid, ObjectKey, PrettyName) \
    MODEL_METADATA(Export, Space::ComputationFactory, Model, Uuid, ObjectKey, PrettyName)

TR_TEXT_METADATA(, Space::ComputationModel, PrettyName_k, "Computation")
