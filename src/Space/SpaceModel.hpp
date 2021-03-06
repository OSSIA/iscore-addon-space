#pragma once

#include <iscore/model/IdentifiedObject.hpp>
#include <iscore/model/IdentifiedObjectMap.hpp>
#include <Space/bounded_symbol.hpp>

#include <src/Space/Dimension/DimensionModel.hpp>
#include <src/Space/Viewport/ViewportModel.hpp>
#include <src/SpaceContext.hpp>
#include <src/Bounds.hpp>

namespace Space
{
// A space has a number of dimensions
class SpaceModel : public IdentifiedObject<SpaceModel>
{
        Q_OBJECT
        ISCORE_SERIALIZE_FRIENDS

        // Precision used for distance, etc... computations
        Q_PROPERTY(int precision READ precision WRITE setPrecision NOTIFY precisionChanged)
    public:
        SpaceModel(
                const Id<SpaceModel>& id,
                QObject* parent);
        SpaceModel(
                const SpaceModel& other,
                QObject* parent);

        template<typename Impl>
        SpaceModel(
                Impl& vis,
                QObject* parent) :
            IdentifiedObject<SpaceModel>{vis, parent}
        {
            vis.writeTo(*this);
        }

        ~SpaceModel();
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
        const OptionalId<ViewportModel>& defaultViewport() const
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

        OptionalId<ViewportModel> m_defaultViewport;
        int32_t m_precision = 5;
};

void clear(Space::SpaceModel& sp);
}
