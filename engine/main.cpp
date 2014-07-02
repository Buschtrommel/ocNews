#include <QtCore/QCoreApplication>
#include <QSettings>
//#include <MRemoteAction>
#if defined(MEEGO_EDITION_HARMATTAN)
#include <aegis_certman.h>
#endif

#include "../common/globals.h"
#include "ocdbmanager.h"
#include "ocupgradehelper.h"
#include "occonfiguration.h"
#include "interfaces/occonfigadaptor.h"
#include "ocgeneric.h"
#include "interfaces/ocgenericadaptor.h"
#include "ocfolders.h"
#include "interfaces/ocfoldersadaptor.h"
#include "ocfeeds.h"
#include "interfaces/ocfeedsadaptor.h"
#include "ocitems.h"
#include "interfaces/ocitemsadaptor.h"
#include "ocupdater.h"
#include "interfaces/ocupdateradaptor.h"
#include "ocdownloadmanager.h"
#include "interfaces/ocdownloadsadaptor.h"
#include "ocimagefetcher.h"
#include "interfaces/ocimagefetcheradaptor.h"
#include "ocnotifications.h"
#if defined(MEEGO_EDITION_HARMATTAN)
#include "ocaccount.h"
#include "interfaces/ocaccountadaptor.h"
#endif


#if defined(MEEGO_EDITION_HARMATTAN)
// Callback function for aegis_certman_iterate_certs
QList<QSslCertificate> localCerts;
QStringList localCertsToDelete;
int addCert(int ordering, X509* cert, void* ctx)
{
    // convert internal X509 structure to DER
    int len;
    unsigned char *buf;

    buf = NULL;

    len = i2d_X509(cert, &buf);

    if (len > 0) {
        int guiCheck = aegis_certman_gui_check_certificate(cert, 120);

        if (guiCheck == 0) {
            QByteArray buffer(reinterpret_cast<const char*>(buf), len);
            QSslCertificate sslCert(buffer, QSsl::Der);
            localCerts.append(sslCert);
        } else {
            aegis_key_id crtKey;
            aegis_certman_get_key_id(cert, crtKey);

            char buffer[AEGIS_KEY_ID_STR_LEN];

            aegis_certman_key_id_to_str(crtKey, buffer, AEGIS_KEY_ID_STR_LEN);

            localCertsToDelete.append(buffer);
        }
    }
    return 0;
}
#endif

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);
    a.setOrganizationName("Buschtrommel");
    a.setOrganizationDomain("buschmann23.de");
    a.setApplicationName("ocNewsEngine");
    a.setApplicationVersion(VERSION_STRING);

    QString locale = QLocale::system().name();
    QTranslator translator;
    if ((translator.load("ocnewsengine_"+locale, L10N_PATH)))
        a.installTranslator(&translator);

    // set paths
    QString basePath(QDir::homePath().append(BASE_PATH));

#if !defined(MEEGO_EDITION_HARMATTAN)
    // move directories to to new pathes on SailfishOS
    if (QDir(QDir::homePath().append(OLD_BASE_PATH)).exists() && !QDir(basePath).exists())
    {
        qDebug() << "Moving Data Dir";
        QProcess::execute("/bin/sh /usr/share/harbour-ocnews-reader/moveDataDir.sh");
    }

    if (QDir(QDir::homePath().append(OLD_SETTINGS_PATH)).exists() && !QDir(QDir::homePath().append(SETTINGS_PATH)).exists())
    {
        qDebug() << "Moving Settings Dir";
        QProcess::execute("/bin/sh /usr/share/harbour-ocnews-reader/moveSettingsDir.sh");
    }
#endif

    QSettings::setPath(QSettings::NativeFormat, QSettings::UserScope, QDir::homePath().append(SETTINGS_PATH));

    // create storage dirs
    QDir().mkpath(basePath + "/favicons");
    QDir().mkpath(basePath + "/certs");
    QDir().mkpath(QDir::homePath() + IMAGE_CACHE);
    QDir().mkpath(QDir::homePath() + MEDIA_PATH_AUDIO);
    QDir().mkpath(QDir::homePath() + MEDIA_PATH_IMAGE);
    QDir().mkpath(QDir::homePath() + MEDIA_PATH_PDF);
    QDir().mkpath(QDir::homePath() + MEDIA_PATH_VIDEO);

