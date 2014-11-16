#ifndef OCUPDATER_H
#define OCUPDATER_H

#include <QObject>
#include <QSqlError>
#include "ocnetwork.h"
#include "occonfiguration.h"
#include "../common/globals.h"

#if defined(MEEGO_EDITION_HARMATTAN)
#include <TransferUI/Client>
#include <TransferUI/Transfer>
#include <QSystemAlignedTimer>
#include <QSystemNetworkInfo>
#include <QSystemBatteryInfo>
#else
#include <keepalive/backgroundactivity.h>
#endif

#if defined(MEEGO_EDITION_HARMATTAN)
QTM_USE_NAMESPACE
#endif



class OcUpdater : public QObject
{
    Q_OBJECT
public:
    explicit OcUpdater(QObject *parent = 0);

public Q_SLOTS: // METHODS
    void startUpdate();
    bool isUpdateRunning();

Q_SIGNALS: // SIGNALS
    void updateError(const QString &updateErrorMessage);
    void updateFinished();
    void updateStarted();

signals:
    void startRequestFeeds();
    void startRequestItems(const QString &batchSize, const QString &offset, const QString &type, const QString &id, const QString &getRead);
    void startUpdateItems(const QString &lastModified, const QString &type, const QString &id);
    
public slots:
    void handleNetAndConfChanges();
    void setUpdateInterval(const int &nInterval);
    void setUpdateBehavior(const int &nBehavior);

private slots:
    void startUpdateTimed();
    void startUpdatePrivate();
    void updateFeeds();
    void updateItems();
    void endUpdate();
    void errorInUpdate(QString errorMessage);

private:
    OcNetwork network;
    OcConfiguration config;
    bool updateRunning;
    int m_interval;
    int m_updateBehavior;
#if defined(MEEGO_EDITION_HARMATTAN)
    TransferUI::Client *transferClient;
    TransferUI::Transfer *transferItem;
    QSystemAlignedTimer *timer;
    QSystemNetworkInfo *networkInfo;
    QSystemBatteryInfo *batteryInfo;
#else
    BackgroundActivity *m_timer;
//    MNotificationGroup *notificationGroup;
//    MNotification *notification;
//    QNetworkInfo *networkInfo;
//    QBatteryInfo *batteryInfo;
#endif
};

#endif // OCUPDATER_H
