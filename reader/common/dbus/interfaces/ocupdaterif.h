/*
 * This file was generated by qdbusxml2cpp version 0.7
 * Command line was: qdbusxml2cpp -v -c OcUpdaterIf -p ocupdaterif.h:ocupdaterif.cpp ../../ocNewsEngine/interfaces/de.buschmann23.ocNewsEngine.Updater.xml
 *
 * qdbusxml2cpp is Copyright (C) 2013 Digia Plc and/or its subsidiary(-ies).
 *
 * This is an auto-generated file.
 * Do not edit! All changes made to it will be lost.
 */

#ifndef OCUPDATERIF_H
#define OCUPDATERIF_H

#include <QtCore/QObject>
#include <QtCore/QByteArray>
#include <QtCore/QList>
#include <QtCore/QMap>
#include <QtCore/QString>
#include <QtCore/QStringList>
#include <QtCore/QVariant>
#include <QtDBus/QtDBus>

/*
 * Proxy class for interface de.buschmann23.ocNewsEngine.Updater
 */
class OcUpdaterIf: public QDBusAbstractInterface
{
    Q_OBJECT
public:
    static inline const char *staticInterfaceName()
    { return "de.buschmann23.ocNewsEngine.Updater"; }

public:
    OcUpdaterIf(const QString &service, const QString &path, const QDBusConnection &connection, QObject *parent = 0);

    ~OcUpdaterIf();

public Q_SLOTS: // METHODS
    inline QDBusPendingReply<bool> isUpdateRunning()
    {
        QList<QVariant> argumentList;
        return asyncCallWithArgumentList(QLatin1String("isUpdateRunning"), argumentList);
    }

    inline QDBusPendingReply<> startUpdate()
    {
        QList<QVariant> argumentList;
        return asyncCallWithArgumentList(QLatin1String("startUpdate"), argumentList);
    }

Q_SIGNALS: // SIGNALS
    void updateError(const QString &updateErrorMessage);
    void updateFinished();
    void updateStarted();
};

namespace de {
  namespace buschmann23 {
    namespace ocNewsEngine {
      typedef ::OcUpdaterIf Updater;
    }
  }
}
#endif
