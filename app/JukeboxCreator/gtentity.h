#ifndef GTENTITY_H
#define GTENTITY_H

#include "jsonbase.h"
#include <QGraphicsPixmapItem>

;
#pragma pack(push, r1, 1)
struct GTEntity_Struct {
    unsigned char vx, vy, frame, slot, hp, state;
};

#pragma pack(pop, r1)

class GTEntity : public JSONBase
{
public:
    GTEntity();
    virtual bool Deserialize(const rapidjson::Value& obj);
    virtual bool Serialize(rapidjson::Writer<rapidjson::StringBuffer>* writer) const;

    GTEntity_Struct ToEngineStruct();

    int vx, vy, frame, slot, hp, state;

    QGraphicsPixmapItem *pixItem;
};

#endif // GTENTITY_H
