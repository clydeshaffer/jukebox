#ifndef GTENTITYSLOT_H
#define GTENTITYSLOT_H

#include "jsonbase.h"
#include "behaviorslistmodel.h"

class GTEntitySlot : JSONBase
{
public:
    GTEntitySlot();

    virtual bool Deserialize(const rapidjson::Value& obj);
    virtual bool Serialize(rapidjson::Writer<rapidjson::StringBuffer>* writer) const;

    unsigned char sprite_id;
    std::string sprite_name;

    BehaviorsListModel* behaviors;

    unsigned char type = 0;
};

#endif // GTENTITYSLOT_H
