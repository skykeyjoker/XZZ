#ifndef WIKIRESULT_H
#define WIKIRESULT_H

#include <QtCore>

class WikiResult
{
public:
    explicit WikiResult();
    explicit WikiResult(const QString& title, const QString& url);

    void setTitle(const QString& title);
    void setUrl(const QString& url);

    const QString getTitle();
    const QString getUrl();

    void print();
    bool isEmpty();

private:
    QString m_title;
    QString m_url;
};

#endif // WIKIRESULT_H
