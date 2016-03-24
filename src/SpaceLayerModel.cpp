#include "SpaceLayerModel.hpp"
#include "SpaceProcess.hpp"

#include "SpaceProcessPanelProxy.hpp"
template<>
void Visitor<Reader<DataStream>>::readFrom_impl(const Space::LayerModel& lm)
{
}

template<>
void Visitor<Writer<DataStream>>::writeTo(Space::LayerModel& lm)
{
}



template<>
void Visitor<Reader<JSONObject>>::readFrom_impl(const Space::LayerModel& lm)
{
}

template<>
void Visitor<Writer<JSONObject>>::writeTo(Space::LayerModel& lm)
{
}

namespace Space
{
LayerModel::LayerModel(
        const Id<Process::LayerModel> & id,
        Space::ProcessModel & proc,
        QObject *parent):
    Process::LayerModel{id, staticMetaObject.className(), proc, parent}
{

}

void LayerModel::serialize(const VisitorVariant & v) const
{
    serialize_dyn(v, *this);
}

Process::LayerModelPanelProxy *LayerModel::make_panelProxy(QObject *parent) const
{
    ISCORE_TODO;
    return nullptr;
}

}
