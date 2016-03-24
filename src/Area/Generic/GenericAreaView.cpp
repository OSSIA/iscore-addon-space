#include "GenericAreaView.hpp"
#include <QDebug>
#include <cmath>

namespace Space
{
GenericAreaView::GenericAreaView(QGraphicsItem *parent):
    AreaView{parent}
{
    m_col = static_cast<Qt::GlobalColor>(std::abs((double)(qrand() % 19)));
}

void GenericAreaView::setPath(QPainterPath path)
{
    prepareGeometryChange();

    m_path = std::move(path);
    m_rect = m_path.boundingRect();

    update();
}

QRectF GenericAreaView::boundingRect() const
{
    return m_rect;
}

void GenericAreaView::paint(QPainter *painter, const QStyleOptionGraphicsItem *, QWidget *)
{
    painter->setPen(m_col.darker());
    painter->setBrush(m_col);

    painter->drawPath(m_path);
}
}
