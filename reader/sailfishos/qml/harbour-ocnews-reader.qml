import QtQuick 2.0
import Sailfish.Silica 1.0

import "Views"
import "Pages"

ApplicationWindow
{
    id: ocNewsReader

    property bool operationRunning: updater.isUpdateRunning()
    property int unreadItems: dbus.getStat(0)
    property int lastFullUpdateTime: dbus.getStat(1)

    Connections {
        target: dbusproxy
        onDbusActivate: ocNewsReader.activate()
    }

    Connections {
        target: updater
        onUpdateFinished: { operationRunning = false; unreadItems = dbus.getStat(0); lastFullUpdateTime = dbus.getStat(1) }
        onUpdateError: operationRunning = false
        onUpdateStarted: operationRunning = true
    }
    Connections {
        target: folders
        onCreatedFolderError: operationRunning = false
        onCreatedFolderSuccess: operationRunning = false
        onDeletedFolderError: operationRunning = false
        onDeletedFolderSuccess: { operationRunning = false; unreadItems = dbus.getStat(0) }
        onMarkedReadFolderError: operationRunning = false
        onMarkedReadFolderSuccess: { operationRunning = false; unreadItems = dbus.getStat(0) }
        onRenamedFolderError: operationRunning = false
        onRenamedFolderSuccess: operationRunning = false
    }
    Connections {
        target: feeds
        onCreatedFeedError: operationRunning = false
        onCreatedFeedSuccess: { operationRunning = false; unreadItems = dbus.getStat(0) }
        onDeletedFeedError: operationRunning = false
        onDeletedFeedSuccess: { operationRunning = false; unreadItems = dbus.getStat(0) }
        onMarkedReadFeedError: operationRunning = false
        onMarkedReadFeedSuccess: { operationRunning = false; unreadItems = dbus.getStat(0) }
        onMovedFeedError: operationRunning = false
        onMovedFeedSuccess: operationRunning = false
        onRenamedFeedSuccess: operationRunning = false
        onRenamedFeedError: operationRunning = false
    }
    Connections {
        target: items
        onMarkedAllItemsReadError: operationRunning = false
        onMarkedAllItemsReadSuccess: { operationRunning = false; unreadItems = dbus.getStat(0) }
        onMarkedItemsError: operationRunning = false
        onMarkedItemsSuccess: { operationRunning = false; unreadItems = dbus.getStat(0) }
        onStarredItemsError: operationRunning = false
        onStarredItemsSuccess: operationRunning = false
        onUpdatedItemsError: operationRunning = false
        onUpdatedItemsSuccess: { operationRunning = false; unreadItems = dbus.getStat(0) }
        onRequestedItemsError: operationRunning = false
        onRequestedItemsSuccess: { operationRunning = false; unreadItems = dbus.getStat(0) }
    }

    initialPage: Component { MainView { } }

    cover: CoverPage {}

    QtObject {
        id: coverConnector
        property string mode: "overview"
    }

    Connections {
        target: pageStack
        onCurrentPageChanged: coverConnector.mode = pageStack.currentPage.objectName === "ItemPage" ? "item" : "overview";
    }
}
