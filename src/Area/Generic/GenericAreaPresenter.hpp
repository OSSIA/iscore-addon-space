#pragma once
#include "src/Area/AreaPresenter.hpp"
#include "GenericAreaView.hpp"
#include <src/Area/Generic/AreaComputer.hpp>
#include <src/Bounds.hpp>
namespace Space
{
class GenericAreaView;
class GenericAreaModel;

class GenericAreaPresenter : public AreaPresenter
{
        Q_OBJECT
    public:
        using model_type = GenericAreaModel;
        using view_type = GenericAreaView;

        GenericAreaPresenter(
                GenericAreaView *view,
                const GenericAreaModel &model,
                QObject* parent);

        void update() override;
        void on_areaChanged(ValMap) override;

        ~GenericAreaPresenter();
    signals:
        void startCompute(Space::Bounds b, SpaceMap sm, ValMap vals);

    private:
        DrawAreaComputer* m_cp{};

};
}
