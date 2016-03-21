#pragma once
#include "src/Area/AreaFactory.hpp"

#include <src/Area/Generic/GenericAreaModel.hpp>
namespace Space
{
class GenericAreaFactory : public AreaMetadata_T<GenericArea, AreaFactory>
{
    public:
        QStringList generic_formula() const override;

        AreaModel* makeModel(
                const QStringList& formula,
                const Space::Context& space,
                const Id<AreaModel>&,
                QObject* parent) const override;

        AreaModel* load(
                        const VisitorVariant& data,
                        const Space::Context& space,
                        QObject* parent) const override;

        AreaPresenter* makePresenter(
                QGraphicsItem* view,
                const AreaModel& model,
                QObject* parent) const override;

        QGraphicsItem* makeView(QGraphicsItem* parent) const override;
};
}
