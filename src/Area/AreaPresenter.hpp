#pragma once
#include <iscore/tools/NamedObject.hpp>
#include <iscore/tools/SettableIdentifier.hpp>
#include <src/Area/ValMap.hpp>
#include <iscore/command/Dispatchers/SingleOngoingCommandDispatcher.hpp>
#include <src/Commands/UpdateTransform.hpp>
class QGraphicsItem;

namespace Space
{
class AreaModel;
class GenericAreaView;
class AreaView;

class AreaPresenter : public NamedObject
{
    public:
        using model_type = AreaModel;
        using view_type = GenericAreaView;
        AreaPresenter(
                AreaView* view,
                const AreaModel &model,
                QObject* parent);

        ~AreaPresenter();

        const Id<AreaModel>& id() const;

        virtual void update() = 0;
        virtual void on_areaChanged(ValMap) = 0;

        // Useful for subclasses
        template<typename T>
        const typename T::model_type& model(T*) const
        { return static_cast<const typename T::model_type&>(m_model); }
        template<typename T>
        typename T::view_type& view(T*) const
        { return static_cast<typename T::view_type&>(*m_view); }

    private:
        void on_areaPressed(QPointF);
        void on_areaMoved(QPointF);
        void on_areaReleased(QPointF);

        QTransform m_originalTransform;
        QPointF m_clickedPoint;

        const AreaModel& m_model;
        AreaView* m_view{};

        SingleOngoingCommandDispatcher<UpdateTransform> m_dispatcher;
};
}
