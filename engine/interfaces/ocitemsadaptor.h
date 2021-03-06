/*
 * This file was generated by qdbusxml2cpp version 0.7
 * Command line was: qdbusxml2cpp -c OcItemsAdaptor -a ocitemsadaptor.h:ocitemsadaptor.cpp de.buschmann23.ocNewsEngine.Items.xml
 *
 * qdbusxml2cpp is Copyright (C) 2013 Digia Plc and/or its subsidiary(-ies).
 *
 * This is an auto-generated file.
 * This file may have been hand-edited. Look for HAND-EDIT comments
 * before re-generating it.
 */

#ifndef OCITEMSADAPTOR_H
#define OCITEMSADAPTOR_H

#include <QtCore/QObject>
#include <QtDBus/QtDBus>
class QByteArray;
template<class T> class QList;
template<class Key, class Value> class QMap;
class QString;
class QStringList;
class QVariant;

/*
 * Adaptor class for interface de.buschmann23.ocNewsEngine.Items
 */
class OcItemsAdaptor: public QDBusAbstractAdaptor
{
    Q_OBJECT
    Q_CLASSINFO("D-Bus Interface", "de.buschmann23.ocNewsEngine.Items")
    Q_CLASSINFO("D-Bus Introspection", ""
"  <interface name=\"de.buschmann23.ocNewsEngine.Items\">\n"
"    <method name=\"requestItems\">\n"
"      <arg direction=\"in\" type=\"s\" name=\"batchSize\"/>\n"
"      <arg direction=\"in\" type=\"s\" name=\"offset\"/>\n"
"      <arg direction=\"in\" type=\"s\" name=\"type\"/>\n"
"      <arg direction=\"in\" type=\"s\" name=\"id\"/>\n"
"      <arg direction=\"in\" type=\"s\" name=\"getRead\"/>\n"
"    </method>\n"
"    <method name=\"updateItems\">\n"
"      <arg direction=\"in\" type=\"s\" name=\"lastModified\"/>\n"
"      <arg direction=\"in\" type=\"s\" name=\"type\"/>\n"
"      <arg direction=\"in\" type=\"s\" name=\"id\"/>\n"
"    </method>\n"
"    <method name=\"markItems\">\n"
"      <arg direction=\"in\" type=\"s\" name=\"action\"/>\n"
"      <arg direction=\"in\" type=\"av\" name=\"ids\"/>\n"
"    </method>\n"
"    <method name=\"markItemsTillThis\">\n"
"      <arg direction=\"in\" type=\"s\" name=\"action\"/>\n"
"      <arg direction=\"in\" type=\"v\" name=\"pubDate\"/>\n"
"      <arg direction=\"in\" type=\"v\" name=\"feedId\"/>\n"
"    </method>\n"
"    <method name=\"starItems\">\n"
"      <annotation value=\"QVariantMap\" name=\"com.trolltech.QtDBus.QtTypeName.In1\"/>\n"
"      <arg direction=\"in\" type=\"s\" name=\"action\"/>\n"
"      <arg direction=\"in\" type=\"a{sv}\" name=\"itemIds\"/>\n"
"    </method>\n"
"    <method name=\"markAllItemsRead\"/>\n"
"    <signal name=\"requestedItemsError\">\n"
"      <arg direction=\"out\" type=\"s\" name=\"requestedItemsErrorString\"/>\n"
"    </signal>\n"
"    <signal name=\"requestedItemsSuccess\">\n"
"      <arg name=\"updated\" type=\"ai\" direction=\"out\"/>\n"
"      <arg name=\"newItems\" type=\"ai\" direction=\"out\"/>\n"
"      <arg name=\"deleted\" type=\"ai\" direction=\"out\"/>\n"
"    </signal>\n"
"    <signal name=\"updatedItemsError\">\n"
"      <arg direction=\"out\" type=\"s\" name=\"updateItemsErrorString\"/>\n"
"    </signal>\n"
"    <signal name=\"updatedItemsSuccess\">\n"
"      <arg name=\"updated\" type=\"ai\" direction=\"out\"/>\n"
"      <arg name=\"newItems\" type=\"ai\" direction=\"out\"/>\n"
"      <arg name=\"deleted\" type=\"ai\" direction=\"out\"/>\n"
"    </signal>\n"
"    <signal name=\"markedItemsError\">\n"
"      <arg direction=\"out\" type=\"s\" name=\"markedItemsErrorString\"/>\n"
"    </signal>\n"
"    <signal name=\"markedItemsSuccess\">\n"
"      <arg name=\"ids\" type=\"as\" direction=\"out\"/>\n"
"      <arg name=\"action\" type=\"s\" direction=\"out\"/>\n"
"    </signal>\n"
"    <signal name=\"markedAllItemsReadError\">\n"
"      <arg direction=\"out\" type=\"s\" name=\"markedAllItemsReadErrorString\"/>\n"
"    </signal>\n"
"    <signal name=\"markedAllItemsReadSuccess\"/>\n"
"    <signal name=\"starredItemsError\">\n"
"      <arg direction=\"out\" type=\"s\" name=\"starredItemsErrorString\"/>\n"
"    </signal>\n"
"    <signal name=\"starredItemsSuccess\">\n"
"      <arg name=\"ids\" type=\"as\" direction=\"out\"/>\n"
"      <arg name=\"action\" type=\"s\" direction=\"out\"/>\n"
"    </signal>\n"
"  </interface>\n"
        "")
public:
    OcItemsAdaptor(QObject *parent);
    virtual ~OcItemsAdaptor();

public: // PROPERTIES
public Q_SLOTS: // METHODS
    void markAllItemsRead();
    void markItems(const QString &action, const QVariantList &ids);
    void markItemsTillThis(const QString &action, const QDBusVariant &pubDate, const QDBusVariant &feedId);
    void requestItems(const QString &batchSize, const QString &offset, const QString &type, const QString &id, const QString &getRead);
    void starItems(const QString &action, const QVariantMap &itemIds);
    void updateItems(const QString &lastModified, const QString &type, const QString &id);
Q_SIGNALS: // SIGNALS
    void markedAllItemsReadError(const QString &markedAllItemsReadErrorString);
    void markedAllItemsReadSuccess();
    void markedItemsError(const QString &markedItemsErrorString);
    void markedItemsSuccess(const QStringList &ids, const QString &action);
    void requestedItemsError(const QString &requestedItemsErrorString);
    void requestedItemsSuccess(const QList<int> &updated, const QList<int> &newItems, const QList<int> &deleted);
    void starredItemsError(const QString &starredItemsErrorString);
    void starredItemsSuccess(const QStringList &hashes, const QString &action);
    void updatedItemsError(const QString &updateItemsErrorString);
    void updatedItemsSuccess(const QList<int> &updated, const QList<int> &newItems, const QList<int> &deleted);
};

#endif
