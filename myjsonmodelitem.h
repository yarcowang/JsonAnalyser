#ifndef MYJSONMODELITEM_H
#define MYJSONMODELITEM_H

#include <QtCore>
#include <QtGui>
#include <QJsonValue>

class MyJsonModelItem
{
public:
  enum Type {T_NULL, T_BOOL, T_INT, T_FLOAT, T_STRING, T_TEXT, T_ARRAY, T_OBJECT, T_UNKOWN, T_LEN};

  MyJsonModelItem();
  ~MyJsonModelItem();

	Type type;
	QString key;
	QString value;

	MyJsonModelItem* parent;
	QList<MyJsonModelItem*> children;

private:
	static Type setTypeByValue(const QJsonValue&);

public:
	static MyJsonModelItem* load(const QJsonValue&, MyJsonModelItem* = 0);
	static QString typeToLabel(MyJsonModelItem::Type);
	static QColor typeToColor(MyJsonModelItem::Type);
	int row() const;

	static QString mapType2Label[T_LEN];
	static QString mapType2Color[T_LEN];

private:
	static QRegExp r_int;
	static QRegExp r_float;

};

#endif // MYJSONMODELITEM_H
