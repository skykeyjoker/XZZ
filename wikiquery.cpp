#include "wikiquery.h"

WikiQuery::WikiQuery(const QString& keyword, QObject *parent)
    : QObject(parent), m_keyword(keyword)
{

}

// 查询Wiki
QString WikiQuery::queryWiki()
{
    QString url;

    // 调用QProcess获取输出
    // https://wiki.archlinux.org 'search=vim'
    qDebug()<<"Curl Wiki Started";

    // 利用事件循环
    QEventLoop eventLoop;

    QNetworkRequest request;
    request.setUrl(QUrl("https://wiki.archlinux.org"));
    QByteArray data = tr("search=").arg(m_keyword).toUtf8();

//    // MultiPart
//    QHttpMultiPart *multiPart = new QHttpMultiPart(QHttpMultiPart::FormDataType);
//    QHttpPart part;
//    part.setHeader(QNetworkRequest::ContentTypeHeader, QVariant("text/plain"));
//    part.setBody(tr("search=%1").arg(_keyword).toUtf8());
//    multiPart->append(part);

    QNetworkAccessManager manager;

    QObject::connect(&manager, SIGNAL(finished(QNetworkReply *)), &eventLoop, SLOT(quit()));
    QNetworkReply *reply = manager.post(request, data);

    eventLoop.exec();
    qDebug()<<"Curl Wiki finished";

    QByteArray buf = reply->readAll();
    qDebug()<<buf;

    return url;
}

