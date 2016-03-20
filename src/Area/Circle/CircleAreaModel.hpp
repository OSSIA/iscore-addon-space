#pragma once
#include "src/Area/AreaModel.hpp"

namespace Space
{
class CircleAreaModel : public SpecializedAreaModel
{
        Q_OBJECT
    public:
        static constexpr int static_type() { return 1; }
        int type() const override { return static_type(); }

        static const UuidKey<AreaFactory>& static_concreteFactoryKey();
        const UuidKey<AreaFactory>& concreteFactoryKey() const override;

        QString prettyName() const override;
        SpaceMap defaultSpaceMap() const override;
        ParameterMap defaultParameterMap() const override;

        static QStringList formula();

        struct values {
                QPointF center;
                double r;
        };

        static auto mapToData(
                const ValMap& map)
        {
            return values{QPointF{map.at("x0"), map.at("y0")}, map.at("r")};
        }

        CircleAreaModel(
                const Space::AreaContext& space,
                const Id<AreaModel>&,
                QObject* parent);

        AreaPresenter *makePresenter(QGraphicsItem *, QObject *) const;
};
}
