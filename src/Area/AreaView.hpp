#pragma once
#include <QGraphicsItem>
#include <QPainterPath>

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
}
