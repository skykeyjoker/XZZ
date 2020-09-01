#ifndef SETUQUERY_H
#define SETUQUERY_H

#include <QObject>

#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QNetworkRequest>

// Json
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QJsonValue>
#include <QJsonParseError>

#include <QEventLoop>

#include <QDebug>

#include "setuinfo.h"

class SetuQuery : public QObject
{
    Q_OBJECT
public:
    explicit SetuQuery(const QString& keyword, QObject *parent = nullptr);
    SetuInfo querySetu();
private:
    QString _keyword;
    const QString APIkey = "587436385f4da24a6182c1";
signals:

};

#endif // SETUQUERY_H
