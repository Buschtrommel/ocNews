#include <QtQml>
#include <QGuiApplication>
#include <QQuickView>
#include <QLocale>
#include <QTranslator>

#include <sailfishapp.h>

#include "../../common/globals.h"
#include "../common/ocdbmanager.h"
#include "../common/models/ocfoldermodelsql.h"
#include "../common/models/ocfeedsmodelsql.h"
#include "../common/models/ocitemsmodelsql.h"
#include "../common/models/ocitemsmodelnew.h"
#include "../common/models/ocitemsmodelfilter.h"
#include "../common/models/ocsingleitemmodelsql.h"
#include "../common/models/ocspecialitemsmodelsql.h"
#include "../common/models/occombinedmodelsql.h"
#include "../common/dbus/interfaces/ocdbusfolders.h"
#include "../common/dbus/interfaces/ocdbusfeeds.h"
#include "../common/dbus/interfaces/ocdbusitems.h"
#include "../common/dbus/interfaces/ocdbusupdater.h"
#include "../common/dbus/interfaces/ocdbusdownloads.h"
#include "../common/dbus/interfaces/ocdbusimagefetcher.h"
#include "../common/dbus/adaptor/ocdbusadaptor.h"
#include "../common/dbus/adaptor/ocdbusproxy.h"
#include "ocnaminteractor.h"

int main(int argc, char *argv[])
{
    QGuiApplication* app = SailfishApp::application(argc, argv);

    app->setOrganizationName("Buschtrommel");
    app->setOrganizationDomain("buschmann23.de");
    app->setApplicationName("harbour-ocnews-reader");
    app->setApplicationVersion(VERSION_STRING);

    QString locale = QLocale::system().name();
    QTranslator *translator = new QTranslator;
    if ((translator->load("ocnewsreader_"+locale, L10N_PATH)))
        app->installTranslator(translator);
#ifdef QT_DEBUG
    qDebug() << locale;
#endif

    // start background daemon ocnews-engine via dbus
    QDBusConnectionInterface* qDbusConInf = QDBusConnection::sessionBus().interface();
    qDbusConInf->startService("de.buschmann23.ocNewsEngine");

    OcDbManager dbman;
    dbman.openDB();

    OcDBusProxy* dbusproxy = new OcDBusProxy();
    new OcDBusAdaptor(dbusproxy);

    OcDBusInterface dbus;
    OcDBusFolders folders;
    OcDBusFeeds feeds;
    OcDBusItems items;
    OcDBusUpdater updater;
    OcDBusDownloads downloads;
    OcDBusImageFetcher imageFetcher;

    OcFolderModelSql *folderModelSql = new OcFolderModelSql();
    OcCombinedModelSql *combinedModelSql = new OcCombinedModelSql();
    OcFeedsModelSql *feedsModelSql = new OcFeedsModelSql();
//    OcItemsModelSql *itemsModelSql = new OcItemsModelSql();
    OcItemsModelNew *itemsModelSql = new OcItemsModelNew();
    OcItemsModelFilter *itemsModelFilter = new OcItemsModelFilter;
    itemsModelFilter->setSourceModel(itemsModelSql);
    OcSpecialItemsModelSql *specialItemsModelSql = new OcSpecialItemsModelSql();
    OcSingleItemModelSql *singleItemModelSql = new OcSingleItemModelSql();


    // connections to the items model
    QObject::connect(&items, SIGNAL(markedItemsSuccess(QStringList,QString)), itemsModelSql, SLOT(itemsMarked(QStringList,QString)));
    QObject::connect(&items, SIGNAL(starredItemsSuccess(QStringList,QString)), itemsModelSql, SLOT(itemsStarred(QStringList,QString)));
    QObject::connect(&feeds, SIGNAL(markedReadFeedSuccess(QString)), itemsModelSql, SLOT(feedMarkedRead(QString)));
    QObject::connect(&items, SIGNAL(updatedItemsSuccess(QList<int>,QList<int>,QList<int>)), itemsModelSql, SLOT(itemsUpdated(QList<int>,QList<int>,QList<int>)));
    QObject::connect(&items, SIGNAL(requestedItemsSuccess(QList<int>,QList<int>,QList<int>)), itemsModelSql, SLOT(itemsUpdated(QList<int>,QList<int>,QList<int>)));


    // register reader dbus interface
    QDBusConnection connection = QDBusConnection::sessionBus();
    bool ret = connection.registerService("harbour.ocnews.reader");
    ret = connection.registerObject("/", dbusproxy);

    if (!ret)
        qDebug() << "Failed to register D-Bus interface...";

    QQuickView* view = SailfishApp::createView();

    OcNamInteractor *nami = new OcNamInteractor(view->engine(), view);

    QObject::connect(view->engine(), SIGNAL(quit()), QCoreApplication::instance(), SLOT(quit()));

    if (dbus.getSetting("engine/quitonclose", false).toBool())
        QObject::connect(QCoreApplication::instance(), SIGNAL(aboutToQuit()), &dbus, SLOT(quitEngine()));

    view->rootContext()->setContextProperty("folderModelSql", folderModelSql);
    view->rootContext()->setContextProperty("combinedModelSql", combinedModelSql);
    view->rootContext()->setContextProperty("feedsModelSql", feedsModelSql);
    view->rootContext()->setContextProperty("itemsModelSql", itemsModelSql);
    view->rootContext()->setContextProperty("itemsModelFilter", itemsModelFilter);
    view->rootContext()->setContextProperty("specialItemsModelSql", specialItemsModelSql);
    view->rootContext()->setContextProperty("singleItemModelSql", singleItemModelSql);
    view->rootContext()->setContextProperty("folders", &folders);
    view->rootContext()->setContextProperty("feeds", &feeds);
    view->rootContext()->setContextProperty("items", &items);
    view->rootContext()->setContextProperty("updater", &updater);
    view->rootContext()->setContextProperty("dbus", &dbus);
    view->rootContext()->setContextProperty("dbusproxy", dbusproxy);
    view->rootContext()->setContextProperty("downloads", &downloads);
    view->rootContext()->setContextProperty("imageFetcher", &imageFetcher);
    view->rootContext()->setContextProperty("nami", nami);
    view->rootContext()->setContextProperty("versionString", VERSION_STRING);
    view->rootContext()->setContextProperty("versionInt", VERSION);

    view->setSource(QUrl::fromLocalFile("/usr/share/harbour-ocnews-reader/qml/harbour-ocnews-reader.qml"));
    view->show();

    return app->exec();
}
