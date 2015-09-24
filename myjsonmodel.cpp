#include "myjsonmodel.h"

MyJsonModel::MyJsonModel()
{
}

bool MyJsonModel::loadJson(const QByteArray &json, QJsonParseError *e)
{
  jsonDoc = QJsonDocument::fromJson(json, e);
  return e->error != QJsonParseError::NoError ? false : true;
}

QByteArray MyJsonModel::toJson()
{
  return jsonDoc.toJson(QJsonDocument::Indented);
}
