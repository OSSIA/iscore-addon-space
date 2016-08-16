#include "SpaceLayerModel.hpp"
#include "SpaceProcess.hpp"

#include "SpaceProcessPanelProxy.hpp"
template<>
void Visitor<Reader<DataStream>>::readFrom_impl(const Space::Layer& lm)
{
}

template<>
void Visitor<Writer<DataStream>>::writeTo(Space::Layer& lm)
{
}



template<>
void Visitor<Reader<JSONObject>>::readFrom_impl(const Space::Layer& lm)
{
}

template<>
void Visitor<Writer<JSONObject>>::writeTo(Space::Layer& lm)
{
}

namespace Space
{
Layer::Layer(
        Space::ProcessModel & proc,
        const Id<Process::LayerModel> & id,
        QObject *parent):
    Process::LayerModel{id, staticMetaObject.className(), proc, parent}
{

}

Layer::Layer(
        const Space::Layer&,
        Space::ProcessModel & proc,
        const Id<Process::LayerModel> & id,
        QObject *parent):
    Process::LayerModel{id, staticMetaObject.className(), proc, parent}
{

}

void Layer::serialize_impl(const VisitorVariant & v) const
{
    serialize_dyn(v, *this);
}

}
