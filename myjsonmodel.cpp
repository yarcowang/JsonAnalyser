#include "myjsonmodel.h"

#include <QJsonParseError>

MyJsonModel::MyJsonModel(QObject* parent) :
	QAbstractItemModel(parent)
{
	root = new MyJsonModelItem;
	headers.append("Key");
	headers.append("Value");
	headers.append("Type");
}

bool MyJsonModel::loadJson(const QByteArray &json, QJsonParseError *e)
{
  bool ret;
  jsonDoc = QJsonDocument::fromJson(json, e);
  ret = e->error != QJsonParseError::NoError ? false : true;

	if (ret && !jsonDoc.isNull() &&!jsonDoc.isEmpty()) {
		beginResetModel();
		if (jsonDoc.isArray()) {
			root = MyJsonModelItem::load(QJsonValue(jsonDoc.array()));
		} else {
			root = MyJsonModelItem::load(QJsonValue(jsonDoc.object()));
		}
		endResetModel();
	}

	return ret;
}

QByteArray MyJsonModel::toJson()
{
  return jsonDoc.toJson(QJsonDocument::Indented);
}

QVariant MyJsonModel::headerData(int section, Qt::Orientation orientation, int role) const
{
	if (orientation == Qt::Horizontal && role == Qt::DisplayRole) {
		return headers.value(section);
	}
	return QVariant();
}

QModelIndex MyJsonModel::index(int row, int column, const QModelIndex &parent) const
{
  if (!hasIndex(row, column, parent))
    return QModelIndex();

  MyJsonModelItem* p;
  if (!parent.isValid()) {
    p = root;
  } else {
    p = static_cast<MyJsonModelItem*>(parent.internalPointer());
  }

  MyJsonModelItem* child = p->children.value(row);
  if (child) {
    return createIndex(row, column, child);
  } else {
    return QModelIndex();
  }
}

QModelIndex MyJsonModel::parent(const QModelIndex &index) const
{
  if (!index.isValid())
      return QModelIndex();

  MyJsonModelItem *childItem = static_cast<MyJsonModelItem*>(index.internalPointer());
  MyJsonModelItem *parentItem = childItem->parent;

  if (parentItem == root)
      return QModelIndex();

  return createIndex(parentItem->row(), 0, parentItem);
}

int MyJsonModel::rowCount(const QModelIndex &parent) const
{
  MyJsonModelItem *parentItem;
  if (parent.column() > 0)
      return 0;

  if (!parent.isValid())
      parentItem = root;
  else
      parentItem = static_cast<MyJsonModelItem*>(parent.internalPointer());

  return parentItem->children.count();
}

int MyJsonModel::columnCount(const QModelIndex &parent) const
{
  Q_UNUSED(parent)
  return 3;
}

QVariant MyJsonModel::data(const QModelIndex &index, int role) const
{
  if (!index.isValid()) return QVariant();

  MyJsonModelItem *item = static_cast<MyJsonModelItem*>(index.internalPointer());

	if (role == Qt::DisplayRole) {
		if (index.column() == 0)
			return QString("%1").arg(item->key);

		if (index.column() == 1)
			return QString("%1").arg(item->value.length() > 20 ? item->value.left(20) + "..." : item->value);

		if (index.column() == 2)
			return QString("%1").arg(MyJsonModelItem::typeToLabel(item->type));
	} else if (role == Qt::ForegroundRole) {
		return MyJsonModelItem::typeToColor(item->type);
	}

	return QVariant();
}
