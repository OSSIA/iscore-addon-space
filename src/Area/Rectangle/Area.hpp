#pragma once
#include "src/Area/AreaModel.hpp"

namespace Space
{
struct RectangleArea
{
        struct values {
                QRectF rect;
        };

        static const UuidKey<AreaFactory>& uuid() {
            static const UuidKey<AreaFactory> k{"7ba287e8-65e8-42b9-bce0-2f9ab52bef42"};
            return k;
        }
        static QString objectKey() { return "RectangleAreaModel"; }
        static QString prettyName(){ return QObject::tr("Rectangle"); }
        static QStringList formula() { return {" xv >= x0; xv <= x0 + w; yv >= y0; yv <= y0 + h"}; }

        static SpaceMap spaceMap() { return {{Id<DimensionModel>(0), "xv"},{Id<DimensionModel>(1), "yv"}} ;}
        static ParameterMap parameterMap() { return {{"x0", {}},{"y0", {}}, {"w", {}}, {"h", {}}} ;}
        static int type() { return 3;}

        static values mapToData(const ValMap& map)
        {
            return values{QRectF{QPointF{map.at("x0"), map.at("y0")}, QSizeF{map.at("w"), map.at("h")}}};
        }

        static QPainterPath makePath(const ValMap& map)
        {
            QPainterPath p;
            p.addRect(mapToData(map).rect);
            return p;
        }
};

class RectangleAreaModel : public AreaModel_T<RectangleArea>
{
        using AreaModel_T::AreaModel_T;

        AreaModel* clone(
                const Space::Context& space,
                const Id<AreaModel>& newId,
                QObject* parent) const override
        {
            return new RectangleAreaModel{*this, space, newId, parent};
        }
};
}
