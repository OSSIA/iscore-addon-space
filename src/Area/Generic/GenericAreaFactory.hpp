#pragma once
#include "src/Area/AreaFactory.hpp"

#include <src/Area/Generic/GenericAreaModel.hpp>
namespace Space
{
class GenericAreaFactory : public AreaMetadata_T<GenericArea, AreaFactory>
{
    public:
        AreaModel* make(
                const QStringList& formula,
                const Space::Context& space,
                const Id<AreaModel>&,
                QObject* parent) override;

        AreaModel* load(
                        const VisitorVariant& data,
                        const Space::Context& space,
                        QObject* parent) override;

        AreaPresenter* makePresenter(
                QGraphicsItem* view,
                const AreaModel& model,
                QObject* parent) const override;

        QGraphicsItem* makeView(QGraphicsItem* parent) const override;
};
}
