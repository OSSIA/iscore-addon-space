#include "AreaView.hpp"
#include <QGraphicsSceneMouseEvent>
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
}
