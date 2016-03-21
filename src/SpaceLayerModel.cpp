#include "SpaceLayerModel.hpp"
#include "SpaceProcess.hpp"

template<>
void Visitor<Reader<DataStream>>::readFrom(const Space::LayerModel& lm)
{
}

template<>
void Visitor<Writer<DataStream>>::writeTo(Space::LayerModel& lm)
{
}



template<>
void Visitor<Reader<JSONObject>>::readFrom(const Space::LayerModel& lm)
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

void LayerModel::serialize(const VisitorVariant &) const
{
    ISCORE_TODO;
}

Process::LayerModelPanelProxy *LayerModel::make_panelProxy(QObject *parent) const
{
    ISCORE_TODO;
    return nullptr;
}

}
