#include "GenericAreaPresenter.hpp"
#include "GenericAreaView.hpp"
#include "GenericAreaModel.hpp"

#include "src/Space/SpaceModel.hpp"

namespace Space
{
GenericAreaPresenter::GenericAreaPresenter(
        GenericAreaView* v,
        const GenericAreaModel& model,
        QObject* parent):
    AreaPresenter{v, model, parent}
{
    this->view(this).setPos(0, 0);

    m_cp = new DrawAreaComputer{model.formula()};
    connect(m_cp, &DrawAreaComputer::ready,
            this, [&] (auto vec) {
        this->view(this).setRects(std::move(vec));
        this->view(this).update();
    }, Qt::QueuedConnection);

    connect(this, &GenericAreaPresenter::startCompute,
            this, [=] (auto&&... args) { m_cp->computeArea(std::forward<decltype(args)>(args)...); },
            Qt::QueuedConnection);
}

GenericAreaPresenter::~GenericAreaPresenter()
{
    if(m_cp->thread()->isRunning())
        m_cp->thread()->quit();
}

void GenericAreaPresenter::update()
{
    view(this).updateRect(view(this).parentItem()->boundingRect());
}

// Il vaut mieux faire comme dans les courbes ou le curvepresenter s'occupe des segments....
void GenericAreaPresenter::on_areaChanged(ValMap map)
{;
    emit startCompute(
                model(this).space().bounds(),
                model(this).spaceMapping(),
                std::move(map));

}
}
