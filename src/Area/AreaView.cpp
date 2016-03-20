#include "AreaView.hpp"
#include <QGraphicsSceneMouseEvent>
#include <QPainterPathStroker>
namespace Space
{
AreaView::AreaView(QGraphicsItem *parent):
    QGraphicsObject{parent}
{

}

AreaView::~AreaView()
{

}

void AreaView::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    emit pressed(event->pos());
    event->accept();
}

void AreaView::mouseMoveEvent(QGraphicsSceneMouseEvent *event)
{
    emit moved(event->pos());
    event->accept();
}

void AreaView::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
    emit released(event->pos());
    event->accept();
}



//TODO use me
static auto randomColor()
{
    return qRgb(
             std::abs((qrand() % 255)),
             std::abs((qrand() % 255)),
             std::abs((qrand() % 255)));
}

PathAreaView::PathAreaView(QGraphicsItem* parent):
    AreaView{parent},
    m_pen{randomColor()},
    m_brush{m_pen.color().lighter()},
    m_stroker{m_pen}
{
}

void PathAreaView::setPath(QPainterPath&& path)
{
    prepareGeometryChange();
    m_rect = m_stroker.createStroke(path).boundingRect();
    m_path = std::move(path);
    update();
}


void PathAreaView::paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget)
{
    painter->setPen(m_pen);
    painter->setBrush(m_brush);
    painter->drawPath(m_path);
}


QRectF PathAreaView::boundingRect() const
{ return m_path.boundingRect(); }

}
