#include "CircleAreaModel.hpp"
#include "CircleAreaPresenter.hpp"
#include "CircleAreaView.hpp"
#include "src/Area/AreaParser.hpp"
#include "src/Space/SpaceModel.hpp"

namespace Space
{

const UuidKey<AreaFactory>&CircleAreaModel::static_concreteFactoryKey()
{
    static const UuidKey<AreaFactory> name{"f7235762-661b-4f0e-a5e6-b95afbd86458"};
    return name;
}

const UuidKey<AreaFactory>&CircleAreaModel::concreteFactoryKey() const
{
    return static_concreteFactoryKey();
}

QString CircleAreaModel::prettyName() const
{
    return tr("Circle");
}

QStringList CircleAreaModel::formula()
{
    return {"(xv-x0)^2 + (yv-y0)^2 <= r^2"};
}

CircleAreaModel::CircleAreaModel(
        const Space::AreaContext &space,
        const Id<AreaModel> &id,
        QObject *parent):
    AreaModel{formula(), space, id, parent}
{
}

AreaPresenter *CircleAreaModel::makePresenter(QGraphicsItem * parentitem, QObject * obj) const
{
    auto pres = new CircleAreaPresenter{new CircleAreaView{parentitem}, *this, obj};
    return pres;
}

}
