/*
 * This file was generated by qdbusxml2cpp version 0.7
 * Command line was: qdbusxml2cpp -v -c OcFoldersIf -p ocfoldersif.h:ocfoldersif.cpp ../../ocNewsEngine/interfaces/de.buschmann23.ocNewsEngine.Folders.xml
 *
 * qdbusxml2cpp is Copyright (C) 2013 Digia Plc and/or its subsidiary(-ies).
 *
 * This is an auto-generated file.
 * Do not edit! All changes made to it will be lost.
 */

#ifndef OCFOLDERSIF_H
#define OCFOLDERSIF_H

#include <QtCore/QObject>
#include <QtCore/QByteArray>
#include <QtCore/QList>
#include <QtCore/QMap>
#include <QtCore/QString>
#include <QtCore/QStringList>
#include <QtCore/QVariant>
#include <QtDBus/QtDBus>

/*
 * Proxy class for interface de.buschmann23.ocNewsEngine.Folders
 */
class OcFoldersIf: public QDBusAbstractInterface
{
    Q_OBJECT
public:
    static inline const char *staticInterfaceName()
    { return "de.buschmann23.ocNewsEngine.Folders"; }

public:
    OcFoldersIf(const QString &service, const QString &path, const QDBusConnection &connection, QObject *parent = 0);

    ~OcFoldersIf();

public Q_SLOTS: // METHODS
    inline QDBusPendingReply<> createFolder(const QString &name)
    {
        QList<QVariant> argumentList;
        argumentList << QVariant::fromValue(name);
        return asyncCallWithArgumentList(QLatin1String("createFolder"), argumentList);
    }

    inline QDBusPendingReply<> deleteFolder(const QString &id)
    {
        QList<QVariant> argumentList;
        argumentList << QVariant::fromValue(id);
        return asyncCallWithArgumentList(QLatin1String("deleteFolder"), argumentList);
    }

    inline QDBusPendingReply<QVariantMap> getFolders()
    {
        QList<QVariant> argumentList;
        return asyncCallWithArgumentList(QLatin1String("getFolders"), argumentList);
    }

    inline QDBusPendingReply<> markFolderRead(const QString &id)
    {
        QList<QVariant> argumentList;
        argumentList << QVariant::fromValue(id);
        return asyncCallWithArgumentList(QLatin1String("markFolderRead"), argumentList);
    }

    inline QDBusPendingReply<> renameFolder(const QString &id, const QString &name)
    {
        QList<QVariant> argumentList;
        argumentList << QVariant::fromValue(id) << QVariant::fromValue(name);
        return asyncCallWithArgumentList(QLatin1String("renameFolder"), argumentList);
    }

    inline QDBusPendingReply<> requestFolders()
    {
        QList<QVariant> argumentList;
        return asyncCallWithArgumentList(QLatin1String("requestFolders"), argumentList);
    }

Q_SIGNALS: // SIGNALS
    void createdFolderError(const QString &createresulterror);
    void createdFolderSuccess(const QString &foldername, const int &folderId);
    void deletedFolderError(const QString &deleteresulterror);
    void deletedFolderSuccess(const int &folderId);
    void markedReadFolderError(const QString &markedreaderror);
    void markedReadFolderSuccess(const int &folderId);
    void renamedFolderError(const QString &renameresulterror);
    void renamedFolderSuccess(const QString &newfoldername, const int &folderId);
    void requestedFoldersError(const QString &requerstresulterror);
    void requestedFoldersSuccess(const QList<int> &updated, const QList<int> &newFolders, const QList<int> &deleted);
};

namespace de {
  namespace buschmann23 {
    namespace ocNewsEngine {
      typedef ::OcFoldersIf Folders;
    }
  }
}
#endif
