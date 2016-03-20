#pragma once
#include <QGraphicsItem>
#include <QPainterPath>
#include <QPainter>

namespace Space
{
class AreaView : public QGraphicsObject
{
        Q_OBJECT
    public:
        explicit AreaView(QGraphicsItem * parent);

        virtual ~AreaView();

    signals:
        void pressed(QPointF);
        void moved(QPointF);
        void released(QPointF);
    private:
        void mousePressEvent(QGraphicsSceneMouseEvent *event) override;
        void mouseMoveEvent(QGraphicsSceneMouseEvent *event) override;
        void mouseReleaseEvent(QGraphicsSceneMouseEvent *event) override;
};

class PathAreaView : public AreaView
{
    public:
        PathAreaView(QGraphicsItem * parent);

        void setPath(QPainterPath&& path);

        void paint(
                QPainter *painter,
                const QStyleOptionGraphicsItem *option,
                QWidget *widget) override;

        QRectF boundingRect() const override;

    private:
        QPainterPath m_path;
        QPen m_pen;
        QBrush m_brush;
        QRectF m_rect;
        QPainterPathStroker m_stroker;
};
}
