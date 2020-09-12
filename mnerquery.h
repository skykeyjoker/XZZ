#ifndef MNERQUERY_H
#define MNERQUERY_H

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

#include "mnerinfo.h"

class MnerQuery : public QObject
{
    Q_OBJECT
public:
    explicit MnerQuery(const QString& mnerName, QObject *parent = nullptr);

    MnerInfo queryMner();

private:
    QString m_mnerName;
signals:

};

#endif // MNERQUERY_H