#if defined(MEEGO_EDITION_HARMATTAN)
    // set credential for ssl domain
    int credSuc = aegis_certman_set_credentials("buschtrommel-ocnews::CertOCNewsSSL");
    if (credSuc != 0) qDebug() << "set credential error: " << credSuc;

    // open ssl domain
    domain_handle ownDomain;
    int openCheck = aegis_certman_open_domain("ssl-ocnews", AEGIS_CERTMAN_DOMAIN_PRIVATE, &ownDomain);
    if (openCheck != 0) qDebug() << "Error Opening Domain: " << openCheck;

    aegis_certman_iterate_certs(ownDomain, &addCert, NULL);

    // delete untrusted certs from private cert domain
    if (!localCertsToDelete.isEmpty())
    {
        for (int i = 0; i < localCertsToDelete.size(); ++i)
        {
            aegis_key_id crtKey;
            aegis_certman_str_to_key_id(localCertsToDelete.at(i).toLocal8Bit().data(),crtKey);
            int rmCertCheck = aegis_certman_rm_cert(ownDomain, crtKey);
            if (rmCertCheck != 0)
                qDebug() << "Error removing Certificate from private domain.";
        }
    }

    aegis_certman_close_domain(ownDomain);
#endif


#if defined(MEEGO_EDITION_HARMATTAN)
    QList<QSslCertificate> certs = QSslCertificate::fromPath(QString(GLOBAL_CERTS_PATH).append(QDir::separator()).append("*.pem"), QSsl::Pem, QRegExp::Wildcard);
    certs += QSslCertificate::fromPath(QString(GLOBAL_CERTS_PATH).append(QDir::separator()).append("*.crt"), QSsl::Pem, QRegExp::Wildcard);

    certs += localCerts;
    QSslSocket::setDefaultCaCertificates(certs);
#endif


#ifdef QT_DEBUG
    qDebug() << "Locale: " << locale;
    qDebug() << "Read SSL Certificates: " << QSslSocket::defaultCaCertificates().count();
#endif

    // create database
    OcDbManager dbman;
    dbman.openDB();
    dbman.createTables();

    OcConfiguration* configuration = new OcConfiguration;
    new OcConfigAdaptor(configuration);

    int oldVersion = configuration->value("system/version", 0).toInt();
    if (oldVersion > 0 && oldVersion < VERSION)
    {
        qDebug() << "Performing internal updrades.";
        OcUpgradeHelper *upHelper = new OcUpgradeHelper;
        upHelper->init(oldVersion, VERSION);
        upHelper->deleteLater();
    }


    OcGeneric* generic = new OcGeneric;
    new OcGenericAdaptor(generic);
    OcFolders* folders = new OcFolders;
    new OcFoldersAdaptor(folders);
    OcFeeds* feeds = new OcFeeds;
    new OcFeedsAdaptor(feeds);
    OcItems* items = new OcItems;
    new OcItemsAdaptor(items);
    OcDownloadManager* dManager = new OcDownloadManager;
    new OcDownloadManagerAdaptor(dManager);
    OcImageFetcher* imgFetcher = new OcImageFetcher;
    new OcImageFetcherAdaptor(imgFetcher);
    OcUpdater* updater = new OcUpdater;
    new OcUpdaterAdaptor(updater);
#if defined(MEEGO_EDITION_HARMATTAN)
    OcAccount* account = new OcAccount;
    new OcAccountAdaptor(account);
