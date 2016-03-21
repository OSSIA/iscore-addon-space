#pragma once
#include <iscore/tools/SettableIdentifier.hpp>
#include <iscore/plugins/customfactory/FactoryInterface.hpp>
#include <src/SpaceContext.hpp>
#include <src/Area/AreaMetadata.hpp>
#include <iscore/serialization/VisitorCommon.hpp>
#include <QObject>
class QGraphicsItem;


namespace Space
{
class SpaceModel;
class AreaModel;
class AreaPresenter;

class AreaFactory : public iscore::AbstractFactory<AreaFactory>
{
        ISCORE_ABSTRACT_FACTORY_DECL(
                AreaFactory,
                "4620006c-4562-42f4-a3eb-4cd525330d50")
    public:
        virtual ~AreaFactory();

        // Pretty name, id
        virtual int type() const = 0;

        virtual QString prettyName() const = 0;

        // Model
        virtual AreaModel* makeModel(
                const QStringList& generic_formula,
                const Space::Context& space,
                const Id<AreaModel>&,
                QObject* parent) const = 0;

        virtual AreaModel* load(
                const VisitorVariant& data,
                const Space::Context& space,
                QObject* parent) const = 0;

        // Presenter
        virtual AreaPresenter* makePresenter(
                QGraphicsItem* view,
                const AreaModel& model,
                QObject* parent) const = 0;

        // View
        virtual QGraphicsItem* makeView(QGraphicsItem* parent) const = 0;

        // Formula
        virtual QStringList generic_formula() const = 0;

        // Widget ?
};



template<typename T,
         typename Model_T,
         typename Presenter_T,
         typename View_T>
class AreaFactory_T : public AreaMetadata_T<T, AreaFactory>
{
    public:
        using metadata_type = T;

        QStringList generic_formula() const override
        { return T::formula(); }

        AreaModel* makeModel(
                const QStringList& formula,
                const Space::Context& space,
                const Id<AreaModel>& id,
                QObject* parent) const override
        {
            return new Model_T{space, id, parent};
        }

        AreaModel* load(
                        const VisitorVariant& data,
                        const Space::Context& space,
                        QObject* parent) const override

        {
            return deserialize_dyn(data, [&] (auto&& deserializer)
            { return new Model_T{deserializer, space, parent}; });
        }

        AreaPresenter* makePresenter(
                QGraphicsItem* view,
                const AreaModel& model,
                QObject* parent) const override
        {
            return new Presenter_T{
                static_cast<View_T*>(view),
                static_cast<const Model_T&>(model),
                        parent};
        }

        QGraphicsItem* makeView(
                QGraphicsItem* parent) const override
        {
            return new View_T{parent};
        }
};

}

Q_DECLARE_METATYPE(UuidKey<Space::AreaFactory>)
