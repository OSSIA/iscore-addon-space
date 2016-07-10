#pragma once
#include <iscore/tools/IdentifiedObject.hpp>
#include <Device/Address/AddressSettings.hpp>
#include <src/Area/AreaFactory.hpp>
#include <src/SpaceContext.hpp>
#include <Process/ModelMetadata.hpp>
#include <iscore/component/Component.hpp>
#include <src/Area/ValMap.hpp>
#include <src/Area/AreaMetadata.hpp>
#include <iscore_plugin_space_export.h>
#include <iscore/plugins/customfactory/SerializableInterface.hpp>
class QGraphicsItem;


namespace Space
{
class SpaceModel;
// in the end, isn't an area the same thing as a domain???
// Maps addresses / values to the parameter of an area
class AreaPresenter;
class ISCORE_PLUGIN_SPACE_EXPORT AreaModel :
        public IdentifiedObject<AreaModel>,
        public iscore::SerializableInterface<AreaFactory>
{
        Q_OBJECT
        ISCORE_SERIALIZE_FRIENDS(Space::AreaModel, DataStream)
        ISCORE_SERIALIZE_FRIENDS(Space::AreaModel, JSONObject)
    public:
        ModelMetadata metadata;
        iscore::Components components;

        // The value is used as default value if the address is invalid.
        virtual QString prettyName() const = 0;
        virtual int type() const = 0;

        AreaModel(
                const QStringList& formula,
                const Space::Context& space,
                const Id<AreaModel>&,
                QObject* parent);

        AreaModel(
                const AreaModel& formula,
                const Space::Context& space,
                const Id<AreaModel>&,
                QObject* parent);

        template<typename Impl>
        AreaModel(Deserializer<Impl>& vis,
                  const Space::Context& space,
                  QObject* parent) :
            IdentifiedObject<AreaModel>{vis, parent},
            m_context{space}
        {
            vis.writeTo(*this);
        }

        virtual AreaModel* clone(
                const Space::Context& space,
                const Id<AreaModel>& newId,
                QObject* parent) const = 0;

        const SpaceModel& space() const
        { return m_context.space; }
        const Context& context() const
        { return m_context; }

        // Maps local variable to space dimension, e.g. xv -> x
        void setSpaceMapping(const SpaceMap& mapping);
        const SpaceMap& spaceMapping() const
        { return m_spaceMap; }

        // Maps local variable to address or value, e.g. :
        // x0 -> 2.45
        // y0 -> an:/address
        // r  -> parent:/t
        void setParameterMapping(const ParameterMap& mapping);
        const ParameterMap& parameterMapping() const
        { return m_parameterMap; }

        // Sets the actual current values for the parameters
        void setCurrentMapping(const ValMap& map);
        void updateCurrentMapping(QString sym, double value);
        const ValMap& currentMapping() const
        { return m_currentParameterMap;}

        QString toString() const;
        const QTransform& transform() const
        { return m_transform; }
        const QTransform& invertedTransform() const
        { return m_inverted; }

        QPointF translate() const
        { return m_translate; }
        QSizeF scale() const
        { return m_scale; }
        qreal rotate() const
        { return m_rotate; }

        void setTranslate(QPointF translate)
        {
            m_translate = translate;
            updateTransform();
        }
        void setScale(QSizeF scale)
        {
            m_scale = scale;
            updateTransform();
        }
        void setRotate(qreal rotate)
        {
            m_rotate = rotate;
            updateTransform();
        }


        const QStringList& formula() const
        { return m_formula; }


    signals:
        void currentSymbolChanged(QString, double);
        void areaChanged(ValMap);
        void transformChanged(const QTransform&);

    private:
        void updateTransform()
        {
            m_transform =
                    QTransform{}
                    .rotate(m_rotate)
                    .scale(m_scale.width(), m_scale.height())
                    .translate(m_translate.x(), m_translate.y());

            m_inverted = m_transform.inverted();
            emit transformChanged(m_transform);
        }
        QPointF m_translate{0, 0};
        QSizeF m_scale{1, 1};
        qreal m_rotate{0};
        QTransform m_transform, m_inverted;
        const Space::Context& m_context;
        QStringList m_formula;

        // Maps a variable from m_area to a variable from m_space.
        SpaceMap m_spaceMap;

        ParameterMap m_parameterMap; // General values of the model.
        ValMap m_currentParameterMap; // Current values used for display / execution.
};

template<typename T>
class AreaModel_T : public AreaMetadata_T<T, AreaModel>
{
    public:
        using metadata_type = T;

        AreaModel_T(
                const Space::Context& space,
                const Id<AreaModel>& id,
                QObject* parent):
            AreaMetadata_T<T, AreaModel>{T::formula(), space, id, parent}
        {

        }

        AreaModel_T(
                const AreaModel_T& other,
                const Space::Context& space,
                const Id<AreaModel>& id,
                QObject* parent):
            AreaMetadata_T<T, AreaModel>{other, space, id, parent}
        {

        }

        template<typename Impl>
        AreaModel_T(Deserializer<Impl>& vis,
                    const Space::Context& space,
                    QObject* parent) :
            AreaMetadata_T<T, AreaModel>{vis, space, parent}
        {
        }
};


inline auto makeAddressFromValue(double val)
{
    Device::FullAddressSettings addr;
    addr.value = State::Value::fromValue(val);
    return addr;
}

struct Formula_k;
struct SpaceMap_k;
struct ParameterMap_k;
struct IntType_k;
}

Q_DECLARE_METATYPE(std::string)
Q_DECLARE_METATYPE(Id<Space::AreaModel>)

// TODO use me
#define AREA_METADATA(Export, Model, Uuid, ObjectKey, PrettyName, AreaFormula, AreaSpaceMap, AreaParameterMap, IntType) \
    MODEL_METADATA(Export, Process::ProcessFactory, Model, Uuid, ObjectKey, PrettyName) \
    TYPED_METADATA(Export, Model, Formula_k, QStringList, AreaFormula) \
    TYPED_METADATA(Export, Model, Formula_k, Space::SpaceMap, AreaSpaceMap) \
    TYPED_METADATA(Export, Model, Formula_k, Space::ParameterMap, AreaParameterMap) \
    TYPED_METADATA(Export, Model, Formula_k, int, IntType)


