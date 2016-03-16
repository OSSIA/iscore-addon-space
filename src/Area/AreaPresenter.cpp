#include "AreaPresenter.hpp"
#include "AreaModel.hpp"
#include "AreaView.hpp"
#include "src/Space/SpaceModel.hpp"
#include <src/Commands/SpaceCommandFactory.hpp>
#include <iscore/document/DocumentContext.hpp>

#include <QGraphicsItem>
#include <iscore/widgets/GraphicsItem.hpp>

namespace Space
{

AreaPresenter::AreaPresenter(
        AreaView* view,
        const AreaModel& model,
        QObject *parent):
    NamedObject{"AreaPresenter", parent},
    m_model{model},
    m_view{view},
    m_dispatcher{model.context().doc.commandStack}
{
    connect(&model, &AreaModel::transformChanged,
            this, [=] (const QTransform& t) {
        m_view->setTransform(t);
    });
    view->setTransform(model.transform());

    connect(m_view, &AreaView::pressed,
            this, &AreaPresenter::on_areaPressed);
    connect(m_view, &AreaView::moved,
            this, &AreaPresenter::on_areaMoved);
    connect(m_view, &AreaView::released,
            this, &AreaPresenter::on_areaReleased);
}

AreaPresenter::~AreaPresenter()
{
    deleteGraphicsItem(m_view);
}

const Id<AreaModel>& AreaPresenter::id() const
{
    return m_model.id();
}

void AreaPresenter::on_areaPressed(QPointF pt)
{
    qDebug() << "click" << pt;
    m_originalTransform = m_model.transform();
    m_clickedPoint = pt;
}

void AreaPresenter::on_areaMoved(QPointF pt)
{
    qDebug() << "move" << pt;
    auto diff = pt - m_clickedPoint;
    auto t = m_model.transform();
    t.translate(diff.x(), diff.y());

    m_dispatcher.submitCommand(model(this), t);
}

void AreaPresenter::on_areaReleased(QPointF pt)
{
    qDebug() << "rel" << pt;
    auto diff = pt - m_clickedPoint;
    auto t = m_model.transform();
    t.translate(diff.x(), diff.y());

    m_dispatcher.submitCommand(model(this), t);
    m_dispatcher.commit();
}
}
