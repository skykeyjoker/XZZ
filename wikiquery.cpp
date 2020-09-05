#include "wikiquery.h"

WikiQuery::WikiQuery(const QString& keyword, QObject *parent)
    : QObject(parent), _keyword(keyword)
{

}

// 查询Wiki
QString WikiQuery::queryWiki()
{
    QString url;

    // 调用QProcess获取输出
    // curl https://wiki.archlinux.org -d 'search=vim' -v
    QProcess process;



    qDebug()<<"Curl Wiki Started";



    process.start(tr("curl https://wiki.archlinux.org -d \'search=%1\' -v").arg(_keyword));

    process.waitForFinished();

    QByteArray buf = process.readAllStandardOutput();
    QString ret = buf;

    qDebug()<<ret;

    // 判断返回是否含有 location
    if(ret.contains("< location:"))
    {
        qDebug()<<"contains";
        url = ret.mid(ret.indexOf("< location:")+11, ret.indexOf("< x-powered-by:")-ret.indexOf("< location:")-11+1);
    }

    qDebug()<<"Curl Wiki finished";

    return url;
}

