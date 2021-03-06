/*
 * This file was generated by qdbusxml2cpp version 0.7
 * Command line was: qdbusxml2cpp -c OcConfigAdaptor -a occonfigadaptor.h:occonfigadaptor.cpp de.buschmann23.ocNewsEngine.Configuration.xml
 *
 * qdbusxml2cpp is Copyright (C) 2013 Digia Plc and/or its subsidiary(-ies).
 *
 * This is an auto-generated file.
 * This file may have been hand-edited. Look for HAND-EDIT comments
 * before re-generating it.
 */

#ifndef OCCONFIGADAPTOR_H
#define OCCONFIGADAPTOR_H

#include <QtCore/QObject>
#include <QtDBus/QtDBus>
class QByteArray;
template<class T> class QList;
template<class Key, class Value> class QMap;
class QString;
class QStringList;
class QVariant;

/*
 * Adaptor class for interface de.buschmann23.ocNewsEngine.Configuration
 */
class OcConfigAdaptor: public QDBusAbstractAdaptor
{
    Q_OBJECT
    Q_CLASSINFO("D-Bus Interface", "de.buschmann23.ocNewsEngine.Configuration")
    Q_CLASSINFO("D-Bus Introspection", ""
"  <interface name=\"de.buschmann23.ocNewsEngine.Configuration\">\n"
"    <method name=\"setSetting\">\n"
"      <arg direction=\"in\" type=\"s\" name=\"entry\"/>\n"
"      <arg direction=\"in\" type=\"v\" name=\"value\"/>\n"
"    </method>\n"
"    <method name=\"getSetting\">\n"
"      <arg direction=\"in\" type=\"s\" name=\"entry\"/>\n"
"      <arg direction=\"in\" type=\"v\" name=\"defaultValue\"/>\n"
"      <arg direction=\"out\" type=\"v\" name=\"result\"/>\n"
"    </method>\n"
"    <method name=\"getConfig\">\n"
"      <annotation value=\"QVariantMap\" name=\"com.trolltech.QtDBus.QtTypeName.Out0\"/>\n"
"      <arg direction=\"out\" type=\"a{sv}\" name=\"config\"/>\n"
"    </method>\n"
"    <method name=\"resetConfig\"/>\n"
"    <signal name=\"gotConfig\">\n"
"      <annotation value=\"QVariantMap\" name=\"com.trolltech.QtDBus.QtTypeName.In0\"/>\n"
"      <arg direction=\"out\" type=\"a{sv}\" name=\"config\"/>\n"
"    </signal>\n"
"    <method name=\"cleanDatabase\"/>\n"
"    <signal name=\"cleanedDatabase\"/>\n"
"    <method name=\"getStatistics\">\n"
"      <annotation value=\"QVariantMap\" name=\"com.trolltech.QtDBus.QtTypeName.Out0\"/>\n"
"      <arg direction=\"out\" type=\"a{sv}\" name=\"stats\"/>\n"
"    </method>\n"
"    <method name=\"getStat\">\n"
"      <arg direction=\"in\" type=\"i\" name=\"stat\"/>\n"
"      <arg direction=\"out\" type=\"v\" name=\"result\"/>\n"
"    </method>\n"
"    <signal name=\"gotStatistics\">\n"
"      <annotation value=\"QVariantMap\" name=\"com.trolltech.QtDBus.QtTypeName.In0\"/>\n"
"      <arg direction=\"out\" type=\"a{sv}\" name=\"stats\"/>\n"
"    </signal>\n"
"    <method name=\"cleanCertificates\"/>\n"
"    <signal name=\"cleanedCertificates\"/>\n"
"    <method name=\"isConfigSet\">\n"
"      <arg direction=\"out\" type=\"b\" name=\"configStatus\"/>\n"
"    </method>\n"
"    <method name=\"isAccountEnabled\">\n"
"      <arg direction=\"out\" type=\"b\" name=\"accountState\"/>\n"
"    </method>\n"
"    <signal name=\"changedConfig\"/>\n"
"    <signal name=\"configReset\" />\n"
"    <method name=\"removeAllAccounts\" />\n"
"    <signal name=\"removedAllAccounts\" />\n"
"  </interface>\n"
        "")
public:
    OcConfigAdaptor(QObject *parent);
    virtual ~OcConfigAdaptor();

public: // PROPERTIES
public Q_SLOTS: // METHODS
    void cleanCertificates();
    void cleanDatabase();
    QVariantMap getConfig();
    QDBusVariant getSetting(const QString &entry, const QDBusVariant &defaultValue);
    QVariantMap getStatistics();
    QDBusVariant getStat(const int stat);
    bool isAccountEnabled();
    bool isConfigSet();
    void resetConfig();
    void setSetting(const QString &entry, const QDBusVariant &value);
    void removeAllAccounts();
Q_SIGNALS: // SIGNALS
    void changedConfig();
    void cleanedCertificates();
    void cleanedDatabase();
    void gotConfig(const QVariantMap &config);
    void gotStatistics(const QVariantMap &stats);
    void configReset();
    void removedAllAccounts();
};

#endif
