#include <QDebug>
#include "ocfeedsmodelsql.h"
#include "../../../common/globals.h"

const char* OcFeedsModelSql::COLUMN_NAMES[] = {
    "title",
    "id",
    "iconSource",
    "iconWidth",
    "iconHeight",
    "unreadCount",
    "type",
    NULL
};

OcFeedsModelSql::OcFeedsModelSql(QObject *parent) :
    QSqlQueryModel(parent)
{

#if defined(MEEGO_EDITION_HARMATTAN)
    int idx = 0;
    QHash<int, QByteArray> roles;
    while(COLUMN_NAMES[idx]) {
        roles[Qt::UserRole + idx + 1] = COLUMN_NAMES[idx];
        idx++;
    }
    setRoleNames(roles);
#endif

}


#if !defined(MEEGO_EDITION_HARMATTAN)
QHash<int,QByteArray> OcFeedsModelSql::roleNames() const
{
    int idx = 0;
    QHash<int, QByteArray> roles;
    while (COLUMN_NAMES[idx]) {
        roles[Qt::UserRole + idx + 1] = COLUMN_NAMES[idx];
        idx++;
    }
    return roles;
}
#endif



QVariant OcFeedsModelSql::data(const QModelIndex &index, int role) const
{
    QVariant value = QSqlQueryModel::data(index, role);
    if(role < Qt::UserRole)
    {
        value = QSqlQueryModel::data(index, role);
    } else {
        int columnIdx = role - Qt::UserRole - 1;
        QModelIndex modelIndex = this->index(index.row(), columnIdx);
        value = QSqlQueryModel::data(modelIndex, Qt::DisplayRole);
        if (columnIdx == 2 && value != "") {
            QString iconSrcPath = QDir::homePath() + BASE_PATH + "/favicons/" + value.toString();
            value = iconSrcPath;
        }
    }
    return value;
}


void OcFeedsModelSql::refresh(const QString &folderId)
{
#ifdef QT_DEBUG
    qDebug() << "Refresh Feeds Model";
#endif

    bool hideReadFeeds = dbus.getSetting("display/hidereadfeeds", false).toBool();

    QString querystring(QString("SELECT title, id, iconSource, iconWidth, iconHeight, localUnreadCount AS unreadCount, '0' AS type FROM feeds WHERE folderId = %1 ").arg(folderId.toInt()));

    if (hideReadFeeds)
        querystring.append("AND localUnreadCount > 0 ");

    querystring.append("UNION ");

    querystring.append(QString("SELECT '%2' AS title, '0' AS id, '' AS iconSource, '' AS iconWidth, '' AS iconHeight, (SELECT SUM(localUnreadCount) FROM feeds WHERE folderId = %1) AS unreadCount, '-1' AS type "
                        ).arg(folderId.toInt()).arg(tr("All posts")));

    QString itemOrder = dbus.getSetting("display/orderby", "id").toString();

    if (itemOrder == "id") {
        querystring.append("ORDER BY type ASC, id ASC;");
    } else if (itemOrder == "title") {
        querystring.append("ORDER BY type ASC, title COLLATE NOCASE ASC;");
    } else {
        querystring.append("ORDER BY type ASC, unreadCount DESC");
    }

    this->setQuery(querystring);
}
