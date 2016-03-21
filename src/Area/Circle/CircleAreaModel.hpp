#pragma once
#include "src/Area/AreaModel.hpp"

namespace Space
{
struct CircleArea
{
        struct values {
                QPointF center;
                double r;
        };

        static const UuidKey<AreaFactory>& uuid() {
            static const UuidKey<AreaFactory> k{"f7235762-661b-4f0e-a5e6-b95afbd86458"};
            return k;
        }
        static QString objectKey() { return "CircleAreaModel"; }
        static QString prettyName(){ return QObject::tr("Circle"); }
        static QStringList formula() { return {"(xv-x0)^2 + (yv-y0)^2 <= r^2"}; }

        static SpaceMap spaceMap() { return {{Id<DimensionModel>(0), "xv"},{Id<DimensionModel>(1), "yv"}} ;}
        static ParameterMap parameterMap() { return {{"x0", {}},{"y0", {}}, {"r", makeAddressFromValue(50)}} ;}
        static int type() { return 1 ;}

        static values mapToData(const ValMap& map)
        {
            return values{QPointF{map.at("x0"), map.at("y0")}, map.at("r")};
        }

        static QPainterPath makePath(const ValMap& map)
        {
            const auto x0 = map.at("x0");
            const auto y0 = map.at("y0");
            const auto r = map.at("r");

            QPainterPath p;
            p.moveTo(x0 - r, y0 - r);
            p.addEllipse({r, r}, r, r);
            return p;
        }
};

class CircleAreaModel : public AreaModel_T<CircleArea>
{
        using AreaModel_T::AreaModel_T;
};
}
