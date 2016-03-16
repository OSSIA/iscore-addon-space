#pragma once
#include <QMetaType>
namespace Space
{
struct Bounds
{
        double min_x;
        double max_x;
        double min_y;
        double max_y;
        double side;
};
}
Q_DECLARE_METATYPE(Space::Bounds)
