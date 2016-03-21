#pragma once
#include <iscore/tools/IdentifiedObject.hpp>
#include "DimensionModel.hpp"
#include <QMap>
#include <QString>
#include <QPointF>

#include <QGraphicsView>

namespace Space
{
class ViewportModel : public IdentifiedObject<ViewportModel>
{
        Q_OBJECT
        ISCORE_SERIALIZE_FRIENDS(Space::ViewportModel, DataStream)
        ISCORE_SERIALIZE_FRIENDS(Space::ViewportModel, JSONObject)
        Q_PROPERTY(QTransform transform READ transform WRITE setTransform NOTIFY transformChanged)
        Q_PROPERTY(int renderPrecision READ renderPrecision WRITE setRenderPrecision NOTIFY renderPrecisionChanged)

    public:
        ViewportModel(const Id<ViewportModel>& id, QObject* parent):
            IdentifiedObject{id, staticMetaObject.className(), parent}
        {

        }

        template<typename DeserializerVisitor,
                 enable_if_deserializer<DeserializerVisitor>* = nullptr>
        ViewportModel(DeserializerVisitor&& vis,
                   QObject* parent) :
            IdentifiedObject{vis, parent}
        {
            vis.writeTo(*this);
        }

        const Id<DimensionModel>& xDim() const;
        void setXDim(const Id<DimensionModel>& xDim);

        const Id<DimensionModel>& yDim() const;
        void setYDim(const Id<DimensionModel>& yDim);

//        const QMap<Id<DimensionModel>, double>& defaultValuesMap() const;
//        void setDefaultValuesMap(const QMap<Id<DimensionModel>, double>& defaultValuesMap);

        const QString& name() const;
        void setName(const QString& name);

        const QTransform& transform() const
        {
            return m_transform;
        }

        void setTransform(const QTransform& transform)
        {
            if (m_transform == transform)
                return;

            m_transform = transform;
            emit transformChanged(transform);
        }

        int renderPrecision() const
        {
            return m_renderPrecision;
        }

        void setRenderPrecision(int renderPrecision)
        {
            if (m_renderPrecision == renderPrecision)
                return;

            m_renderPrecision = renderPrecision;
            emit renderPrecisionChanged(renderPrecision);
        }

    signals:
        void transformChanged(const QTransform& transform);

        void renderPrecisionChanged(int renderPrecision);

    private:
        QString m_name;
        QTransform m_transform;

        // Map from a dimension in space to a dimension in the GUI
        Id<DimensionModel> m_xDim;
        Id<DimensionModel> m_yDim;

        // Map from a dimension in space to a default value.
        // e.g. : for (x, y, z), we set z = 0.
        QMap<Id<DimensionModel>, double> m_defaultValuesMap;
        int m_renderPrecision;
};
}
