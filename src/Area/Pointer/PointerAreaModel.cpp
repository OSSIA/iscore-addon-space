#include "PointerAreaModel.hpp"
#include "PointerAreaPresenter.hpp"
#include "PointerAreaView.hpp"
#include "src/Area/AreaParser.hpp"
#include "src/Space/SpaceModel.hpp"


namespace Space
{
const AreaFactoryKey& PointerAreaModel::static_concreteFactoryKey()
{
    static const AreaFactoryKey name{"d407b641-4d0a-4769-8eb6-59a59c9c6a66"};
    return name;
}

const AreaFactoryKey& PointerAreaModel::concreteFactoryKey() const
{
    return static_concreteFactoryKey();
}

QString PointerAreaModel::prettyName() const
{
    return tr("Pointer");
}

QStringList PointerAreaModel::formula()
{
    return QStringList{"xv == x0", "yv == y0"};
}

PointerAreaModel::PointerAreaModel(
        const Space::AreaContext &space,
        const Id<AreaModel> &id,
        QObject *parent):
    AreaModel{AreaParser{formula()}.result(), space, id, parent}
{

}

AreaPresenter *PointerAreaModel::makePresenter(QGraphicsItem * parentitem, QObject * obj) const
{
    auto pres = new PointerAreaPresenter{new PointerAreaView{parentitem}, *this, obj};
    return pres;
}
}
