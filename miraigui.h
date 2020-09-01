#ifndef MIRAIGUI_H
#define MIRAIGUI_H

#include <QWidget>

// Debug
#include <QDebug>

// Mirai
#include <mirai.h>

#include "pkgquery.h"
#include "pkginfo.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MiraiGUI; }
QT_END_NAMESPACE

using namespace Cyan;

class MiraiGUI : public QWidget
{
    Q_OBJECT
public:
    MiraiGUI(QWidget *parent = nullptr);
    ~MiraiGUI();

private:
    Ui::MiraiGUI *ui;
signals:

};

#endif // MIRAIGUI_H
