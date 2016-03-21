#pragma once
#include "src/Area/AreaModel.hpp"


namespace Space
{

struct GenericArea
{

        static const UuidKey<AreaFactory>& uuid() {
            static const UuidKey<AreaFactory> k{"ac867755-9c5c-472b-848f-a9826b239648"};
            return k;
        }

        static QStringList formula() { return {}; }
        static QString objectKey() { return "GenericAreaModel"; }
        static QString prettyName(){ return QObject::tr("Generic"); }

        static int type() { return 0 ;}

};

class GenericAreaModel : public AreaMetadata_T<GenericArea, AreaModel>
{
    public:
        using metadata_type = GenericArea;

        GenericAreaModel(
                const QStringList& form,
                const Space::Context& space,
                const Id<AreaModel>& id,
                QObject* parent):
            AreaMetadata_T<GenericArea, AreaModel>{form, space, id, parent}
        {

        }

        template<typename Impl>
        GenericAreaModel(Deserializer<Impl>& vis,
                    const Space::Context& space,
                    QObject* parent) :
            AreaMetadata_T<GenericArea, AreaModel>{vis, space, parent}
        {
        }
};
}
