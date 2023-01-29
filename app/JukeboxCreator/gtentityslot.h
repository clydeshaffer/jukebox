#ifndef GTENTITYSLOT_H
#define GTENTITYSLOT_H

#include "jsonbase.h"
#include "gtbehavior.h"

class GTEntitySlot : JSONBase
{
public:
    GTEntitySlot();

    virtual bool Deserialize(const rapidjson::Value& obj);
    virtual bool Serialize(rapidjson::Writer<rapidjson::StringBuffer>* writer) const;

    unsigned char sprite_id;
    std::string sprite_name;

    int behavior_id = 0;
    GTBehavior *behavior;
};

#endif // GTENTITYSLOT_H
