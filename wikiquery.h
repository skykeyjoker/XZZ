#ifndef WIKIQUERY_H
#define WIKIQUERY_H

#include <QObject>

#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QNetworkRequest>
#include <QHttpMultiPart>
#include <QHttpPart>

// Json
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QJsonValue>
#include <QJsonParseError>

#include <QEventLoop>

#include <QDebug>

#include "wikiresult.h"

class WikiQuery : public QObject
{
    Q_OBJECT
public:
    explicit WikiQuery(const QString& keyword, QObject *parent = nullptr);

    WikiResult queryWiki();
private:
    QString m_keyword;
    QList<WikiResult> results;

signals:

};

#endif // WIKIQUERY_H
