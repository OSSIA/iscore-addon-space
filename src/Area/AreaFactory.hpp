#pragma once
#include <iscore/tools/SettableIdentifier.hpp>
#include <iscore/plugins/customfactory/FactoryInterface.hpp>
#include <src/SpaceContext.hpp>
#include <src/Area/ValMap.hpp>
#include <src/Area/AreaMetadata.hpp>
#include <iscore/serialization/VisitorCommon.hpp>
#include <iscore/plugins/customfactory/ModelFactory.hpp>
#include <QObject>
class QGraphicsItem;


namespace Space
{
class SpaceModel;
class AreaModel;
class AreaPresenter;

class AreaFactory :
        public iscore::AbstractFactory<AreaFactory>,
        public iscore::GenericModelFactory<
             AreaModel,
             iscore::MakeArgs<const QStringList&, const Space::Context&, const Id<AreaModel>&, QObject*>,
             iscore::LoadArgs<const VisitorVariant&, const Space::Context&, QObject*>>
{
        ISCORE_ABSTRACT_FACTORY("4620006c-4562-42f4-a3eb-4cd525330d50")
    public:
        virtual ~AreaFactory();

        // Pretty name, id
        virtual int type() const = 0;

        // Presenter
        virtual AreaPresenter* makePresenter(
                QGraphicsItem* view,
                const AreaModel& model,
                QObject* parent) const = 0;

        // View
        virtual QGraphicsItem* makeView(QGraphicsItem* parent) const = 0;

        // Formula
        virtual QStringList formula() const {
            return {};
        }
        virtual SpaceMap defaultSpaceMap() const {
            return {};
        }
        virtual ParameterMap defaultParameterMap() const {
            return {};
        }

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

        QStringList formula() const override
        { return T::formula(); }
        SpaceMap defaultSpaceMap() const override
        { return T::spaceMap(); }
        ParameterMap defaultParameterMap() const override
        { return T::parameterMap(); }

        AreaModel* make(
                const QStringList& formula,
                const Space::Context& space,
                const Id<AreaModel>& id,
                QObject* parent) override
        {
            return new Model_T{space, id, parent};
        }

        AreaModel* load(
                        const VisitorVariant& data,
                        const Space::Context& space,
                        QObject* parent) override

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
