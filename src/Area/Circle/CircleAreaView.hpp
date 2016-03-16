#pragma once
#include <QGraphicsItem>
#include <QPainterPath>
#include <src/Area/AreaView.hpp>

namespace Space
{
class CircleAreaView : public AreaView
{
    public:
        CircleAreaView(QGraphicsItem* parent);


        QRectF boundingRect() const override;

        void paint(QPainter* painter,
                   const QStyleOptionGraphicsItem* ,
                   QWidget*) override;

        void update(double x0, double y0, double r);

    private:
        QPainterPath m_path;
};
}
