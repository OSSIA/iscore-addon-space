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
}

const Id<AreaModel>& AreaPresenter::id() const
{
    return m_model.id();
}

static QPointF center(const Bounds& b)
{
    return QPointF{b.max_x - b.min_x, b.max_y - b.min_y};
}


static TransformData make_transform(const AreaModel& area)
{
    return std::make_tuple(area.translate(), area.scale(), area.rotate());
}

static TransformData transform(
        TransformData area,
        Tool tool,
        QPointF ctr,
        QPointF orig,
        QPointF cur)
{
    switch(tool)
    {
        case Tool::Move:
        {
            std::get<0>(area) += (cur - orig);
            break;
        }
        case Tool::Rotate:
        {
            // 1. Compute angle between center - orig & center - cur
            auto angle = QLineF{ctr, orig}.angleTo(QLineF{ctr, cur});

            // 2. Transform
            std::get<2>(area) += angle;
            break;
        }
        case Tool::Scale:
        {
            auto& s = std::get<1>(area);
            s.scale((orig.x() - ctr.x()) / (cur.x() - ctr.x()),
                    (orig.y() - ctr.y()) / (cur.y() - ctr.y()),
                    Qt::IgnoreAspectRatio);
            if(s.width() <= 0.1)
                s.setWidth(0.1);
            if(s.width() >= 2)
                s.setWidth(2);
            if(s.height() <= 0.1)
                s.setHeight(0.1);
            if(s.height() >= 2)
                s.setHeight(2);
            break;
        }
    }
    return area;
}

void AreaPresenter::on_areaPressed(QPointF pt)
{
    m_originalTransform = make_transform(m_model);
    m_clickedPoint = m_model.invertedTransform().map(pt);
    m_curBounds = m_model.context().space.bounds();
}

void AreaPresenter::on_areaMoved(QPointF pt)
{
    auto res = transform(
                m_originalTransform,
                m_model.context().settings.tool(),
                center(m_curBounds),
                m_clickedPoint,
                m_model.transform().map(pt));

    m_dispatcher.submitCommand(model(this), std::get<0>(res), std::get<1>(res), std::get<2>(res));
}

void AreaPresenter::on_areaReleased(QPointF pt)
{
    auto res = transform(
                m_originalTransform,
                m_model.context().settings.tool(),
                center(m_curBounds),
                m_clickedPoint,
                m_model.transform().map(pt));

    m_dispatcher.submitCommand(model(this), std::get<0>(res), std::get<1>(res), std::get<2>(res));
    m_dispatcher.commit();
}
}
