#pragma once
#include <QString>
#include <boost/optional.hpp>
#include <State/Value.hpp>
#include <iscore/tools/IdentifiedObject.hpp>

namespace Space
{
// For now a simple min-max.
class DimensionModel : public IdentifiedObject<DimensionModel>
{
        Q_OBJECT
        Q_PROPERTY(double min READ min WRITE setMin NOTIFY minChanged)
        Q_PROPERTY(double max READ max WRITE setMax NOTIFY maxChanged)

    public:
        DimensionModel(const QString& name, const Id<DimensionModel>& id, QObject* parent):
            IdentifiedObject{id, staticMetaObject.className(), parent},
            m_name{name}
        {
        }

        template<typename DeserializerVisitor,
                 enable_if_deserializer<DeserializerVisitor>* = nullptr>
        DimensionModel(DeserializerVisitor&& vis,
                   QObject* parent) :
            IdentifiedObject{vis, parent}
        {
            vis.writeTo(*this);
        }

        ~DimensionModel();

        const QString& name() const
        {
            return m_name;
        }

        const auto& value() const { return m_val; }
        void setValue(const boost::optional<double>& val) { m_val = val; }

        double min() const
        {
            return m_min;
        }

        double max() const
        {
            return m_max;
        }

        void setMin(double min)
        {
            if (m_min == min)
                return;

            m_min = min;
            emit minChanged(min);
        }

        void setMax(double max)
        {
            if (m_max == max)
                return;

            m_max = max;
            emit maxChanged(max);
        }

    signals:
        void minChanged(double min);
        void maxChanged(double max);

    private:
        QString m_name;

        // The value is used if valid.
        boost::optional<double> m_val;
        double m_min;
        double m_max;
};
}
