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

#include <QDateTime>

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

    const QString urlPathPrefix = "https://aur.archlinux.org";
    const QString aurPrefix_tuna = "https://aur.tuna.tsinghua.edu.cn";
    const QString aurPrefix = "https://aur.archlinux.org/packages/";
    QString m_pkgname;
    QNetworkAccessManager manager;
signals:

};

#endif // PKGQUERY_H
