import QtQuick 1.1
import com.nokia.meego 1.0
import com.nokia.extras 1.0
import QtMobility.feedback 1.1

import "../Common"
import "../Delegates"
import "../Sheets"

Page {
    id: mainView
    Component.onCompleted: {
        if (dbus.isConfigSet() && dbus.isAccountEnabled()) {
            configState = 0
        } else if (!dbus.isConfigSet()) {
            configState = 1
        } else if (dbus.isConfigSet() && !dbus.isAccountEnabled()) {
            configState = 2
        }
    }

    property int configState

    tools: mainViewTools
    orientationLock: PageOrientation.LockPortrait

    function openFile(file, properties) {
             var component = Qt.createComponent(file)
             if (component.status == Component.Ready)
                 pageStack.push(component, properties);
             else
                 console.log("Error loading component:", component.errorString());
         }

    Connections {
        target: mainViewAddFeed
        onAccepted: if (mainViewAddFeed.feedAddressText !== "") mainViewHeader.indicatorState = "RUNNING"
    }
    Connections {
        target: mainViewAddFolder
        onAccepted: if (mainViewAddFolder.folderNameText !== "") mainViewHeader.indicatorState = "RUNNING"
    }
    Connections {
        target: mainViewRenameFolder
        onAccepted: if (mainViewRenameFolder.folderName !== mainViewRenameFolder.newFolderName && mainViewRenameFolder.newFolderName !== "") mainViewHeader.indicatorState = "RUNNING"
    }
    Connections {
        target: mainViewMoveFeed
        onAccepted: mainViewHeader.indicatorState = "RUNNING"
    }
    Connections {
        target: dbus
        onChangedConfig: {
            if (dbus.isConfigSet() && dbus.isAccountEnabled()) {
                configState = 0
            } else if (!dbus.isConfigSet()) {
                configState = 1
            } else if (dbus.isConfigSet() && !dbus.isAccountEnabled()) {
                configState = 2
            }
        }
    }


// ------------- Header Start ----------------

    Header {
        id: mainViewHeader
        text: qsTr("ocNews")
    }

// ------------- Header End ----------------


// ------------- Welcome Text Start ---------------
// only shown when account configuration is incomplete

    Item {
        anchors { fill: parent; topMargin: 90; }
        visible: configState === 1
        width: parent.width - 20

        Text {
            id: welcomeText
            anchors { top: parent.top; horizontalCenter: parent.horizontalCenter }
            text: qsTr("<b>Welcome to ocNews</b><br />With these app you can synchronize and view your ownCloud News App content on your smartphone. Before you can start, you have to setup your ownCloud server account.")
            width: parent.width - 30
            wrapMode: Text.WordWrap
            font.pointSize: 17
            font.weight: Font.Light
            horizontalAlignment: Text.AlignHCenter
            color: theme.inverted ? "white" : "black"
            textFormat: Text.StyledText
        }

        Text {
            id: updateHint
            anchors { top: welcomeText.bottom; topMargin: 20; horizontalCenter: parent.horizontalCenter }
            text: qsTr("<i>If you have updated ocNews from a version prior to 1.3.0, you have to resetup your account and resync your data, because of changed storage locations.</i>")
            width: parent.width - 30
            wrapMode: Text.WordWrap
            font.pointSize: 17
            font.weight: Font.Light
            horizontalAlignment: Text.AlignHCenter
            color: theme.inverted ? "white" : "black"
            textFormat: Text.StyledText
        }

        Button {
            id: privacyPolicyButton
            text: qsTr("Privacy policy")
            anchors { top: updateHint.bottom; topMargin: 20; horizontalCenter: parent.horizontalCenter }
            onClicked: {
                mainViewPrivacySheet.policy = dbus.getSetting("display/privacypolicy", false) == "true" ? true : false
                mainViewPrivacySheet.open();
            }

        }

        Button {
            id: setupAccountButton
            text: qsTr("Setup account")
            anchors { top: privacyPolicyButton.bottom; topMargin: 15; horizontalCenter: parent.horizontalCenter }
            onClicked: launcher.launch("/usr/bin/invoker", "-w,--type=d,--single-instance,/usr/lib/AccountSetup/bin/ocnewsplugin")
        }

//        Image {
//            id: ocnewsLogo
//            anchors { horizontalCenter: parent.horizontalCenter; top: setupAccountButton.bottom; topMargin: 30 }
//            asynchronous: true
//            width: 256
//            height: 256
//            sourceSize.width: 256
//            sourceSize.height: 256
//            source: "/opt/ocNewsReader/images/ocNews256.png"

//            MouseArea {
//                id: mouseAreaLogo;
//                anchors.fill: parent
//                onClicked: openFile("../Pages/About.qml")
//            }
//        }
    }
// ------------- Welcome Text End ---------------


// ------------- Account not enabled Start   ----------

    Item {
        anchors { fill: parent; topMargin: 90; }
        visible: configState === 2
        width: parent.width - 20

        Text {
            id: accountNotSetText
            anchors { top: parent.top; horizontalCenter: parent.horizontalCenter }
            text: qsTr("Your ocNews account is currently disabled. In order to use ocNews you have to enable your account.")
            width: parent.width - 30
            wrapMode: Text.WordWrap
            font.pointSize: 17
            font.weight: Font.Light
            horizontalAlignment: Text.AlignHCenter
            color: theme.inverted ? "white" : "black"
            textFormat: Text.StyledText
        }

        Button {
            id: enableAccountButton
            text: qsTr("Setup account")
            anchors { top: accountNotSetText.bottom; topMargin: 15; horizontalCenter: parent.horizontalCenter }
            onClicked: launcher.launch("/usr/bin/invoker", "--single-instance,--type=m,--splash=/usr/share/themes/blanco/meegotouch/images/splash/meegotouch-accounts-splash.jpg,/usr/bin/accounts-ui")
        }
    }

// ------------- Account not enabled Start   ----------


// ------------- Main View Start ---------------
    ListView {
        id: folderList
        anchors { fill: parent; topMargin: 71; leftMargin: 20; rightMargin: 20 }
        model: folderModelSql
        visible: configState === 0
        delegate: FolderListDelegate {
                 subtitleColor: "grey"
                 onClicked: {
                     if (type === "1")  {
                         openFile("FeedListView.qml", {folderId: id, folderName: title})
                     } else if (type === "0") {
                         openFile("ItemListView.qml", {feedId: id, feedName: title})
                     } else if (type === "-1" && id === "0") {
                         openFile("SpecialItemListView.qml", { pageName: title, feedType: "all" })
                     } else if (type === "-1" && id === "1") {
                         openFile("SpecialItemListView.qml", { pageName: title, feedType: "starred" })
                     }
                 }
                 onPressAndHold: {
                     contextMenuEffect.play();
                     if (type === "1") {
                         folderContextMenu.folderId = id
                         folderContextMenu.folderName = title
                         folderContextMenu.open()
                     } else if (type === "0") {
                         feedsContextMenu.feedId = id
                         feedsContextMenu.feedName = title
                         feedsContextMenu.open()
                     }
                 }
             }
    }

    ScrollDecorator {
        flickableItem: folderList
        anchors { right: parent.right; rightMargin: -folderList.anchors.rightMargin }
    }

    ThemeEffect { id: contextMenuEffect; effect: "LongPress" }


// ------------- Main View End ---------------



// ------------- ToolBar Start -------------
    ToolBarLayout {
        id: mainViewTools
        ToolIcon {
            id: addIcon
            platformIconId: "toolbar-add"
            anchors.left: (parent === undefined) ? undefined : parent.left
            onClicked: { (addMenu.status === DialogStatus.Closed) ? addMenu.open() : addMenu.close(); settingsMenu.close(); }
            visible: updater.isUpdateRunning() == false
            Connections {
                target: updater
                onUpdateFinished: addIcon.visible = true
                onUpdateError: addIcon.visible = true
                onUpdateStarted: addIcon.visible = false
            }
        }
        ToolIcon {
            id: updaterIcon
            platformIconId: "toolbar-refresh"
            state: updater.isUpdateRunning() ? "RUNNING" : "NORMAL"
            states: [
                State {
                    name: "NORMAL"
                    PropertyChanges { target: updaterIcon; visible: true; enabled: true; }
                },
                State {
                    name: "RUNNING"
                    PropertyChanges { target: updaterIcon; visible: false; enabled: false; }
                }
            ]
            onClicked: {
                updaterIcon.state = "RUNNING";
                mainViewHeader.indicatorState = "RUNNING";
                dbus.initConnection();
            }
            Connections {
                target: dbus
                onInitError: updaterIcon.state = "NORMAL"
                onInitSuccess: updater.startUpdate();
            }
            Connections {
                target: updater
                onUpdateFinished: updaterIcon.state = "NORMAL"
                onUpdateError: updaterIcon.state = "NORMAL"
                onUpdateStarted: updaterIcon.state = "RUNNING";
            }
        }
        ToolIcon {
            platformIconId: "toolbar-view-menu"
            anchors.right: (parent === undefined) ? undefined : parent.right
            onClicked: { (settingsMenu.status === DialogStatus.Closed) ? settingsMenu.open() : settingsMenu.close(); addMenu.close(); }
        }
    }


    Menu {
        id: settingsMenu
        visualParent: pageStack
        MenuLayout {
            MenuItem {
                text: qsTr("Settings")
                onClicked: openFile("../Pages/Settings.qml")
            }
            MenuItem {
                text: qsTr("About")
                onClicked: openFile("../Pages/About.qml")
            }
            MenuItem {
                text: qsTr("Quit completely")
                onClicked: quitCompletely.open()
            }
        }
    }

    Menu {
        id: addMenu
        visualParent: pageStack
        MenuLayout {
            MenuItem {
                text: qsTr("Add Folder")
                onClicked: mainViewAddFolder.open()
            }
            MenuItem {
                text: qsTr("Add Feed")
                onClicked: {
                    mainViewAddFeed.buildFolderList()
                    mainViewAddFeed.open()
                }
            }
        }
    }
// ----------------- ToolBar End -------------



// ----------------- Context Menu Start --------------

    ContextMenu {
        id: folderContextMenu
        property string folderId
        property string folderName
        MenuLayout {
            MenuItem {
                text: qsTr("Mark folder as read")
                onClicked: {
                    markReadQuery.id = folderContextMenu.folderId
                    markReadQuery.name = folderContextMenu.folderName
                    markReadQuery.open()
                }
            }
            MenuItem {
                text: qsTr("Update folder")
                onClicked: {
                    items.updateItems("0", "1", folderContextMenu.folderId)
                    mainViewHeader.indicatorState = "RUNNING"
                }
            }
            MenuItem {
                text: qsTr("Rename folder")
                onClicked: {
                    mainViewRenameFolder.folderId = folderContextMenu.folderId
                    mainViewRenameFolder.folderName = folderContextMenu.folderName
                    mainViewRenameFolder.open()
                }
            }
            MenuItem {
                text: qsTr("Delete folder")
                onClicked: {
                    deleteQuery.id = folderContextMenu.folderId
                    deleteQuery.name = folderContextMenu.folderName
                    deleteQuery.type = "1"
                    deleteQuery.open()
                }
            }
        }
    }

    ContextMenu {
        id: feedsContextMenu
        property string feedId
        property string feedName
        MenuLayout {
            MenuItem {
                text: qsTr("Mark feed as read")
                onClicked: {
                    feeds.markFeedRead(feedsContextMenu.feedId)
                    mainViewHeader.indicatorState = "RUNNING"
                }
            }
            MenuItem {
                text: qsTr("Update feed")
                onClicked: {
                    items.updateItems("0", "0", feedsContextMenu.feedId)
                    mainViewHeader.indicatorState = "RUNNING"
                }
            }
            MenuItem {
                text: qsTr("Move feed")
                onClicked: {
                    mainViewMoveFeed.feedId = feedsContextMenu.feedId
                    mainViewMoveFeed.feedName = feedsContextMenu.feedName
                    mainViewMoveFeed.buildFolderList()
                    mainViewMoveFeed.open()
                }
            }
            MenuItem {
                text: qsTr("Delete feed")
                onClicked: {
                    deleteQuery.id = feedsContextMenu.feedId
                    deleteQuery.name = feedsContextMenu.feedName
                    deleteQuery.type = "0"
                    deleteQuery.open()
                }
            }
        }
    }

// ----------------- Context Menu End --------------

// ----------------- Sheets Start --------------------

    AddFolderSheet {
        id: mainViewAddFolder
    }

    AddFeedSheet {
        id: mainViewAddFeed
    }

    RenameFolderSheet {
        id: mainViewRenameFolder
    }

    MoveFeedSheet {
        id: mainViewMoveFeed
    }

    PrivacySheet {
        id: mainViewPrivacySheet
    }

// ----------------- Sheets End --------------------


// ----------------- Dialogs Start --------------------

    QueryDialog {
        id: deleteQuery
        property string id
        property string name
        property string type
        icon: "image://theme/icon-m-bootloader-warning"
        acceptButtonText: qsTr("Delete")
        rejectButtonText: qsTr("Cancel")
        message: type === "1" ? qsTr("Do you really want to delete this folder? All feeds and posts in this folder will then be deleted, too.") : ""
        titleText: type === "1" ? qsTr("Delete folder %1?").arg(name) : qsTr("Delete feed %1?").arg(name)
        onAccepted: {
            mainViewHeader.indicatorState = "RUNNING"
            type === "1" ? folders.deleteFolder(id) : feeds.deleteFeed(id)
        }
    }

    QueryDialog {
        id: markReadQuery
        property string id
        property string name
        icon: "image://theme/icon-m-bootloader-warning"
        acceptButtonText: qsTr("Yes")
        rejectButtonText: qsTr("No")
        message: qsTr("Do you really want to mark the whole content of this folder as read?")
        titleText: qsTr("Mark folder %1 as read?").arg(name)
        onAccepted: {
            mainViewHeader.indicatorState = "RUNNING"
            folders.markFolderRead(id)
        }
    }

    QueryDialog {
        id: quitCompletely
        icon: "image://theme/icon-m-bootloader-warning"
        acceptButtonText: qsTr("Yes")
        rejectButtonText: qsTr("No")
        message: qsTr("If you quit both, reader and daemon/engine, there are no updates in the background anymore.")
        titleText: qsTr("Quit completely?")
        onAccepted: {
            dbus.quitEngine()
            Qt.quit()
        }
    }

// ----------------- Dialogs End --------------------

}
