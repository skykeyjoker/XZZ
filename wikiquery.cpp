#include "wikiquery.h"

WikiQuery::WikiQuery(const QString& keyword, QObject *parent)
    : QObject(parent), m_keyword(keyword)
{

}

// 查询Wiki
WikiResult WikiQuery::queryWiki()
{
    WikiResult result;

    // 调用QProcess获取输出
    // https://wiki.archlinux.org/api.php?action=opensearch&format=json&formatversion=2&search=123&namespace=0|3000&limit=10&suggest=true
    qDebug()<<"Wiki Started";

    // 利用事件循环
    QEventLoop eventLoop;

    QNetworkRequest request;

    // Url编码
    QByteArray sUrl = QUrl::toPercentEncoding(m_keyword);
    QString url = QString::fromUtf8(QString("https://wiki.archlinux.org/api.php?action=opensearch&format=json&formatversion=2&search=").toUtf8() + sUrl + QString("&namespace=0|3000&limit=10&suggest=true").toUtf8());
    //request.setUrl(QUrl(tr("https://wiki.archlinux.org/api.php?action=opensearch&format=json&formatversion=2&search=%1&namespace=0|3000&limit=10&suggest=true").arg(m_keyword)));
    request.setUrl(url);

    QNetworkAccessManager manager;

    QObject::connect(&manager, SIGNAL(finished(QNetworkReply *)), &eventLoop, SLOT(quit()));
    QNetworkReply *reply = manager.get(request);

    eventLoop.exec();
    qDebug()<<"Wiki finished";

    // 获取所有返回
    QByteArray buf = reply->readAll();

    // 转换为JSON
    QJsonParseError err;
    QJsonDocument document = QJsonDocument::fromJson(buf, &err);

    if (err.error != QJsonParseError::NoError) {
        qDebug()<<"JSON 转换失败";
    } else {
        QJsonArray rootArr = document.array(); // 根数组
        QJsonValue keyword = rootArr.at(0);  // 关键字
        QJsonArray titles = rootArr.at(1).toArray();  // 第一个返回数组，标题
        QJsonArray arr2 = rootArr.at(2).toArray();  // 第二个返回数组
        QJsonArray links = rootArr.at(3).toArray();  // 第三个返回数组，链接

        for (int i = 0; i<titles.size(); ++i) {
            // 将返回的URL解码
            QString linkRet = QUrl::fromPercentEncoding(links.at(i).toString().toUtf8());
            //WikiResult tmpResult(titles.at(i).toString(),links.at(i).toString());
            WikiResult tmpResult(titles.at(i).toString(),linkRet);
            results.push_back(tmpResult);
        }
    }

    // 若有检索结果，只返回第一个界面
    if (!results.isEmpty())
        result = results.at(0);
    return result;
}

