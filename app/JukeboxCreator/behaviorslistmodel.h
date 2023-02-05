#ifndef BEHAVIORSLISTMODEL_H
#define BEHAVIORSLISTMODEL_H

#include <QAbstractListModel>
#include "gtbehavior.h"
#include "jsonbase.h"
#include "assemblyunit.h"

class BehaviorsListModel : public QAbstractListModel, public JSONBase, public AssemblyUnit
{
    Q_OBJECT

public:
    BehaviorsListModel();
    explicit BehaviorsListModel(QObject *parent);

    int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;
    bool insertRows(int row, int count, const QModelIndex &parent = QModelIndex()) override;
    bool removeRows(int row, int count, const QModelIndex &parent = QModelIndex()) override;
    bool setData(const QModelIndex &index, const QVariant &value, int role = Qt::EditRole) override;
    Qt::ItemFlags flags(const QModelIndex &index) const override;
    Qt::DropActions supportedDropActions() const override;

    virtual bool Deserialize(const rapidjson::Value& obj) override;
    virtual bool Serialize(rapidjson::Writer<rapidjson::StringBuffer>* writer) const override;

    virtual std::string GenName() override;
    virtual std::string GetCode(std::filesystem::path root) override;

    bool push_back(GTBehavior *b);
    int removeAll(std::string toRemove);

    void fixLinks();

private:
    std::vector<GTBehaviorLink> behaviors;
};

#endif // BEHAVIORSLISTMODEL_H
