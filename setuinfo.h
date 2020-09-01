#ifndef SETUINFO_H
#define SETUINFO_H

#include <QtCore>

typedef struct Setuinfo{
    QString url;
    int code;
    int quota;
    int quota_min_ttl;
}SetuInfo;

#endif // SETUINFO_H
