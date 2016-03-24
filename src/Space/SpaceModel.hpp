#pragma once

#include <iscore/tools/IdentifiedObject.hpp>
#include <iscore/tools/IdentifiedObjectMap.hpp>
#include <Space/space.hpp>
#include <Space/bounded_symbol.hpp>
#include "DimensionModel.hpp"
#include "ViewportModel.hpp"
#include <src/SpaceContext.hpp>
#include <src/Bounds.hpp>

namespace Space
{
// A space has a number of dimensions
class SpaceModel : public IdentifiedObject<SpaceModel>
{
        Q_OBJECT
        ISCORE_SERIALIZE_FRIENDS(Space::SpaceModel, DataStream)
        ISCORE_SERIALIZE_FRIENDS(Space::SpaceModel, JSONObject)

        // Precision used for distance, etc... computations
        Q_PROPERTY(int precision READ precision WRITE setPrecision NOTIFY precisionChanged)
    public:
        SpaceModel(
                const Id<SpaceModel>& id,
                QObject* parent);

        template<typename Impl>
        SpaceModel(
                Deserializer<Impl>& vis,
                QObject* parent) :
            IdentifiedObject<SpaceModel>{vis, parent}
        {
            vis.writeTo(*this);
        }

        Bounds bounds() const;
        void addDimension(DimensionModel* dim);
        void removeDimension(const Id<DimensionModel> &id);
        const DimensionModel& dimension(const Id<DimensionModel>& id) const;
        const DimensionModel& dimension(const QString& name) const;

        const auto& dimensions() const
        { return m_dimensions; }

        void addViewport(ViewportModel*);
        void removeViewport(const Id<ViewportModel>&);
        const auto& viewports() const
        { return m_viewports; }

        // Might be false if there is no viewport.
        const Id<ViewportModel>& defaultViewport() const
        { return m_defaultViewport; }

        int precision() const
        {
            return m_precision;
        }

        void setPrecision(int precision)
        {
            if (m_precision == precision)
                return;

            m_precision = precision;
            emit precisionChanged(precision);
        }

    signals:
        void dimensionAdded(const DimensionModel&);
        void viewportAdded(const ViewportModel&);
        void spaceChanged();

        void precisionChanged(int precision);

    private:
        IdContainer<DimensionModel> m_dimensions;
        IdContainer<ViewportModel> m_viewports;

        Id<ViewportModel> m_defaultViewport;
        int32_t m_precision = 5;
};
}