#endif


    // updater connections
    QObject::connect(updater, SIGNAL(updateStarted()), folders, SLOT(requestFolders()));
    QObject::connect(folders, SIGNAL(requestedFoldersSuccess(QList<int>,QList<int>,QList<int>)), updater, SLOT(updateFeeds()));
    QObject::connect(folders, SIGNAL(requestedFoldersError(QString)), updater, SLOT(errorInUpdate(QString)));
    QObject::connect(updater, SIGNAL(startRequestFeeds()), feeds, SLOT(requestFeeds()));
    QObject::connect(feeds, SIGNAL(requestedFeedsSuccess(QList<int>, QList<int>, QList<int>)), updater, SLOT(updateItems()));
    QObject::connect(feeds, SIGNAL(requestedFeedsError(QString)), updater, SLOT(errorInUpdate(QString)));
    QObject::connect(updater, SIGNAL(startRequestItems(QString,QString,QString,QString,QString)), items, SLOT(requestItems(QString,QString,QString,QString,QString)));
    QObject::connect(updater, SIGNAL(startUpdateItems(QString,QString,QString)), items, SLOT(updateItems(QString,QString,QString)));
    QObject::connect(items, SIGNAL(updatedItemsSuccess(QList<int>, QList<int>, QList<int>)), updater, SLOT(endUpdate()));
    QObject::connect(items, SIGNAL(updatedItemsError(QString)), updater, SLOT(errorInUpdate(QString)));
    QObject::connect(items, SIGNAL(requestedItemsSuccess(QList<int>,QList<int>,QList<int>)), updater, SLOT(endUpdate()));
    QObject::connect(items, SIGNAL(requestedItemsError(QString)), updater, SLOT(errorInUpdate(QString)));
    QObject::connect(configuration, SIGNAL(updateBehaviorChanged(int)), updater, SLOT(setUpdateBehavior(int)));
    QObject::connect(configuration, SIGNAL(updateIntervalChanged(int)), updater, SLOT(setUpdateInterval(int)));

    // image fetcher connections
    QObject::connect(items, SIGNAL(updatedItemsSuccess(QList<int>,QList<int>,QList<int>)), imgFetcher, SLOT(fetchImages(QList<int>,QList<int>,QList<int>)));
    QObject::connect(items, SIGNAL(requestedItemsSuccess(QList<int>,QList<int>,QList<int>)), imgFetcher, SLOT(fetchImages(QList<int>,QList<int>,QList<int>)));
    QObject::connect(items, SIGNAL(cleanedItems(QList<int>,QList<int>,QList<int>)), imgFetcher, SLOT(fetchImages(QList<int>,QList<int>,QList<int>)));
    QObject::connect(configuration, SIGNAL(cleanedDatabase()), imgFetcher, SLOT(databaseCleaned()));

    // API interconnections
    QObject::connect(feeds, SIGNAL(requestedFeedsSuccess(QList<int>,QList<int>,QList<int>)), items, SLOT(cleanItems(QList<int>,QList<int>,QList<int>)));
    QObject::connect(feeds, SIGNAL(deletedFeedSuccess(int)), items, SLOT(cleanItems(int)));
    QObject::connect(folders, SIGNAL(deletedFolderCleanItems(QList<int>)), items, SLOT(cleanItems(QList<int>)));
    QObject::connect(feeds, SIGNAL(feedCreatedFetchItems(QString,QString,QString,QString,QString)), items, SLOT(requestItems(QString,QString,QString,QString,QString)));



    QDBusConnection connection = QDBusConnection::sessionBus();
    connection.registerService("de.buschmann23.ocNewsEngine");
    connection.registerObject("/", generic);
    connection.registerObject("/Configuration", configuration);
    connection.registerObject("/Folders", folders);
    connection.registerObject("/Feeds", feeds);
    connection.registerObject("/Items", items);
    connection.registerObject("/Updater", updater);
    connection.registerObject("/Downloads", dManager);
    connection.registerObject("/ImageFetcher", imgFetcher);
#if defined(MEEGO_EDITION_HARMATTAN)
    connection.registerObject("/Account", account);
#endif

    configuration->setValue("system/version", VERSION);

//    MRemoteAction evenFeedAction("de.buschmann23.ocNewsEngine", "/Updater", "de.buschmann23.ocNewsEngine", "startUpdate");
//    QDBusInterface eventFeedInterface("com.nokia.home.EventFeed", "/eventfeed", "com.nokia.home.EventFeed", QDBusConnection::sessionBus());
//    eventFeedInterface.call(QDBus::NoBlock, "addRefreshAction", evenFeedAction.toString());

    return a.exec();
}
