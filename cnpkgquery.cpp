#include "cnpkgquery.h"

CNPkgquery::CNPkgquery(const QString& pkgname, QObject *parent)
    : QObject(parent), m_pkgname(pkgname)
{

}

// 查询包
PkgInfo CNPkgquery::queryPkg()
{
    PkgInfo _info;

    // 利用事件循环
    QEventLoop eventLoop;

    QNetworkRequest request;

    //TODO Url编码
    QByteArray sUrl = QUrl::toPercentEncoding(m_pkgname.toUtf8());
    QString url = QString::fromUtf8(QString("https://build.archlinuxcn.org/api/v2/packages/").toUtf8() + sUrl);

    //request.setUrl(QUrl("https://build.archlinuxcn.org/api/v2/packages/" + m_pkgname));
    request.setUrl(url);
    QNetworkAccessManager manager;

    QObject::connect(&manager, SIGNAL(finished(QNetworkReply *)), &eventLoop, SLOT(quit()));
    QNetworkReply *reply = manager.get(request);

    eventLoop.exec();
    qDebug()<<"ArchLinuxCN query finished!";

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
        // 仅获取第一个符合的包
        QJsonObject pkgObj = document.object();

        // 获取JSON值
        QString pkgname = pkgObj.value("name").toString();
        QString pkgver = pkgObj.value("version").toString();
        QString maintainers;
        QJsonValue maintainersValue = pkgObj.value("users");
        if(maintainersValue.isArray())
        {
            QJsonArray maintainersArr = maintainersValue.toArray();
            for(auto maintainerItem : maintainersArr)
            {
                if(maintainers.isEmpty())
                    maintainers += maintainerItem.toString();
                else
                    maintainers += " " + maintainerItem.toString();
            }
        }

        // 赋值
        _info.pkgname = pkgname;
        _info.pkgver = pkgver;
        _info.maintainers = maintainers;
        _info.isAUR = false;
    }

    return _info;
}
