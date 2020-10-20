#include "mnerquery.h"

MnerQuery::MnerQuery(const QString& mnerName, QObject *parent)
    : QObject(parent), m_mnerName(mnerName)
{

}

// 查询打包人信息
MnerInfo MnerQuery::queryMner()
{
    MnerInfo _info;

    if(m_mnerName.isEmpty())
        return _info;

    // 查询
    // 利用事件循环
    QEventLoop eventLoop;

    QNetworkRequest request;

    // Url编码
    QByteArray sUrl = QUrl::toPercentEncoding(m_mnerName.toUtf8());
    QString url = QString::fromUtf8(QString("https://aur.archlinux.org/rpc/?v=5&type=search&by=maintainer&arg=").toUtf8() + sUrl);

    //request.setUrl(QUrl("https://aur.archlinux.org/rpc/?v=5&type=search&by=maintainer&arg=" + m_mnerName));
    request.setUrl(url);
    QNetworkAccessManager manager;

    QObject::connect(&manager, SIGNAL(finished(QNetworkReply *)), &eventLoop, SLOT(quit()));
    QNetworkReply *reply = manager.get(request);

    eventLoop.exec();
    qDebug()<<"Mner query finished!";

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
        int count = document.object().value("resultcount").toInt();
        // "results" Array
        QJsonArray resultsArr = document.object().value("results").toArray();

        // 仅获取前五个包
        QStringList packageList;
        for(auto ite : resultsArr)
        {
            if(packageList.count()>=5)
                break;

            // 获取当前包名
            QString currentPkgName;
            currentPkgName = ite.toObject().value("Name").toString();

            // 添加到包列表
            packageList.append(currentPkgName);
        }

        // 赋值
        _info.mnerName = m_mnerName;
        _info.count = count;
        _info.packageList = packageList;
    }

    return _info;
}
