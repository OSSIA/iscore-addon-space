#pragma once
#include "src/Area/AreaModel.hpp"


namespace Space
{
class GenericAreaModel : public AreaModel
{
        Q_OBJECT
    public:
        static constexpr int static_type() { return 0; }
        int type() const override { return static_type(); }

        static const AreaFactoryKey& static_factoryKey();
        const AreaFactoryKey& factoryKey() const override;

        QString prettyName() const override;
        QStringList formula() const;


        GenericAreaModel(
                const QStringList& formula,
                const Space::AreaContext& space,
                const Id<AreaModel>&,
                QObject* parent);

    private:
        QStringList m_formula;
};
}
