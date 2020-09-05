#ifndef WIKIQUERY_H
#define WIKIQUERY_H

#include <QObject>

#include <QProcess>
#include <QEventLoop>

#include <QDebug>

class WikiQuery : public QObject
{
    Q_OBJECT
public:
    explicit WikiQuery(const QString& keyword, QObject *parent = nullptr);

    QString queryWiki();
private:
    QString _keyword;

signals:

};

#endif // WIKIQUERY_H
