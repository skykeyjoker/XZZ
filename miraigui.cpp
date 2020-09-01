#include "miraigui.h"

MiraiGUI::MiraiGUI(QWidget *parent) : QWidget(parent)
{
    MiraiBot bot("127.0.0.1",8080); //初始化一个机器人
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
                query->queryPkg();

                return;
            }
        }
        catch (const std::exception& ex)
        {
            std::cout<<ex.what()<<std::endl;
        }
    });


    bot.EventLoop();
}

MiraiGUI::~MiraiGUI()
{

}
