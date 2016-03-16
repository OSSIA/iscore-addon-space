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

static QPointF center(const Bounds& b)
{
    return QPointF{b.max_x - b.min_x, b.max_y - b.min_y};
}

static void transform(
        QTransform& t,
        Tool tool,
        QPointF ctr,
        QPointF orig,
        QPointF cur)
{
    switch(tool)
    {
        case Tool::Move:
        {
            auto diff = cur - orig;
            t.translate(diff.x(), diff.y());
            break;
        }
        case Tool::Rotate:
        {
            // 1. Compute angle between center - orig & center - cur
            auto angle = QLineF{ctr, orig}.angleTo(QLineF{ctr, cur});

            // 2. Transform
            t.rotate(angle);
            break;
        }
        case Tool::Scale:
        {
            t.scale((orig.x() - ctr.x()) / (cur.x() - ctr.x()),
                    (orig.y() - ctr.y()) / (cur.y() - ctr.y()));
            break;
        }
    }
}

void AreaPresenter::on_areaPressed(QPointF pt)
{
    m_originalTransform = m_model.transform();
    m_clickedPoint = pt;
    m_curBounds = m_model.context().space.bounds();
}

void AreaPresenter::on_areaMoved(QPointF pt)
{
    auto t = m_model.transform();
    transform(t, m_model.context().settings.tool(), center(m_curBounds), m_clickedPoint, pt);
    m_dispatcher.submitCommand(model(this), t);
}

void AreaPresenter::on_areaReleased(QPointF pt)
{
    auto t = m_model.transform();
    transform(t, m_model.context().settings.tool(), center(m_curBounds), m_clickedPoint, pt);

    m_dispatcher.submitCommand(model(this), t);
    m_dispatcher.commit();
}
}
