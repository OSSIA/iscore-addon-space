#pragma once
#include <iscore/tools/IdentifiedObject.hpp>
#include <Space/area.hpp>
#include <Device/Address/AddressSettings.hpp>
#include <src/Area/AreaFactory.hpp>
#include <src/SpaceContext.hpp>
#include <Process/ModelMetadata.hpp>
#include <iscore/component/Component.hpp>
#include <src/Area/ValMap.hpp>
#include <src/Area/AreaMetadata.hpp>
#include <iscore_plugin_space_export.h>
class QGraphicsItem;


namespace Space
{
class SpaceModel;
// in the end, isn't an area the same thing as a domain???
// Maps addresses / values to the parameter of an area
class AreaPresenter;
class ISCORE_PLUGIN_SPACE_EXPORT AreaModel : public IdentifiedObject<AreaModel>
{
        Q_OBJECT
        ISCORE_SERIALIZE_FRIENDS(Space::AreaModel, DataStream)
        ISCORE_SERIALIZE_FRIENDS(Space::AreaModel, JSONObject)
    public:
        ModelMetadata metadata;
        iscore::Components components;

        // The value is used as default value if the address is invalid.
        virtual const UuidKey<AreaFactory>& concreteFactoryKey() const = 0;
        virtual QString prettyName() const = 0;
        virtual int type() const = 0;

        AreaModel(
                const QStringList& formula,
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
        void setTransform(const QTransform& t)
        {
            m_transform = t;
            m_inverted = t.inverted();
            emit transformChanged(m_transform);
        }

        const QStringList& formula() const
        { return m_formula; }


    signals:
        void currentSymbolChanged(QString, double);
        void areaChanged(ValMap);
        void transformChanged(const QTransform&);

    private:
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
Q_DECLARE_METATYPE(GiNaC::exmap)
Q_DECLARE_METATYPE(GiNaC::symbol)
Q_DECLARE_METATYPE(Id<Space::AreaModel>)
#define AREA_METADATA(Export, Model, Uuid, ObjectKey, PrettyName, AreaFormula, AreaSpaceMap, AreaParameterMap, IntType) \
    OBJECTKEY_METADATA(Export, Model, ObjectKey) \
    UUID_METADATA(Export, Process::ProcessFactory, Model, Uuid) \
    TR_TEXT_METADATA(Export, Model, PrettyName_k, PrettyName) \
    TYPED_METADATA(Export, Model, Formula_k, QStringList, AreaFormula) \
    TYPED_METADATA(Export, Model, Formula_k, Space::SpaceMap, AreaSpaceMap) \
    TYPED_METADATA(Export, Model, Formula_k, Space::ParameterMap, AreaParameterMap) \
    TYPED_METADATA(Export, Model, Formula_k, int, IntType)


