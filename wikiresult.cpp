#include "wikiresult.h"

WikiResult::WikiResult()
    : m_title(), m_url()
{

}

WikiResult::WikiResult(const QString &title, const QString &url)
    : m_title(title), m_url(url)
{

}

void WikiResult::setTitle(const QString &title)
{
    m_title = title;
}

void WikiResult::setUrl(const QString &url)
{
    m_url = url;
}

const QString WikiResult::getTitle()
{
    return m_title;
}

const QString WikiResult::getUrl()
{
    return m_url;
}

void WikiResult::print()
{
    qDebug()<<"Title:"<<m_title;
    qDebug()<<"Url:"<<m_url;
}

bool WikiResult::isEmpty()
{
    if (m_title.isEmpty() || m_url.isEmpty())
        return true;
    return false;
}
