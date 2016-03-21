#pragma once
#include <iscore/tools/SettableIdentifier.hpp>
#include <iscore/plugins/customfactory/FactoryInterface.hpp>
#include <src/SpaceContext.hpp>
#include <QObject>

namespace Space
{
class AreaModel;
class ComputationModel;
class ComputationFactory : public iscore::AbstractFactory<ComputationFactory>
{
        ISCORE_ABSTRACT_FACTORY_DECL(
                ComputationFactory,
                "60563b69-d8a2-40d4-93b8-284b142fbf4b")

        public:
            virtual ComputationModel* makeModel(
                        const AreaModel& a1,
                        const AreaModel& a2,
                        const Space::Context& ctx,
                        const Id<ComputationModel>& comp,
                        QObject* parent);

        virtual AreaModel* load(
                const VisitorVariant& data,
                const Space::Context& space,
                QObject* parent) = 0;

};
}
