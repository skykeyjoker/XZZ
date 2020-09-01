#include "setuquery.h"

SetuQuery::SetuQuery(const QString& keyword, QObject *parent)
    : QObject(parent), _keyword(keyword)
{

}

SetuInfo SetuQuery::querySetu()
{
    SetuInfo _info;

    // 利用事件循环
    QEventLoop eventLoop;

    QNetworkRequest request;
    QUrl url;

    if(_keyword.isEmpty())
        url.setUrl("https://api.lolicon.app/setu/?apikey=" + APIkey + "&" + "r18=1&num=1");  // 无关键词
    else
        url.setUrl("https://api.lolicon.app/setu/?apikey=" + APIkey + "&keyword=" + QUrl::toPercentEncoding(_keyword) + "&" + "r18=1&num=1");  // 含有关键词

    request.setUrl(url);

    QNetworkAccessManager manager;

    QObject::connect(&manager, SIGNAL(finished(QNetworkReply *)), &eventLoop, SLOT(quit()));
    QNetworkReply *reply = manager.get(request);

    eventLoop.exec();
    qDebug()<<"Setu query finished!";

    // 获取所有返回
    QByteArray buf = reply->readAll();

    // 转换为JSON
    QJsonParseError err;
    QJsonDocument document = QJsonDocument::fromJson(buf, &err);

    if(err.error != QJsonParseError::NoError)
    {
        qDebug()<<"转换JSON失败";
    }
    else
    {
        int code = document.object().value("code").toInt();
        int quota = document.object().value("quota").toInt();
        int quota_min_ttl = document.object().value("quota_min_ttl").toInt();

        // "data" Array
        QJsonArray dataArr = document.object().value("data").toArray();

        // 仅获取第一个图
        QJsonObject setuObj = dataArr[0].toObject();

        // 获取JSON值
        QString url = setuObj.value("url").toString();

        // 赋值
        _info.url = url;
        _info.code = code;
        _info.quota = quota;
        _info.quota_min_ttl = quota_min_ttl;
    }

    return _info;
}
