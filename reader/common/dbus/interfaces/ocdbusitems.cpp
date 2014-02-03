#include "ocdbusitems.h"

OcDBusItems::OcDBusItems(QObject *parent) :
    QObject(parent)
{
    items = new de::buschmann23::ocNewsEngine::Items("de.buschmann23.ocNewsEngine", "/Items", QDBusConnection::sessionBus(), this);
    connect(items, SIGNAL(markedAllItemsReadError(QString)), this, SLOT(dbusMarkedAllItemsReadError(QString)));
    connect(items, SIGNAL(markedAllItemsReadSuccess()), this, SLOT(dbusMarkedAllItemsReadSuccess()));
    connect(items, SIGNAL(markedItemsError(QString)), this, SLOT(dbusMarkedItemsError(QString)));
    connect(items, SIGNAL(markedItemsSuccess()), this, SLOT(dbusMarkedItemsSuccess()));
    connect(items, SIGNAL(requestedItemsError(QString)), this, SLOT(dbusRequestedItemsError(QString)));
    connect(items, SIGNAL(requestedItemsSuccess()), this, SLOT(dbusRequestedItemsSuccess()));
    connect(items, SIGNAL(starredItemsError(QString)), this, SLOT(dbusStarredItemsError(QString)));
    connect(items, SIGNAL(starredItemsSuccess()), this, SLOT(dbusStarredItemsSuccess()));
    connect(items, SIGNAL(updatedItemsError(QString)), this, SLOT(dbusUpdatedItemsError(QString)));
    connect(items, SIGNAL(updatedItemsSuccess()), this, SLOT(dbusUpdatedItemsSuccess()));
    connect(items, SIGNAL(startedFetchingImages(int)), this, SLOT(dbusStartedFetchingImages(int)));
    connect(items, SIGNAL(finishedFetchingImages()), this, SLOT(dbusFinishedFetchingImages()));
    connect(items, SIGNAL(fetchingImages(int)), this, SLOT(dbusFetchingImages(int)));
}

void OcDBusItems::markAllItemsRead()
{
    items->markAllItemsRead();
}

void OcDBusItems::markItems(const QString &action, const QVariantList &ids)
{
    items->markItems(action, ids);
}

void OcDBusItems::markItemsTillThis(const QString &action, const QVariant &pubDate, const QVariant &feedId)
{
#ifdef QT_DEBUG
        qDebug() << "DBus markItemsTillThis";
#endif
    QDBusVariant t_pubDate(pubDate);
    QDBusVariant t_feedId(feedId);
    items->markItemsTillThis(action, t_pubDate, t_feedId);
}

void OcDBusItems::requestItems(const QString &batchSize, const QString &offset, const QString &type, const QString &id, const QString &getRead)
{
    items->requestItems(batchSize, offset, type, id, getRead);
}

void OcDBusItems::starItems(const QString &action, const QVariantMap &itemIds)
{
    items->starItems(action, itemIds);
}

void OcDBusItems::updateItems(const QString &lastModified, const QString &type, const QString &id)
{
    items->updateItems(lastModified, type, id);
}

int OcDBusItems::isFetchImagesRunning()
{
    return items->isFetchImagesRunning();
}

void OcDBusItems::dbusMarkedAllItemsReadError(const QString &markedAllItemsReadErrorString)
{
    emit markedAllItemsReadError(markedAllItemsReadErrorString);
}

void OcDBusItems::dbusMarkedAllItemsReadSuccess()
{
    emit markedAllItemsReadSuccess();
}

void OcDBusItems::dbusMarkedItemsError(const QString &markedItemsErrorString)
{
    emit markedItemsError(markedItemsErrorString);
}

void OcDBusItems::dbusMarkedItemsSuccess()
{
    emit markedItemsSuccess();
}

void OcDBusItems::dbusRequestedItemsError(const QString &requestedItemsErrorString)
{
    emit requestedItemsError(requestedItemsErrorString);
}

void OcDBusItems::dbusRequestedItemsSuccess()
{
    emit requestedItemsSuccess();
}

void OcDBusItems::dbusStarredItemsError(const QString &starredItemsErrorString)
{
    emit starredItemsError(starredItemsErrorString);
}

void OcDBusItems::dbusStarredItemsSuccess()
{
    emit starredItemsSuccess();
}

void OcDBusItems::dbusUpdatedItemsError(const QString &updateItemsErrorString)
{
    emit updatedItemsError(updateItemsErrorString);
}

void OcDBusItems::dbusUpdatedItemsSuccess()
{
    emit updatedItemsSuccess();
}

void OcDBusItems::dbusStartedFetchingImages(const int &numberOfItems)
{
    emit startedFetchingImages(numberOfItems);
}

void OcDBusItems::dbusFinishedFetchingImages()
{
    emit finishedFetchingImages();
}

void OcDBusItems::dbusFetchingImages(const int &currentItem)
{
    emit fetchingImages(currentItem);
}
