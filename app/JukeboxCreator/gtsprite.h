#ifndef GTSPRITE_H
#define GTSPRITE_H

#include <vector>
#include <string>
#include <QPixmap>
#include <filesystem>
#include "jsonbase.h"

using namespace std;
using namespace filesystem;

;
#pragma pack(push, r1, 1)
struct GTFrame {
    signed char x, y;
    unsigned char width, height, gx, gy, color, bank;
};
#pragma pack(pop, r1)

class GTSprite : JSONBase
{
public:
    GTSprite ();
    GTSprite (string name, path imgFile, path framesFile);
    GTSprite (string name, path imgFile, unsigned char tileSize);

    void InitImageData(path root);

    virtual bool Deserialize(const rapidjson::Value& obj);
    virtual bool Serialize(rapidjson::Writer<rapidjson::StringBuffer>* writer) const;

    QPixmap& getFrame(int frameNum);
    const GTFrame& getFrameInfo(int frameNum);
    QByteArray getCompressedSheet();
    QByteArray getRomFrames();

    int frameCount();
    string name;
private:
    bool hasFrames;
    path imgFile, framesFile;
    unsigned char tileSize;

    vector<GTFrame> frames;
    vector<QPixmap> framePixmaps;
    QPixmap *sheetPixmap;
    QImage *sheetImage_rgb;
    QImage *sheetImage_alpha;
    QPixmap *sheetPixmap_rgb;
    void loadFramesFromGSI(path framesFile);
    void loadFramesFromJSON(path framesFile);
};

#endif // GTSPRITE_H
