#ifndef GTENTITY_H
#define GTENTITY_H

#include "jsonbase.h"
#include <QGraphicsPixmapItem>

class GTEntity : public JSONBase
{
public:
    GTEntity();
    virtual bool Deserialize(const rapidjson::Value& obj);
    virtual bool Serialize(rapidjson::Writer<rapidjson::StringBuffer>* writer) const;

    int vx, vy, frame, slot, hp, state;

    QGraphicsPixmapItem *pixItem;
};

#endif // GTENTITY_H
