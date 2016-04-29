#include "myjsonmodelitem.h"

QRegExp MyJsonModelItem::r_int = QRegExp("\\d+");
QRegExp MyJsonModelItem::r_float = QRegExp("(?:\\d+)?\\.\\d+");

// {T_NULL, T_BOOL, T_INT, T_FLOAT, T_STRING, T_TEXT, T_ARRAY, T_OBJECT, T_UNKOWN};
QString MyJsonModelItem::mapType2Label[T_LEN] = {"null", "bool", "integer", "float", "string", "text", "array", "object", "unknown"};
QString MyJsonModelItem::mapType2Color[T_LEN] = {"lightblue", "olive", "maroon", "brown", "olive", "green", "orange", "purple", "red"};

MyJsonModelItem::MyJsonModelItem()
{
}

MyJsonModelItem::~MyJsonModelItem()
{
	qDeleteAll(children);
}

MyJsonModelItem::Type MyJsonModelItem::setTypeByValue(const QJsonValue& v)
{
	MyJsonModelItem::Type t;

	switch(v.type()) {
		case QJsonValue::Null:
			t = T_NULL;
			break;
		case QJsonValue::Bool:
			t = T_BOOL;
			break;
		case QJsonValue::Double:
			t = v.toVariant().toString().indexOf(QChar('.')) == -1 ? T_INT : T_FLOAT;
			break;
		case QJsonValue::String:
			if (MyJsonModelItem::r_int.exactMatch(v.toString())) {
				t = T_INT;
			} else if (MyJsonModelItem::r_float.exactMatch(v.toString())) {
				t = T_FLOAT;
			} else {
				t = v.toString().length() > 80 ? T_TEXT : T_STRING;
			}
			break;
		case QJsonValue::Array:
			t = T_ARRAY;
			break;
		case QJsonValue::Object:
			t = T_OBJECT;
			break;
		default:
			t = T_UNKOWN;
	}

	return t;
}

MyJsonModelItem* MyJsonModelItem::load(const QJsonValue& v, MyJsonModelItem* p)
{
	MyJsonModelItem* root = new MyJsonModelItem;
	root->key = "root";
	root->parent = p;

	if (v.isObject()) {
		foreach(QString key, v.toObject().keys()) {
			QJsonValue value = v.toObject().value(key);
			MyJsonModelItem* child = load(value, root);
			child->key = key;
			child->type = MyJsonModelItem::setTypeByValue(value);
			root->children.append(child);
		}
	} else if (v.isArray()) {
		QJsonArray tmp = v.toArray();
		int count = tmp.count();
		bool flag = tmp[0].isObject() && tmp[count - 1].isObject() && tmp[count / 2].isObject();
		int i = 0;
		foreach(QJsonValue value, tmp) {
			MyJsonModelItem* child = load(value, root);
			child->key = flag ? "..@.." : (value.isObject() ? "@" : "");
			child->type = MyJsonModelItem::setTypeByValue(value);
			root->children.append(child);
			i++;
			if (flag) break;
		}
		root->value = QString("(%1)").arg(count);
	} else {
		root->type = MyJsonModelItem::setTypeByValue(v);
		root->value = v.toVariant().toString();
	}

	return root;
}

QString MyJsonModelItem::typeToLabel(MyJsonModelItem::Type t)
{
	if (t == T_ARRAY || t == T_OBJECT) {
		return "";
	}
	if (t == T_UNKOWN) {
		return "!!!";
	}
	return MyJsonModelItem::mapType2Label[t];
}

QColor MyJsonModelItem::typeToColor(MyJsonModelItem::Type t)
{
	return QColor(MyJsonModelItem::mapType2Color[t]);
}

int MyJsonModelItem::row() const
{
	if (parent)
		return parent->children.indexOf(const_cast<MyJsonModelItem*>(this));

	return 0;
}



