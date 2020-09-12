#include "setuquery.h"

SetuQuery::SetuQuery(const QString& keyword, QObject *parent)
    : QObject(parent), m_keyword(keyword)
{

}

SetuInfo SetuQuery::querySetu()
{
    SetuInfo _info;

    // 利用事件循环
    QEventLoop eventLoop;

    QNetworkRequest request;
    QUrl url;

    if(m_keyword.isEmpty())
        url.setUrl("https://api.lolicon.app/setu/?apikey=" + APIkey + "&" + "r18=1&num=1&size1200=true");  // 无关键词
    else
        url.setUrl("https://api.lolicon.app/setu/?apikey=" + APIkey + "&keyword=" + QUrl::toPercentEncoding(m_keyword) + "&" + "r18=1&num=1&size1200=true");  // 含有关键词

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
        _info.url = url.mid(url.lastIndexOf("/")+1);
        _info.code = code;
        _info.quota = quota;
        _info.quota_min_ttl = quota_min_ttl;

        // 下载图片
        if(!url.isEmpty())
            downloadPhoto(url);
    }

    return _info;
}

void SetuQuery::downloadPhoto(const QString &url)
{
    // 检查目录下图片缓存目录 pics 是否存在
    QDir dir = QDir::currentPath();
    if(!dir.exists("/pics"))
    {
        dir.mkdir("pics");
    }

    downloadFile = new QFile("pics/"+url.mid(url.lastIndexOf("/")+1));
    if(!downloadFile->open(QIODevice::WriteOnly))
    {
        qDebug()<<"图片打开失败";
        delete  downloadFile;
        downloadFile = 0;
    }

    QEventLoop evenLoop;
    // 开始下载
    QNetworkRequest request;
    request.setUrl(url);
    downloadReply = downloadManager.get(request);

    // 下载完成后
    connect(downloadReply, SIGNAL(finished()), this, SLOT(downloadFinished()));
    connect(downloadReply,&QNetworkReply::finished, &evenLoop, &QEventLoop::quit);
    // 有可用数据
    connect(downloadReply, SIGNAL(readyRead()), this, SLOT(downloadReady()));
    // 下载进度更新
    connect(downloadReply, SIGNAL(downloadProgress(qint64,qint64)), this, SLOT(updateDataReadProgress(qint64, qint64)));

    evenLoop.exec();
}

void SetuQuery::downloadFinished()
{
    downloadFile->flush();
    downloadFile->close();
    downloadReply->deleteLater();
    downloadReply = 0;
    delete  downloadFile;
    downloadFile = 0;
}

void SetuQuery::downloadReady()
{
    if(downloadFile)
        downloadFile->write(downloadReply->readAll());
}

void SetuQuery::updateDataReadProgress(qint64 bytesRead, qint64 totalBytes)
{
    qDebug()<<bytesRead<<":"<<totalBytes;
}
