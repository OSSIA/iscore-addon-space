#include "ComputationModel.hpp"

template<>
void Visitor<Reader<DataStream>>::readFrom(
        const Space::ComputationModel& area)
{
    // Save the parent class
    readFrom(static_cast<const IdentifiedObject<Space::ComputationModel>&>(area));
    ISCORE_TODO;
}

template<>
void Visitor<Writer<DataStream>>::writeTo(
        Space::ComputationModel& area)
{
    ISCORE_TODO;
}

template<>
void Visitor<Reader<JSONObject>>::readFrom(
        const Space::ComputationModel& area)
{
    // Save the parent class
    readFrom(static_cast<const IdentifiedObject<Space::ComputationModel>&>(area));
    ISCORE_TODO;

}

template<>
void Visitor<Writer<JSONObject>>::writeTo(
        Space::ComputationModel& area)
{
    ISCORE_TODO;
}

namespace Space
{
ComputationModel::ComputationModel(
        QObject* computationContext,
        const Computation& comp,
        const SpaceModel& space,
        const Id<ComputationModel>& id,
        QObject* parent):
    IdentifiedObject{id, staticMetaObject.className(), parent},
    m_context{computationContext},
    m_fun{comp},
    m_space{space}
{
    metadata.setName(QString("Computation.%1").arg(*this->id().val()));

    m_context->setParent(this);
}
}
