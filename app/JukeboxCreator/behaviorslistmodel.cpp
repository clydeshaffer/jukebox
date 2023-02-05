#include "behaviorslistmodel.h"

#include "gtproject.h"
#include <QDebug>

BehaviorsListModel::BehaviorsListModel(QObject *parent)
    : QAbstractListModel(parent)
{
}

BehaviorsListModel::BehaviorsListModel()
    : QAbstractListModel(nullptr)
{
}

int BehaviorsListModel::rowCount(const QModelIndex &parent) const
{
    return behaviors.size();
}

QVariant BehaviorsListModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid())
        return QVariant();

    if((index.row() < 0) || (index.row() >= behaviors.size())) {
        return QVariant();
    }

    if(role == Qt::DisplayRole) {
        return QString::fromStdString(behaviors[index.row()].ptr->name);
    } else if(role == Qt::ToolTipRole) {
        return QString::fromStdString(behaviors[index.row()].ptr->source.string());
    }
    return QVariant();

}

bool BehaviorsListModel::insertRows(int row, int count, const QModelIndex &parent)
{
    beginInsertRows(parent, row, row+count);
    for(int i = 0; i < count; ++i) {
        behaviors.insert(behaviors.begin()+row, {"", nullptr});
    }
    endInsertRows();
    return true;
}

bool BehaviorsListModel::removeRows(int row, int count, const QModelIndex &parent)
{
    beginRemoveRows(parent, row, row+count);
    behaviors.erase(behaviors.begin()+row, behaviors.begin()+row+count);
    endRemoveRows();
    return true;
}

bool BehaviorsListModel::setData(const QModelIndex &index, const QVariant &value, int role)
{
    return true;
}

bool BehaviorsListModel::Deserialize(const rapidjson::Value& obj)
{
    behaviors.clear();
    auto behaviorsJSONArray = obj.GetArray();
    beginInsertRows(QModelIndex(), 0, behaviorsJSONArray.Size() - 1);
    for(rapidjson::Value::ConstValueIterator itr = behaviorsJSONArray.Begin(); itr != behaviorsJSONArray.end(); ++itr) {
        GTBehavior* referencedBehavior = &GTProject::loadedProject->behaviors[itr->GetInt()];
        behaviors.push_back({referencedBehavior->uid, referencedBehavior});
    }
    endInsertRows();

    emit dataChanged(createIndex(0, 0), createIndex(behaviors.size()-1, 0));
    return true;
}

bool BehaviorsListModel::Serialize(rapidjson::Writer<rapidjson::StringBuffer>* writer) const
{
    writer->StartArray();
    for(auto& b : behaviors) {
        writer->Int(b.ptr->cached_index);
    }
    writer->EndArray();
    return true;
}

std::string BehaviorsListModel::GenName()
{
    std::string name = "updater";
    for(auto& behavior : behaviors) {
        name += "_";
        name += std::to_string(behavior.ptr->cached_index);
    }
    return name;
}

std::string BehaviorsListModel::GetCode(std::filesystem::path root)
{
    std::string outCode = "";
    for(auto& behavior : behaviors) {
        outCode += ".import ";
        outCode += behavior.ptr->GenName();
        outCode += "\n";
    }
    outCode += ".export ";
    outCode += GenName();
    outCode += "\n";

    outCode += GenName();
    outCode += ":\n";

    for(auto& behavior : behaviors) {
        outCode += "\tJSR ";
        outCode += behavior.ptr->GenName();
        outCode += "\n";
    }
    outCode += "\tRTS\n";
    return outCode;
}

bool BehaviorsListModel::push_back(GTBehavior *b) {
    beginInsertRows(QModelIndex(), behaviors.size(), behaviors.size());
    behaviors.push_back({b->uid, b});
    endInsertRows();
    return true;
}

int BehaviorsListModel::removeAll(std::string toRemove) {
    vector<int> indicies;
    int remCnt = 0;
    for(int i = behaviors.size()-1; i >= 0; --i) {
        if(behaviors[i].uid == toRemove) {
            indicies.push_back(i);
        }
    }

    for(int i = 0; i < indicies.size(); ++i) {
        if(removeRow(indicies[i])) {
            ++remCnt;
        }
    }
    return remCnt;
}

void BehaviorsListModel::fixLinks() {
    for(auto& behavior : behaviors) {
        behavior.ptr = GTBehavior::find(behavior.uid);
        if(behavior.ptr == nullptr) {
            qDebug() << "OOHHHHHH NOOOOOOOOO fixLinks found dead uid" << QString::fromStdString(behavior.uid);
        }
    }
}
