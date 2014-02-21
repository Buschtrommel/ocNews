#include "ocsingleitemmodelsql.h"
#include "../../../common/globals.h"
#include <QDebug>

OcSingleItemModelSql::OcSingleItemModelSql(QObject *parent) :
    QObject(parent)
{
}


QVariantMap OcSingleItemModelSql::getItemData(const QString &itemId, int handleRead, bool sortAsc, const QString &searchString, const QString &feedType, const QString &feedId, bool showImg)
{
    QVariantMap itemresult;
    QSqlQuery query;
//    query.exec(QString("SELECT it.id, it.guidHash, it.url, it.title, it.author, it.pubDate, it.body, it.enclosureMime, it.enclosureLink, it.unread, it.starred, it.feedId, "
//                       "(SELECT title FROM feeds WHERE id = it.feedId) AS feedName, "
//                       "IFNULL((SELECT id FROM items WHERE pubDate > it.pubDate AND feedId = it.feedId ORDER BY pubDate ASC LIMIT 1),0) AS previous, "
//                       "IFNULL((SELECT id FROM items WHERE pubDate < it.pubDate AND feedId = it.feedId ORDER BY pubDate DESC LIMIT 1),0) AS next "
//                       "FROM items it WHERE id = %1").arg(itemId.toInt()));


    QString queryString = "SELECT it.id, it.guidHash, it.url, it.title, it.author, it.pubDate, it.body, it.enclosureMime, it.enclosureLink, it.unread, it.starred, it.feedId, "
                          "(SELECT title FROM feeds WHERE id = it.feedId) AS feedName, ";


    // get previous item based on sorting options

    queryString.append("IFNULL((SELECT id FROM items ");

    if (!sortAsc) {
        queryString.append("WHERE pubDate > it.pubDate ");
    } else {
        queryString.append("WHERE pubDate < it.pubDate ");
    }

    if (feedType == "0") {
        queryString.append(QString("AND feedId = %1 ").arg(feedId.toInt()));
    } else if (feedType == "folder") {
        queryString.append(QString("AND feedId IN (SELECT id FROM feeds WHERE folderId = %1) ").arg(feedId.toInt()));
    } else if (feedType == "starred") {
        queryString.append(QString("AND starred = %1 ").arg(SQL_TRUE));
    }

    if (handleRead == 1)
        queryString.append(QString("AND unread = %1 ").arg(SQL_TRUE));

    if (!searchString.isEmpty())
    {
        QString t_search = searchString;
        t_search.prepend("%");
        t_search.append("%");
        queryString.append(QString(" AND title LIKE \"%1\"").arg(t_search));
    }


    queryString.append("ORDER BY ");

    if (handleRead == 2)
        queryString.append("unread DESC, ");

    if (!sortAsc) {
        queryString.append("pubDate ASC ");
    } else {
        queryString.append("pubDate DESC ");
    }

    queryString.append("LIMIT 1),0) AS previous, ");



    // get next item based on sorting options

    queryString.append("IFNULL((SELECT id FROM items ");

    if (!sortAsc) {
        queryString.append("WHERE pubDate < it.pubDate ");
    } else {
        queryString.append("WHERE pubDate > it.pubDate ");
    }

    if (feedType == "0") {
        queryString.append(QString("AND feedId = %1 ").arg(feedId.toInt()));
    } else if (feedType == "folder") {
        queryString.append(QString("AND feedId IN (SELECT id FROM feeds WHERE folderId = %1) ").arg(feedId.toInt()));
    } else if (feedType == "starred") {
        queryString.append(QString("AND starred = %1 ").arg(SQL_TRUE));
    }

    if (handleRead == 1)
        queryString.append(QString("AND unread = %1 ").arg(SQL_TRUE));

    if (!searchString.isEmpty())
    {
        QString t_search = searchString;
        t_search.prepend("%");
        t_search.append("%");
        queryString.append(QString(" AND title LIKE \"%1\"").arg(t_search));
    }


    queryString.append("ORDER BY ");

    if (handleRead == 2)
        queryString.append("unread DESC, ");

    if (!sortAsc) {
        queryString.append("pubDate DESC ");
    } else {
        queryString.append("pubDate ASC ");
    }

    queryString.append(" LIMIT 1),0) AS next ");

    queryString.append(QString("FROM items it WHERE id = %1").arg(itemId.toInt()));

    query.exec(queryString);


    qDebug() << "Item ID: " << itemId;
    qDebug() << "Show Img: " << showImg;
    qDebug() << "Handle read: " << handleRead;
    qDebug() << "Sort ascending: " << sortAsc;
    qDebug() << "Search string: " << searchString;
    qDebug() << "Feed type: " << feedType;
    qDebug() << "Feed id: " << feedId;


    if (query.next())
    {
        itemresult["id"] = query.value(0).toInt();
        itemresult["guidHash"] = query.value(1).toString();
        itemresult["url"] = query.value(2).toString();
        itemresult["title"] = query.value(3).toString();
        itemresult["author"] = query.value(4).toString();
        QString date = (QDateTime::currentDateTimeUtc().date() == QDateTime::fromTime_t(query.value(5).toUInt()).date()) ?
                        tr("Today") :
                        QDateTime::fromTime_t(query.value(5).toUInt()).toLocalTime().toString(tr("d. MMMM"));
        QString time = QDateTime::fromTime_t(query.value(5).toUInt()).toLocalTime().toString(tr("hh:mm"));
        itemresult["pubDate"] = date + " | " + time;
        itemresult["body"] = cleanHTML(query.value(6).toString(), showImg);
        itemresult["enclosureMime"] = query.value(7).toString();
        itemresult["enclosureLink"] = query.value(8).toString();
        itemresult["unread"] = query.value(9).toBool();
        itemresult["starred"] = query.value(10).toBool();
        itemresult["feedId"] = query.value(11).toString();
        itemresult["feedName"] = query.value(12).toString();
        itemresult["previous"] = query.value(13).toString();
        itemresult["next"] = query.value(14).toString();
        itemresult["containsImg"] = showImg ? false : query.value(6).toString().contains(QRegExp("<img[^>]*>"));
        if (itemresult["enclosureLink"] == "") {
            itemresult["enclosureType"] = 0;
            itemresult["enclosureHost"] = "";
            itemresult["enclosureName"] = "";
        } else {
            itemresult["enclosureType"] = getEnclosureType(query.value(7).toString());
            itemresult["enclosureHost"] = QUrl(query.value(8).toString()).host();
            itemresult["enclosureName"] = QFileInfo(query.value(8).toString()).fileName();
        }
    }

    return itemresult;
}


