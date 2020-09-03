#include "pkgquery.h"

PkgQuery::PkgQuery(const QString &pkgname, QObject *parent)
    : _pkgname(pkgname), QObject(parent)
{

}

// 查询包
PkgInfo PkgQuery::queryPkg()
{
    PkgInfo _info;

    if(_pkgname.isEmpty())
        return _info;

    qDebug()<<"start query";


    // 首先查询官方仓库
    _info = queryPkgInOfficial();

    if(_info.pkgname.isEmpty())
    {
        // 在AUR仓库中查询
        _info = queryPkgInAUR();
    }


    return _info;
}

// 在官方仓库中查询
PkgInfo PkgQuery::queryPkgInOfficial()
{
    PkgInfo _info;

    // 利用事件循环
    QEventLoop eventLoop;

    QNetworkRequest request;
    request.setUrl(QUrl("https://www.archlinux.org/packages/search/json/?name=" + _pkgname));
    QNetworkAccessManager manager;

    QObject::connect(&manager, SIGNAL(finished(QNetworkReply *)), &eventLoop, SLOT(quit()));
    QNetworkReply *reply = manager.get(request);

    eventLoop.exec();
    qDebug()<<"Official query finished!";

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
        // "results" Array
        QJsonArray resultsArr = document.object().value("results").toArray();

        // 仅获取第一个符合的包
        QJsonObject pkgObj = resultsArr[0].toObject();

        // 获取JSON值
        QString pkgname = pkgObj.value("pkgname").toString();
        QString pkgdesc = pkgObj.value("pkgdesc").toString();
        QString pkgver = pkgObj.value("pkgver").toString() + "-" + pkgObj.value("pkgrel").toString();
        QString url = pkgObj.value("url").toString();
        QString last_update = pkgObj.value("last_update").toString();
        QString repo = pkgObj.value("repo").toString();
        QString maintainers;
        QJsonValue maintainersValue = pkgObj.value("maintainers");
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


        // LastUpdate时间格式转换

        QDateTime dateTime = QDateTime::fromString(last_update, "yyyy-MM-ddThh:mm:ss.zzzZ");
        last_update = dateTime.toString("yyyy年MM月dd日 hh:mm:ss");

        // 赋值
        _info.pkgname = pkgname;
        _info.pkgdesc = pkgdesc;
        _info.pkgver = pkgver;
        _info.url = url;
        _info.last_update = last_update;
        _info.maintainers = maintainers;
        _info.isAUR = false;
        _info.repo = repo;
    }

    return _info;
}

// 在AUR中查询
PkgInfo PkgQuery::queryPkgInAUR()
{
    PkgInfo _info;

    // 利用事件循环
    QEventLoop eventLoop;

    QNetworkRequest request;
    request.setUrl(QUrl("https://aur.archlinux.org/rpc/?v=5&type=search&arg=" + _pkgname));
    QNetworkAccessManager manager;

    QObject::connect(&manager, SIGNAL(finished(QNetworkReply *)), &eventLoop, SLOT(quit()));
    QNetworkReply *reply = manager.get(request);

    eventLoop.exec();
    qDebug()<<"AUR query finished!";

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
        // 首先判断 "resultcount" 字段
        if(document.object().value("resultcount").toInt() <= 0)
            return _info;  // 结果为空

        // "results" Array
        QJsonArray resultsArr = document.object().value("results").toArray();

        // 仅获取第一个符合的包
        QJsonObject pkgObj = resultsArr[0].toObject();

        // 获取JSON值
        QString pkgname = pkgObj.value("Name").toString();
        QString pkgdesc = pkgObj.value("Description").toString();
        QString pkgver = pkgObj.value("Version").toString();
        QString url = pkgObj.value("URL").toString();

        // outofdate 以时间戳形式，需要特殊处理
        QString outofdate;
        int outofdateInt = pkgObj.value("OutOfDate").toInt();
        if(outofdateInt != 0)
        {
            //qDebug()<<"outofdateInt:"<<outofdateInt;
            QDateTime dateTime = QDateTime::fromSecsSinceEpoch(outofdateInt);
            outofdate = dateTime.toString("yyyy年MM月dd日 hh:mm:ss");
        }

        QString maintainers = pkgObj.value("Maintainer").toString();
        int numvotes = pkgObj.value("NumVotes").toInt();
        QString urlPath = pkgObj.value("URLPath").toString();

        // 赋值
        _info.pkgname = pkgname;
        _info.pkgdesc = pkgdesc;
        _info.pkgver = pkgver;
        _info.outofdate = outofdate;
        _info.url = url;
        _info.maintainers = maintainers;
        _info.numvotes = numvotes;
        _info.isAUR = true;
        _info.urlPath = aurPrefix + pkgname;
    }

    return _info;
}
