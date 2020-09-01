#include <QCoreApplication>
#include <mirai.h>

#include "pkginfo.h"
#include "pkgquery.h"
#include "mnerinfo.h"
#include "mnerquery.h"
#include "setuinfo.h"
#include "setuquery.h"
#include "whitelist.h"

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
                messageBody.append("/ping    检查机器人是否在线\n");
                messageBody.append("/pkg xxx    查询包xxx\n");
                messageBody.append("/off xxx    在官方仓库中查询包xxx\n");
                messageBody.append("/aur xxx   在AUR中查询包xxx\n");
                messageBody.append("/mner xxx    查询打包人信息xxx\n");
                messageBody.append("/enable lsp    开启色图机器人\n");
                messageBody.append("/disable lsp    关闭色图机器人\n");
                messageBody.append("/lsp     我要一份色图！\n");
                messageBody.append("/lsp xxx    我要一份xxx的色图！");

                // 发送消息
                m.Reply(MessageChain().Plain(messageBody.toStdString()));
            }

            // ping
            if(plain == "/ping" || plain =="!ping")
            {
                // 这句话不知道是干啥的
                groups[m.Sender.Group.GID] = true;
                m.Reply(MessageChain().Plain("小易吃粑粑！"));

                return;
            }

            // pkg
            if(qplain.contains("/pkg") || qplain.contains("!pkg"))
            {
                // 这句话不知道是干啥的
                groups[m.Sender.Group.GID] = true;

                // 查询包
                QString pkgname = QString::fromStdString(m.MessageChain.GetPlainText()).section(" ",1,1);  //获取包名
                qDebug()<<"Pkgname: "<< pkgname;
                PkgQuery *query = new PkgQuery(pkgname);
                PkgInfo ret = query->queryPkg();

                if(!ret.pkgname.isEmpty())
                {
                    if(!ret.isAUR)  // 官方仓库
                    {
                        // 生成消息体
                        QString messageBody;
                        messageBody.append("仓库    :官方");
                        messageBody.append("\n包名    :" + ret.pkgname);
                        messageBody.append("\n版本    :" + ret.pkgver);
                        messageBody.append("\n描述    :" + ret.pkgdesc);
                        messageBody.append("\n维护    :" + ret.maintainers);
                        messageBody.append("\n更新    :" + ret.last_update);
                        messageBody.append("\n网址    :" + ret.url);

                        // 发送消息
                        m.Reply(MessageChain().Plain(messageBody.toStdString()));
                    }
                    else  // AUR仓库
                    {
                        // 生成消息体
                        QString messageBody;
                        messageBody.append("仓库    :AUR");
                        messageBody.append("\n包名    :" + ret.pkgname);
                        messageBody.append("\n版本    :" + ret.pkgver);
                        messageBody.append("\n描述    :" + ret.pkgdesc);
                        messageBody.append("\n维护    :" + ret.maintainers);
                        messageBody.append("\n投票    :" + QString::number(ret.numvotes));
                        if(ret.outofdate.isEmpty())
                            messageBody.append("\n过期    :未过期");
                        else
                            messageBody.append("\n过期    :" + ret.outofdate);
                        messageBody.append("\n地址    :" + ret.url);

                        // 发送消息
                        m.Reply(MessageChain().Plain(messageBody.toStdString()));
                    }
                }

                return;
            }

            // aur
            if(qplain.contains("/aur") || qplain.contains("!aur"))
            {
                // 这句话不知道是干啥的
                groups[m.Sender.Group.GID] = true;

                // 查询包
                QString pkgname = QString::fromStdString(m.MessageChain.GetPlainText()).section(" ",1,1);  //获取包名
                qDebug()<<"Pkgname: "<< pkgname;
                PkgQuery *query = new PkgQuery(pkgname);
                PkgInfo ret = query->queryPkgInAUR();

                if(!ret.pkgname.isEmpty())
                {

                    // 生成消息体
                    QString messageBody;
                    messageBody.append("仓库    :AUR");
                    messageBody.append("\n包名    :" + ret.pkgname);
                    messageBody.append("\n版本    :" + ret.pkgver);
                    messageBody.append("\n描述    :" + ret.pkgdesc);
                    messageBody.append("\n维护    :" + ret.maintainers);
                    messageBody.append("\n投票    :" + QString::number(ret.numvotes));
                    if(ret.outofdate.isEmpty())
                        messageBody.append("\n过期    :未过期");
                    else
                        messageBody.append("\n过期    :" + ret.outofdate);
                    messageBody.append("\n地址    :" + ret.url);

                    // 发送消息
                    m.Reply(MessageChain().Plain(messageBody.toStdString()));
                }

                return;
            }

            // off
            if(qplain.contains("/off") || qplain.contains("!off"))
            {
                // 这句话不知道是干啥的
                groups[m.Sender.Group.GID] = true;

                // 查询包
                QString pkgname = QString::fromStdString(m.MessageChain.GetPlainText()).section(" ",1,1);  //获取包名
                qDebug()<<"Pkgname: "<< pkgname;
                PkgQuery *query = new PkgQuery(pkgname);
                PkgInfo ret = query->queryPkgInOfficial();

                if(!ret.pkgname.isEmpty())
                {
                    // 生成消息体
                    QString messageBody;
                    messageBody.append("仓库    :官方");
                    messageBody.append("\n包名    :" + ret.pkgname);
                    messageBody.append("\n版本    :" + ret.pkgver);
                    messageBody.append("\n描述    :" + ret.pkgdesc);
                    messageBody.append("\n维护    :" + ret.maintainers);
                    messageBody.append("\n更新    :" + ret.last_update);
                    messageBody.append("\n网址    :" + ret.url);

                    // 发送消息
                    m.Reply(MessageChain().Plain(messageBody.toStdString()));

                }

                return;
            }

            // mner
            if(qplain.contains("/mner") || qplain.contains("!mner"))
            {
                // 这句话不知道是干啥的
                groups[m.Sender.Group.GID] = true;

                // 打包人姓名
                QString mnerName = qplain.section(" ",1,1);
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
            if(qplain.contains("/lsp") || qplain.contains("!lsp"))
            {
                // 开启色图开关才能获取色图
                if(setu)
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
        }
        catch (const std::exception& ex)
        {
            std::cout<<ex.what()<<std::endl;
        }
    });


    bot.EventLoop();

    return a.exec();
}
