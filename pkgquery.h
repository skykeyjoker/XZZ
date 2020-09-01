#ifndef PKGQUERY_H
#define PKGQUERY_H

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

#include "pkginfo.h"

class PkgQuery : public QObject
{
    Q_OBJECT
public:
    explicit PkgQuery(const QString& pkgname, QObject *parent = nullptr);

    PkgInfo queryPkg();
    PkgInfo queryPkgInOfficial();
    PkgInfo queryPkgInAUR();

private:


    QString _pkgname;
    QNetworkAccessManager manager;
signals:

};

#endif // PKGQUERY_H
