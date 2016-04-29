#ifndef MYJSONMODEL_H
#define MYJSONMODEL_H

#include <QAbstractItemModel>
#include <QJsonDocument>

#include "myjsonmodelitem.h"

struct QJsonParseError;

class MyJsonModel : public QAbstractItemModel
{
  Q_OBJECT

public:
	MyJsonModel(QObject* = 0);

	QVariant headerData(int section, Qt::Orientation orientation, int role) const;
	QModelIndex index(int row, int column, const QModelIndex &parent = QModelIndex()) const;
	QModelIndex parent(const QModelIndex &index) const;
	int rowCount(const QModelIndex &parent = QModelIndex()) const;
	int columnCount(const QModelIndex &parent = QModelIndex()) const;
	QVariant data(const QModelIndex &index, int role) const;

	bool loadJson(const QByteArray &, QJsonParseError* = 0);
	QByteArray toJson();

private:
	QJsonDocument jsonDoc;

	MyJsonModelItem* root;
	QStringList headers;
};

#endif // MYJSONMODEL_H
