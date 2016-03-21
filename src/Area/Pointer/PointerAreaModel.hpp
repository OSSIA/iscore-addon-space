#pragma once
#include "src/Area/AreaModel.hpp"

namespace Space
{
struct PointerArea
{
        struct values {
                QPointF center;
        };

        static const UuidKey<AreaFactory>& uuid() {
            static const UuidKey<AreaFactory> k{"d407b641-4d0a-4769-8eb6-59a59c9c6a66"};
            return k;
        }
        static QString objectKey() { return "PointerAreaModel"; }
        static QString prettyName(){ return QObject::tr("Pointer"); }
        static QStringList formula() { return {"xv == x0", "yv == y0"}; }

        static SpaceMap spaceMap() { return {{Id<DimensionModel>(0), "xv"},{Id<DimensionModel>(1), "yv"}} ;}
        static ParameterMap parameterMap() { return {{"x0", {}},{"y0", {}}} ;}
        static int type() { return 2 ;}

        static values mapToData(const ValMap& map)
        {
            return values{QPointF{map.at("x0"), map.at("y0")}};
        }

        static QPainterPath makePath(const ValMap& map)
        {
            const auto x0 = map.at("x0");
            const auto y0 = map.at("y0");

            QPainterPath p;
            p.moveTo(x0 - 2, y0 - 2);
            p.addEllipse({x0, y0}, 4, 4);
            p.moveTo(x0, y0 - 2); // top
            p.lineTo(x0, y0 + 2); // to bottom
            p.moveTo(x0 - 2, y0); // left
            p.lineTo(x0 + 2, y0); // to right

            return p;
        }
};

class PointerAreaModel : public AreaModel_T<PointerArea>
{
        using AreaModel_T::AreaModel_T;
};
}