int OcSingleItemModelSql::getEnclosureType(const QString &encMime)
{

    if (encMime.contains("audio", Qt::CaseInsensitive)) {
        return 1;
    } else if (encMime.contains("video", Qt::CaseInsensitive)) {
        return 2;
    } else if (encMime.contains("pdf", Qt::CaseInsensitive)) {
        return 3;
    } else if (encMime.contains("image", Qt::CaseInsensitive)) {
        return 4;
    } else {
        return 0;
    }
}


QString OcSingleItemModelSql::cleanHTML(const QString &html, bool showImg)
{
    QString m_html = html;

    if (!showImg) {
        m_html.remove(QRegExp("<img[^>]*>"));
        m_html.remove(QRegExp("<a[^>]*>\\s*</a>"));
    }

    m_html.remove(QRegExp("class=\"[\\w\\s-]*\"")); // remove class statements

//    if (showImg)
//        m_html.remove(QRegExp("alt=\"[\\w\\s-\\.\\?=:~;&]*\""));

//    m_html.remove(QRegExp("align=\"\\w*\""));

//    m_html.remove(QRegExp("target=\"\\w*\""));

//    m_html.remove(QRegExp("text-\\w*:[\\s\\w]*;?"));

//    m_html.remove(QRegExp("clear:[\\s\\w]*;?"));

//    m_html.remove(QRegExp("margin-\\w*:[\\s\\w]*;?"));

//    m_html.remove(QRegExp("style=\"\\s*\""));

//    m_html.replace(QRegExp("\\s+>"), ">");

//    m_html.replace(QRegExp("\\s{2,}"), " ");

//    m_html.remove(QRegExp("<table.*</table>"));

    return m_html;
}
