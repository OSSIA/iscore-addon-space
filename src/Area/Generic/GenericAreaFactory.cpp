#include "GenericAreaFactory.hpp"
#include "GenericAreaModel.hpp"
#include "GenericAreaPresenter.hpp"
#include "GenericAreaView.hpp"
#include <iscore/serialization/VisitorCommon.hpp>


namespace Space
{
QStringList GenericAreaFactory::generic_formula() const
{
    return GenericArea::formula();
}

AreaModel*GenericAreaFactory::makeModel(
        const QStringList& formula,
        const Space::Context& space,
        const Id<AreaModel>& id,
        QObject* parent) const
{
    return new GenericAreaModel{formula, space, id, parent};
}

AreaModel*GenericAreaFactory::load(
        const VisitorVariant& data,
        const Context& space,
        QObject* parent) const

{
    return deserialize_dyn(data, [&] (auto&& deserializer)
    { return new GenericAreaModel{deserializer, space, parent}; });
}

AreaPresenter*GenericAreaFactory::makePresenter(
        QGraphicsItem* view,
        const AreaModel& model,
        QObject* parent) const
{
    return new GenericAreaPresenter{
        static_cast<GenericAreaPresenter::view_type*>(view),
                static_cast<const GenericAreaPresenter::model_type&>(model), parent};
}

QGraphicsItem* GenericAreaFactory::makeView(QGraphicsItem* parent) const
{
    return new GenericAreaView{parent};
}
}
