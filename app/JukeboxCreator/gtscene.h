#ifndef GTSCENE_H
#define GTSCENE_H

#include <vector>
#include "gtentityslot.h"
#include "jsonbase.h"
#include "gtentity.h"

class GTScene : public JSONBase
{
public:
    GTScene();

    virtual bool Deserialize(const rapidjson::Value& obj);
    virtual bool Serialize(rapidjson::Writer<rapidjson::StringBuffer>* writer) const;

    std::vector<GTEntitySlot> entitySlots;
    std::vector<GTEntity> entities;
};

#endif // GTSCENE_H
