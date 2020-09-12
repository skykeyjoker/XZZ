#ifndef CNPKGQUERY_H
#define CNPKGQUERY_H

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

#include <QDateTime>

#include <QDebug>

#include "pkginfo.h"

class CNPkgquery : public QObject
{
    Q_OBJECT
public:
    explicit CNPkgquery(const QString& pkgname, QObject *parent = nullptr);
    PkgInfo queryPkg();
private:
    QString m_pkgname;
    QNetworkAccessManager manager;
signals:

};

#endif // CNPKGQUERY_H
