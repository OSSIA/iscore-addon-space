#pragma once
#include <iscore/tools/NamedObject.hpp>
#include <iscore/tools/SettableIdentifier.hpp>
#include <src/Area/ValMap.hpp>
#include <iscore/command/Dispatchers/SingleOngoingCommandDispatcher.hpp>
#include <src/Commands/UpdateTransform.hpp>
#include <src/EditionSettings.hpp>
#include <iscore/widgets/GraphicsItem.hpp>
#include <QGraphicsItem>
#include <src/Bounds.hpp>

namespace Space
{
class AreaModel;
class GenericAreaView;
class AreaView;
using TransformData = std::tuple<QPointF, QSizeF, qreal>;
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

        TransformData m_originalTransform;
        QPointF m_clickedPoint;
        Bounds m_curBounds;

        const AreaModel& m_model;
        graphics_item_ptr<AreaView> m_view;

        SingleOngoingCommandDispatcher<UpdateTransform> m_dispatcher;
};


template<typename T,
         typename Model_T,
         typename View_T>
class AreaPresenter_T : public AreaPresenter
{
    public:
        using model_type = Model_T;
        using view_type = View_T;
        template<typename... Args>
        AreaPresenter_T(Args&&... args):
            AreaPresenter{std::forward<Args>(args)...}
        {

        }

        void update() override
        {
            ((QGraphicsItem&)AreaPresenter::view(this)).update();
        }

        void on_areaChanged(ValMap v) override
        {
            view(this).setPath(T::makePath(v));
        }
};
}
