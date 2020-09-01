#ifndef PKGINFO_H
#define PKGINFO_H

#include <QtCore>

typedef struct pkginfo{
    QString pkgname = "";
    QString pkgver = "";
    QString pkgdesc = "";
    QString url = "";
    QString maintainers = "";
    QString last_update = "";
    QString outofdate = "";
    int numvotes = 0;
    bool isAUR = false;
}PkgInfo;

#endif // PKGINFO_H
