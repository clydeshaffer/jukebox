#include <fstream>
#include <sstream>
#include <rapidjson/rapidjson.h>
#include <rapidjson/document.h>
#include "gtsprite.h"
#include "jsonmacros.h"

void GTSprite::loadFramesFromGSI(path framesFile)
{
    std::ifstream framesFileStream(framesFile.string().c_str(), std::ios::binary);
    GTFrame tempFrame;
    while (framesFileStream.read(reinterpret_cast<char*>(&tempFrame), sizeof(GTFrame))) {
        frames.push_back(tempFrame);
        framePixmaps.push_back(sheetPixmap->copy(tempFrame.gx, tempFrame.gy, tempFrame.width, tempFrame.height));
    }
}

void GTSprite::loadFramesFromJSON(path framesFile)
{
    std::ifstream framesFileStream(framesFile.string().c_str());
    std::stringstream buffer;
    buffer << framesFileStream.rdbuf();
    framesFileStream.close();

    rapidjson::Document doc;
    doc.Parse(buffer.str().c_str());
    auto framesJSONArray = doc["frames"].GetArray();
    GTFrame tempFrame;
    for(rapidjson::Value::ConstValueIterator itr = framesJSONArray.Begin(); itr != framesJSONArray.end(); ++itr) {
        tempFrame.x = (*itr)["spriteSourceSize"].GetObject()["x"].GetInt() - ((*itr)["spriteSourceSize"].GetObject()["w"].GetInt() / 2),
        tempFrame.y = (*itr)["spriteSourceSize"].GetObject()["y"].GetInt() - ((*itr)["spriteSourceSize"].GetObject()["h"].GetInt() / 2),
        tempFrame.width = (*itr)["frame"].GetObject()["w"].GetInt(),
        tempFrame.height = (*itr)["frame"].GetObject()["h"].GetInt(),
        tempFrame.gx = (*itr)["frame"].GetObject()["x"].GetInt(),
        tempFrame.gy = (*itr)["frame"].GetObject()["y"].GetInt();
        frames.push_back(tempFrame);
        framePixmaps.push_back(sheetPixmap->copy(tempFrame.gx, tempFrame.gy, tempFrame.width, tempFrame.height));
    }
}

GTSprite::GTSprite()
{
    hasFrames = false;
}

GTSprite::GTSprite(string name, path imgFile, path framesFile): name(name), imgFile(imgFile), framesFile(framesFile)
{
    hasFrames = true;
    sheetPixmap = new QPixmap(imgFile.string().c_str());
}

GTSprite::GTSprite(string name, path imgFile, unsigned char tileSize): name(name), imgFile(imgFile), tileSize(tileSize)
{
    hasFrames = false;
    sheetPixmap = new QPixmap(imgFile.string().c_str());
}

void GTSprite::InitImageData(path root)
{
    sheetPixmap = new QPixmap((root / imgFile).string().c_str());

    if(hasFrames) {
        path fullFramesPath = root / framesFile;
        string ext = fullFramesPath.extension().string();
        if(ext == ".gsi") {
            loadFramesFromGSI(fullFramesPath);
        } else if(ext == ".json") {
            loadFramesFromJSON(fullFramesPath);
        }
    } else {
        for(unsigned char y = 0; y < sheetPixmap->height(); y += tileSize) {
            for(unsigned char x = 0; x < sheetPixmap->width(); x += tileSize) {
                framePixmaps.push_back(sheetPixmap->copy(x, y, tileSize, tileSize));
                frames.push_back((GTFrame){0, 0, tileSize, tileSize, x, y, 0, 0});
            }
        }
    }
}

bool GTSprite::Deserialize(const rapidjson::Value& obj)
{
    JSON_READ(Bool, hasFrames);
    JSON_READ(Int, tileSize);
    JSON_READ(String, name);
    JSON_READ_PATH(imgFile);
    if(hasFrames) {
        JSON_READ_PATH(framesFile);
    }
}

bool GTSprite::Serialize(rapidjson::Writer<rapidjson::StringBuffer>* writer) const
{
    writer->StartObject();
    JSON_EMIT(Bool, hasFrames);
    JSON_EMIT(Int, tileSize);
    JSON_EMIT_STRING(name);
    JSON_EMIT_PATH(imgFile);
    if(hasFrames) {
        JSON_EMIT_PATH(framesFile);
    }
    writer->EndObject();
    return true;
}

QPixmap& GTSprite::getFrame(int frameNum)
{
    if(frameNum < 0) frameNum = 0;
    if(frameNum >= framePixmaps.size()) frameNum = framePixmaps.size() - 1;
    return framePixmaps[frameNum];
}

const GTFrame& GTSprite::getFrameInfo(int frameNum)
{
    if(frameNum < 0) frameNum = 0;
    if(frameNum >= framePixmaps.size()) frameNum = framePixmaps.size() - 1;
    return frames[frameNum];
}

int GTSprite::frameCount()
{
    return framePixmaps.size();
}
