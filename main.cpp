#include <QCoreApplication>
#include <mirai.h>

#include "pkginfo.h"
#include "pkgquery.h"
#include "cnpkgquery.h"
#include "mnerinfo.h"
#include "mnerquery.h"
#include "setuinfo.h"
#include "setuquery.h"
#include "wikiquery.h"
#include "whitelist.h"
#include "wikiresult.h"

bool setu = false;

using namespace Cyan;

int main(int argc, char *argv[])
{

    QCoreApplication a(argc, argv);

    //MiraiBot bot("39.97.253.64",9090); //初始化一个机器人
    MiraiBot bot("127.0.0.01",9090);
    QQ_t botqq = QQ_t(3625434216);

    while(true)
    {
        try
        {
            // bot认证
            bot.Auth("1234567890", botqq);
            break;
        }
        catch (const std::exception& ex)
        {
            std::cout<<ex.what()<<std::endl;
        }
        MiraiBot::SleepSeconds(1);
    }
    std::cout<<"Bot Working..."<<std::endl;

    std::map<GID_t, bool> groups;

    bot.On<GroupMessage>(
                [&](GroupMessage m)
    {
        try
        {
            // 获取群消息
            std::string plain = m.MessageChain.GetPlainText();
            QString qplain = QString::fromStdString(plain);

            // help
            if(plain == "/help" || plain == "!help")
            {
                // 生成消息体
                QString messageBody;
                messageBody.append("咱现在支持以下命令：\n");
                messageBody.append("ping\n");
                messageBody.append("pkg  ${pkgName}\n");
                messageBody.append("off  ${pkgName}\n");
                messageBody.append("aur  ${pkgName}\n");
                messageBody.append("cnpkg ${pkgName}\n");
                messageBody.append("mner  ${mnerName}\n");
                messageBody.append("wiki  ${keyWord}\n");
                messageBody.append("enable lsp\n");
                messageBody.append("disable lsp\n");
                messageBody.append("lsp\n");
                messageBody.append("lsp  ${keyWord}");

                // 发送消息
                m.Reply(MessageChain().Plain(messageBody.toStdString()));
            }

            // ping
            if(plain == "/ping" || plain =="!ping")
            {
                // 这句话不知道是干啥的
                groups[m.Sender.Group.GID] = true;

                //m.Reply(MessageChain().Plain("小易吃粑粑！"));
                // Send photo
                GroupImage img = bot.UploadGroupImage("ping.jpg");
                m.Reply(MessageChain().Image(img));


                return;
            }

            // pkg
            if(qplain.startsWith("/pkg") || qplain.startsWith("!pkg"))
            {
                // 这句话不知道是干啥的
                groups[m.Sender.Group.GID] = true;

                QString prefix = qplain.at(0); // 获取前缀
                // 获取包名
                QString pkgname;
                if (qplain.count(' ') > 1) {
                    QString tStr = QString::fromStdString(m.MessageChain.GetPlainText());
                    tStr = tStr.trimmed();  // 去掉两端空格
                    tStr = tStr.split(prefix + "pkg").at(1); // 去掉头部
                    tStr = tStr.trimmed(); // 再次去掉两次空格
                    if (tStr.contains(" ")) {
                        // 若tStr还有空格，则只取第一个空格前的数据
                        pkgname = tStr.section(" ",0,0);
                    } else {
                        // 没有空格，tStr即为包名
                        pkgname = tStr;
                    }

                } else {
                    pkgname = QString::fromStdString(m.MessageChain.GetPlainText()).section(" ",1,1);
                }

                // 如果包名为空
                if (pkgname.isEmpty()) {
                    m.Reply(MessageChain().Plain("请输入包名！"));

                    return;
                }

                // 查询包
                qDebug()<<"Pkgname: "<< pkgname;
                PkgQuery *query = new PkgQuery(pkgname);
                PkgInfo ret = query->queryPkg();

                if (!ret.pkgname.isEmpty())
                {
                    if (!ret.isAUR)  // 官方仓库
                    {
                        // 生成消息体
                        QString messageBody;
                        messageBody.append("仓库    : 官方 [" + ret.repo + "]");
                        messageBody.append("\n包名    : " + ret.pkgname);
                        messageBody.append("\n版本    : " + ret.pkgver);
                        messageBody.append("\n描述    : " + ret.pkgdesc);
                        messageBody.append("\n维护    : " + ret.maintainers);
                        messageBody.append("\n更新    : " + ret.last_update);
                        if(ret.flag_date.isEmpty())
                            messageBody.append("\n过期    : 未过期");
                        else
                            messageBody.append("\n过期    : " + ret.flag_date);
                        messageBody.append("\n网址    : " + ret.url);

                        // 发送消息
                        m.Reply(MessageChain().Plain(messageBody.toStdString()));
                    }
                    else  // AUR仓库
                    {
                        // 生成消息体
                        QString messageBody;
                        messageBody.append("仓库    : AUR");
                        messageBody.append("\n包名    : " + ret.pkgname);
                        messageBody.append("\n版本    : " + ret.pkgver);
                        messageBody.append("\n描述    : " + ret.pkgdesc);
                        messageBody.append("\n维护    : " + ret.maintainers);
                        messageBody.append("\n投票    : " + QString::number(ret.numvotes));
                        messageBody.append("\n更新    : " + ret.lastModified);
                        if(ret.outofdate.isEmpty())
                            messageBody.append("\n过期    : 未过期");
                        else
                            messageBody.append("\n过期    : " + ret.outofdate);
                        messageBody.append("\n地址    : " + ret.url);
                        messageBody.append("\n链接    : " + ret.urlPath);

                        // 发送消息
                        m.Reply(MessageChain().Plain(messageBody.toStdString()));
                    }
                } else {
                    m.Reply(MessageChain().Plain(QString("未能找到 " + pkgname +" !").toStdString()));
                }

                return;
            }

            // aur
            if(qplain.startsWith("/aur") || qplain.startsWith("!aur"))
            {
                // 这句话不知道是干啥的
                groups[m.Sender.Group.GID] = true;

                QString prefix = qplain.at(0); // 获取前缀
                // 获取包名
                QString pkgname;
                if (qplain.count(' ') > 1) {
                    QString tStr = QString::fromStdString(m.MessageChain.GetPlainText());
                    tStr = tStr.trimmed();  // 去掉两端空格
                    tStr = tStr.split(prefix + "aur").at(1); // 去掉头部
                    tStr = tStr.trimmed(); // 再次去掉两次空格
                    if (tStr.contains(" ")) {
                        // 若tStr还有空格，则只取第一个空格前的数据
                        pkgname = tStr.section(" ",0,0);
                    } else {
                        // 没有空格，tStr即为包名
                        pkgname = tStr;
                    }

                } else {
                    pkgname = QString::fromStdString(m.MessageChain.GetPlainText()).section(" ",1,1);
                }

                // 如果包名为空
                if (pkgname.isEmpty()) {
                    m.Reply(MessageChain().Plain("请输入包名！"));

                    return;
                }

                // 查询包
                qDebug()<<"Pkgname: "<< pkgname;
                PkgQuery *query = new PkgQuery(pkgname);
                PkgInfo ret = query->queryPkgInAUR();

                if(!ret.pkgname.isEmpty())
                {

                    // 生成消息体
                    QString messageBody;
                    messageBody.append("仓库    : AUR");
                    messageBody.append("\n包名    : " + ret.pkgname);
                    messageBody.append("\n版本    : " + ret.pkgver);
                    messageBody.append("\n描述    : " + ret.pkgdesc);
                    messageBody.append("\n维护    : " + ret.maintainers);
                    messageBody.append("\n投票    : " + QString::number(ret.numvotes));
                    messageBody.append("\n更新    : " + ret.lastModified);
                    if(ret.outofdate.isEmpty())
                        messageBody.append("\n过期    : 未过期");
                    else
                        messageBody.append("\n过期    : " + ret.outofdate);
                    messageBody.append("\n地址    : " + ret.url);
                    messageBody.append("\n链接    : " + ret.urlPath);

                    // 发送消息
                    m.Reply(MessageChain().Plain(messageBody.toStdString()));
                } else {
                    m.Reply(MessageChain().Plain(QString("未能找到 " + pkgname +" !").toStdString()));
                }

                return;
            }

            // off
            if(qplain.startsWith("/off") || qplain.startsWith("!off"))
            {
                // 这句话不知道是干啥的
                groups[m.Sender.Group.GID] = true;

                QString prefix = qplain.at(0); // 获取前缀
                // 获取包名
                QString pkgname;
                if (qplain.count(' ') > 1) {
                    QString tStr = QString::fromStdString(m.MessageChain.GetPlainText());
                    tStr = tStr.trimmed();  // 去掉两端空格
                    tStr = tStr.split(prefix + "off").at(1); // 去掉头部
                    tStr = tStr.trimmed(); // 再次去掉两次空格
                    if (tStr.contains(" ")) {
                        // 若tStr还有空格，则只取第一个空格前的数据
                        pkgname = tStr.section(" ",0,0);
                    } else {
                        // 没有空格，tStr即为包名
                        pkgname = tStr;
                    }

                } else {
                    pkgname = QString::fromStdString(m.MessageChain.GetPlainText()).section(" ",1,1);
                }

                // 如果包名为空
                if (pkgname.isEmpty()) {
                    m.Reply(MessageChain().Plain("请输入包名！"));

                    return;
                }

                // 查询包
                qDebug()<<"Pkgname: "<< pkgname;
                PkgQuery *query = new PkgQuery(pkgname);
                PkgInfo ret = query->queryPkgInOfficial();

                if(!ret.pkgname.isEmpty())
                {
                    // 生成消息体
                    QString messageBody;
                    messageBody.append("仓库    : 官方 [" + ret.repo + "]");
                    messageBody.append("\n包名    : " + ret.pkgname);
                    messageBody.append("\n版本    : " + ret.pkgver);
                    messageBody.append("\n描述    : " + ret.pkgdesc);
                    messageBody.append("\n维护    : " + ret.maintainers);
                    messageBody.append("\n更新    : " + ret.last_update);
                    if(ret.flag_date.isEmpty())
                        messageBody.append("\n过期    : 未过期");
                    else
                        messageBody.append("\n过期    : " + ret.flag_date);
                    messageBody.append("\n网址    : " + ret.url);

                    // 发送消息
                    m.Reply(MessageChain().Plain(messageBody.toStdString()));

                } else {
                    m.Reply(MessageChain().Plain(QString("未能找到 " + pkgname +" !").toStdString()));
                }

                return;
            }

            // mner
            if(qplain.startsWith("/mner") || qplain.startsWith("!mner"))
            {
                // 这句话不知道是干啥的
                groups[m.Sender.Group.GID] = true;

                QString prefix = qplain.at(0); // 获取前缀
                // 获取打包人姓名
                QString mnerName;
                if (qplain.count(' ') > 1) {
                    QString tStr = QString::fromStdString(m.MessageChain.GetPlainText());
                    tStr = tStr.trimmed();  // 去掉两端空格
                    tStr = tStr.split(prefix + "mner").at(1); // 去掉头部
                    tStr = tStr.trimmed(); // 再次去掉两次空格
                    if (tStr.contains(" ")) {
                        // 若tStr还有空格，则只取第一个空格前的数据
                        mnerName = tStr.section(" ",0,0);
                    } else {
                        // 没有空格，tStr即为包名
                        mnerName = tStr;
                    }

                } else {
                    mnerName = QString::fromStdString(m.MessageChain.GetPlainText()).section(" ",1,1);
                }

                // 如果打包人名称为空
                if (mnerName.isEmpty()) {
                    m.Reply(MessageChain().Plain("请输入打包人姓名！"));

                    return;
                }

                // 打包人姓名
                qDebug()<<"mner: "<<mnerName;

                MnerQuery *mnerQuery = new MnerQuery(mnerName);
                MnerInfo ret = mnerQuery->queryMner();

                // 生成消息体
                QString messageBody;
                messageBody = ret.mnerName + "共打了" + QString::number(ret.count) + "个包:";
                for(auto ite : ret.packageList)
                {
                    if(!ite.isEmpty())
                    {
                        messageBody = messageBody + "\n" + ite;
                    }
                }
                if(ret.count >= 5)
                    messageBody = messageBody + "\n...";

                // 发送消息
                m.Reply(MessageChain().Plain(messageBody.toStdString()));

                return;
            }

            // enable lsp
            if(qplain == "/enable lsp" || qplain == "!enable lsp")
            {
                // 这句咱也不知道是干啥的
                groups[m.Sender.Group.GID] = true;

                if(WhiteList.contains(m.Sender.Group.GID.ToInt64()))
                {
                    if(!setu)
                    {
                        setu = true;
                        m.Reply(MessageChain().Plain("上车！"));
                    }
                    else
                    {
                        m.Reply(MessageChain().Plain("咦，你不是已经在车上了吗？"));
                    }
                }
                else
                {
                    m.Reply(MessageChain().Plain("想什么呢！你可不是我的猪头少年！"));
                }

                return;
            }

            // disable lsp
            if(qplain == "/disable lsp" || qplain == "!disable lsp")
            {
                // 这句咱也不知道是干啥的
                groups[m.Sender.Group.GID] = true;

                if(WhiteList.contains(m.Sender.Group.GID.ToInt64()))
                {
                    if(setu)
                    {
                        setu = false;
                        m.Reply(MessageChain().Plain("下车！"));
                    }
                    else
                        m.Reply(MessageChain().Plain("不是吧啊Sir,还在催啊，人家早就下车了！"));
                }
                else
                {
                    m.Reply(MessageChain().Plain("想什么呢！你可不是我的猪头少年！"));
                }

                return;
            }

            // lsp
            if(qplain.startsWith("/lsp") || qplain.startsWith("!lsp"))
            {
                // 开启色图开关才能获取色图
                if(setu && WhiteList.contains(m.Sender.Group.GID))
                {
                    // 关键字
                    QString keyword = qplain.section(" ",1,1);

                    SetuQuery *setuQuery = new SetuQuery(keyword);
                    SetuInfo ret = setuQuery->querySetu();

                    if(ret.code == 404)
                    {
                        m.Reply(MessageChain().Plain("呀，没有找到符合的色图！"));

                        return;
                    }
                    if(ret.code != 0)
                    {
                        m.Reply(MessageChain().Plain("Woc，恭喜你，色图机器人坏掉了！"));

                        return;
                    }

                    qDebug()<<ret.url;

                    //MiraiImage miraiImage;
                    //miraiImage.Url = ret.url.toStdString();
                    //bot.SendMessage(m.Sender.Group.GID, MessageChain().Plain("咱帮你🔍找到了这个\n").Image(miraiImage));

                    QString imgPath = "pics/" + ret.url;
                    qDebug()<<ret.url<<" "<<imgPath;
                    GroupImage img = bot.UploadGroupImage(imgPath.toStdString());
                    //GroupImage img = bot.UploadGroupImage("http://i.pixiv.cat/img-original/img/2019/11/10/13/16/49/77742218_p0.png");
                    m.Reply(MessageChain().Plain("咱帮你🔍找到了这个\n").Image(img));

                    return;
                }
                else
                {
                    m.Reply(MessageChain().Plain("LSP被抓起来啦！"));
                }

                return;
            }


            // cnpkg
            if(qplain.startsWith("/cnpkg") || qplain.startsWith("!cnpkg"))
            {
                // 这句话不知道是干啥的
                groups[m.Sender.Group.GID] = true;

                QString prefix = qplain.at(0); // 获取前缀
                // 获取包名
                QString pkgname;
                if (qplain.count(' ') > 1) {
                    QString tStr = QString::fromStdString(m.MessageChain.GetPlainText());
                    tStr = tStr.trimmed();  // 去掉两端空格
                    tStr = tStr.split(prefix + "cnpkg").at(1); // 去掉头部
                    tStr = tStr.trimmed(); // 再次去掉两次空格
                    if (tStr.contains(" ")) {
                        // 若tStr还有空格，则只取第一个空格前的数据
                        pkgname = tStr.section(" ",0,0);
                    } else {
                        // 没有空格，tStr即为包名
                        pkgname = tStr;
                    }

                } else {
                    pkgname = QString::fromStdString(m.MessageChain.GetPlainText()).section(" ",1,1);
                }

                // 如果包名为空
                if (pkgname.isEmpty()) {
                    m.Reply(MessageChain().Plain("请输入包名！"));

                    return;
                }

                // 查询包
                qDebug()<<"Pkgname: "<< pkgname;
                CNPkgquery *query = new CNPkgquery(pkgname);
                PkgInfo ret = query->queryPkg();

                if(!ret.pkgname.isEmpty())
                {
                    // 生成消息体
                    QString messageBody;
                    messageBody.append("仓库    : ArchLinuxCN");
                    messageBody.append("\n包名    : " + ret.pkgname);
                    messageBody.append("\n版本    : " + ret.pkgver);
                    messageBody.append("\n维护    : " + ret.maintainers);

                    // 发送消息
                    m.Reply(MessageChain().Plain(messageBody.toStdString()));

                } else {
                    m.Reply(MessageChain().Plain(QString("未能找到 " + pkgname +" !").toStdString()));
                }

                return;
            }

            // Wiki
            if(qplain.startsWith("/wiki") || qplain.startsWith("!wiki"))
            {
                // 这句话不知道是干啥的
                groups[m.Sender.Group.GID] = true;

                QString prefix = qplain.at(0); // 获取前缀
                // 获取wiki检索 关键字
                QString keyword;
                if (qplain.count(' ') > 1) {
                    QString tStr = QString::fromStdString(m.MessageChain.GetPlainText());
                    tStr = tStr.trimmed();  // 去掉两端空格
                    tStr = tStr.split(prefix + "wiki").at(1); // 去掉头部
                    tStr = tStr.trimmed(); // 再次去掉两次空格

                    /*
                    if (tStr.contains(" ")) {
                        // 若tStr还有空格，则只取第一个空格前的数据
                        keyword = tStr.section(" ",0,0);
                    } else {
                        // 没有空格，tStr即为包名
                        keyword = tStr;
                    }
                    */

                    keyword = tStr.section(" ",0,-1);

                } else {
                    keyword = QString::fromStdString(m.MessageChain.GetPlainText()).section(" ",1,1);
                }

                // 如果Wiki检索关键字为空
                if (keyword.isEmpty()) {
                    m.Reply(MessageChain().Plain("请输入要检索的Wiki关键字！"));

                    return;
                }

                // 查询Wiki
                qDebug()<<"keyword: "<<keyword;
                WikiQuery *query = new WikiQuery(keyword);

                WikiResult ret = query->queryWiki();

                if(!ret.isEmpty())  // 有相关界面
                {
                    // 构建消息体
                    QString messageBody;
                    messageBody.append(ret.getTitle() + ":\n");
                    messageBody.append(ret.getUrl());
                    // 发送消息
                    m.Reply(MessageChain().Plain(messageBody.toStdString()));
                }
                else  //若没有界面
                {
                    // 发送消息
                    QString url = "https://wiki.archlinux.org/index.php?search=" + keyword; // 构造网址
                    m.Reply(MessageChain().Plain(url.toStdString()));
                }

                return;
            }
        }
        catch (const std::exception& ex)
        {
            std::cout<<ex.what()<<std::endl;
        }
    });


    bot.EventLoop();

    return a.exec();
}
