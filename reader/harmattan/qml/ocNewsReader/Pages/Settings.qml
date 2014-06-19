import QtQuick 1.1
import com.nokia.meego 1.0
import com.nokia.extras 1.0

import "../Common"
import "../Sheets"
import "../Delegates"

Page {
    id: settings

    tools: settingsTools

    orientationLock: PageOrientation.LockPortrait

    property int maxFeeds
    property bool isConfigSet

    function saveValues() {

        // get IDs of the selected feeds for the event view
        var selInd = feedSelectionDialog.selectedIndexes;
        var selFed = new Array();
        for (var i=0, tot=selInd.length; i < tot; i++) {
            selFed.push(feedSelectionDialog.model.get(feedSelectionDialog.selectedIndexes[i]).value);
        }

        var saveConf={
            maxitems:chooseMaxItems.currentValue,
            viewmode:chooseViewMode.currentValue,
            orderby:chooseOrderBy.currentValue,
            textformat:textFormatSelection.valueChoosen,
            themecolor:invertThemeSelection.choosenValue,
            handleimgs:handleImgs.currentValue,
            updatebehavior:updateBehavior.currentValue,
            updateinterval:updateInterval.currentValue,
            eventfeeds:selFed.toString(),
            handleread:handleRead.currentValue,
            sortasc:sortAsc.checked,
            fontsize:fontSizeSelector.value,
            hidereadfeeds:hideReadFeeds.checked,
            quitengine:quitEngine.checked,
            notifyFeedsFolders:feedsFoldersNotify.checked,
            notifyNewItems:newItemsNotify.checked
        };

        return saveConf;
    }

    Component.onCompleted: {
        isConfigSet = dbus.isConfigSet();
        dbus.getConfig();
        if (dbus.getSetting("display/privacypolicy", false) == false) {
            settingsPrivacySheet.policy = dbus.getSetting("display/privacypolicy", false) == "true" ? true : false;
            settingsPrivacySheet.open();
        }
        dbus.getStatistics();
    }

    Component.onDestruction: dbus.saveConfig(saveValues());

    // ------------- Header Start ----------------

        Header {
            id: settingsPageHeader
            text: qsTr("Account Settings")
        }

    // ------------- Header End ----------------



    TabGroup {
        id: tabgroup
        currentTab: tab1
        anchors { top: settingsPageHeader.bottom; topMargin: 5 }

        Page {
            id: tab1
            orientationLock: PageOrientation.LockPortrait

            Button {
                id: accountButton
                anchors { horizontalCenter: parent.horizontalCenter; top: parent.top; topMargin: 20 }
                text: isConfigSet ? qsTr("Open account manager") : qsTr("Create new account")
                onClicked: isConfigSet ? launcher.launch("/usr/bin/invoker", "--single-instance,--type=m,--splash=/usr/share/themes/blanco/meegotouch/images/splash/meegotouch-accounts-splash.jpg,/usr/bin/accounts-ui") : launcher.launch("/usr/bin/invoker", "-w,--type=d,--single-instance,/usr/lib/AccountSetup/bin/ocnewsplugin")
            }

            Text {
                id: accountButtonDescription
                anchors { top: accountButton.bottom; topMargin: 20 }
                textFormat: Text.PlainText
                width: parent.width
                wrapMode: Text.WordWrap
                color: theme.inverted ? "white" : "black"
                font.pointSize: 17
                font.weight: Font.Light
                text: isConfigSet ? qsTr("Your account is successfully set. You can configure it in the account manager.") : qsTr("You need to create an account for your ownCloud server. Click the button above to create a new account in the account manager.")
            }
        }

        Page {
            id: tab2
            orientationLock: PageOrientation.LockPortrait

            Flickable {
                id: tab2Content
                anchors { right: parent.right; left: parent.left; top: parent.top; bottom: parent.bottom; bottomMargin: 70 }
                contentWidth: parent.width
                contentHeight: chooseMaxItems.height + chooseViewMode.height + chooseOrderBy.height + hideReadFeeds.height + updateBehavior.height + updateInterval.height + textFormatLabel.height + textFormatSelection.height + invertThemeLabel.height + invertThemeSelection.height + handleImgs.height + handleRead.height + fontSizeLabel.height + fontSizeSelector.height + quitEngine.height + quitEngineDesc.height + sortAsc.height + 150

                flickableDirection:  Flickable.VerticalFlick

                ListModel {
                    id: maxItemModel
                    ListElement { name: "50"; value: "50" }
                    ListElement { name: "100"; value: "100" }
                    ListElement { name: "200"; value: "200" }
                    ListElement { name: "500"; value: "500" }
                }

                SelectionItem {
                    id: chooseMaxItems
                    title: qsTr("Number of items to keep")
                    width: (parent.width - 40)
                    anchors { top: parent.top; topMargin: 10; horizontalCenter: parent.horizontalCenter }
                    model: maxItemModel
                    initialValue: "100"
                    Connections {
                        target: dbus
                        onGotConfig: chooseMaxItems.initialValue = config["maxitems"]
                    }
                }

                ListModel {
                    id: viewModeModel
                    ListElement { name: QT_TR_NOOP("Folders"); value: 0 }
                    ListElement { name: QT_TR_NOOP("Feeds"); value: 1 }
                }

                SelectionItem {
                    id: chooseViewMode
                    title: qsTr("Main view layout")
                    width: (parent.width - 40)
                    anchors { top: chooseMaxItems.bottom; topMargin: 10; horizontalCenter: parent.horizontalCenter }
                    model: viewModeModel
                    initialValue: "0"
                    tsBasename: "Settings"
                    Connections {
                        target: dbus
                        onGotConfig: chooseViewMode.initialValue = config["viewmode"]
                    }
                }

                ListModel {
                    id: orderByModel
                    ListElement { name: QT_TR_NOOP("Added (like on server)"); value: "id" }
                    ListElement { name: QT_TR_NOOP("Title"); value: "title" }
                    ListElement { name: QT_TR_NOOP("Unread count"); value: "unreadCount" }
                }

                SelectionItem {
                    id: chooseOrderBy
                    title: qsTr("Order feeds and folders by")
                    width: parent.width - 40
                    anchors { top: chooseViewMode.bottom; topMargin: 10; horizontalCenter: parent.horizontalCenter }
                    model: orderByModel
                    initialValue: "id"
                    tsBasename: "Settings"
                    Connections {
                        target: dbus
                        onGotConfig: chooseOrderBy.initialValue = config["orderby"]
                    }
                }

                LabeledSwitch {
                    id: hideReadFeeds
                    width: parent.width - 40
                    anchors { top: chooseOrderBy.bottom; horizontalCenter: parent.horizontalCenter }
                    text: qsTr("Hide read feeds and folders")
                    Connections {
                        target: dbus
                        onGotConfig: hideReadFeeds.checked = config["hidereadfeeds"]
                    }
                }

                ListModel {
                    id: updateBehaviorModel
                    ListElement { name: QT_TR_NOOP("Only manually"); value: "0" }
                    ListElement { name: QT_TR_NOOP("Automatic on Wi-Fi"); value: "1" }
                    ListElement { name: QT_TR_NOOP("Always automatic"); value: "2" }
                }

                SelectionItem {
                    id: updateBehavior
                    title: qsTr("Updating")
                    width: parent.width - 40
                    anchors { top: hideReadFeeds.bottom; topMargin: 10; horizontalCenter: parent.horizontalCenter }
                    model: updateBehaviorModel
                    initialValue: "0"
                    tsBasename: "Settings"
                    Connections {
                        target: dbus
                        onGotConfig: updateBehavior.initialValue = config["updatebehavior"]
                    }
                }

                ListModel {
                    id: updateIntervalModel
                    ListElement { name: QT_TR_NOOP("15 Minutes"); value: "900" }
                    ListElement { name: QT_TR_NOOP("30 Minutes"); value: "1800" }
                    ListElement { name: QT_TR_NOOP("1 Hour"); value: "3600" }
                    ListElement { name: QT_TR_NOOP("3 Hours"); value: "10800" }
                    ListElement { name: QT_TR_NOOP("6 Hours"); value: "21600" }
                    ListElement { name: QT_TR_NOOP("12 Hours"); value: "43200" }
                }

                SelectionItem {
                   id: updateInterval
                   visible: updateBehavior.currentValue != "0"
                   title: qsTr("Update interval")
                   width: parent.width - 40
                   anchors { top: updateBehavior.bottom; topMargin: 10; horizontalCenter: parent.horizontalCenter }
                   model: updateIntervalModel
                   initialValue: "3600"
                   tsBasename: "Settings"
                   Connections {
                       target: dbus
                       onGotConfig: updateInterval.initialValue = config["updateinterval"]
                   }
                }

                Label {
                    id: textFormatLabel
                    anchors { top: updateBehavior.currentValue == "0" ? updateBehavior.bottom : updateInterval.bottom; topMargin: 10; left: parent.left; leftMargin: 20 }
                    text: qsTr("Display post content format")
                    textFormat: Text.PlainText
                }

                ButtonRow {
                    id: textFormatSelection
                    property string valueChoosen
                    anchors { top: textFormatLabel.bottom; horizontalCenter: parent.horizontalCenter }
                    Button {
                        id: rich;
                        text: qsTr("Full");
                        checked: textFormatSelection.valueChoosen == "rich";
                        onClicked: textFormatSelection.valueChoosen = "rich"
                    }
                    Button {
                        id: styled;
                        text: qsTr("Limited");
                        checked: textFormatSelection.valueChoosen == "styled";
                        onClicked: textFormatSelection.valueChoosen = "styled"
                    }
                    Connections {
                        target: dbus
                        onGotConfig: textFormatSelection.valueChoosen = config["textformat"]
                    }
                }


                Label {
                    id: invertThemeLabel
                    anchors { top: textFormatSelection.bottom; topMargin: 15; left: parent.left; leftMargin: 20 }
                    text: qsTr("Theme color")
                }

                ButtonRow {
                    id: invertThemeSelection
                    property string choosenValue
                    anchors { top: invertThemeLabel.bottom; horizontalCenter: parent.horizontalCenter }
                    Button {
                        id: white
                        text: qsTr("White")
                        checked: invertThemeSelection.choosenValue == "white"
                        onClicked: { invertThemeSelection.choosenValue = "white"; theme.inverted = false }
                    }
                    Button {
                        id: black
                        text: qsTr("Black")
                        checked: invertThemeSelection.choosenValue == "black"
                        onClicked: { invertThemeSelection.choosenValue = "black"; theme.inverted = true }
                    }
                    Connections {
                        target: dbus
                        onGotConfig: invertThemeSelection.choosenValue = config["themecolor"]
                    }
                }


                ListModel {
                    id: handleImgsModel
                    ListElement { name: QT_TR_NOOP("On request"); value: 0 }
                    ListElement { name: QT_TR_NOOP("When item loads"); value: 1 }
                    ListElement { name: QT_TR_NOOP("Prefetch at updating"); value: 2 }
                }

                SelectionItem {
                    id: handleImgs
                    title: qsTr("Load content images")
                    width: parent.width - 40
                    anchors { top: invertThemeSelection.bottom; topMargin: 10; horizontalCenter: parent.horizontalCenter }
                    enabled: textFormatSelection.valueChoosen === "rich"
                    model: handleImgsModel
                    initialValue: "0"
                    tsBasename: "Settings"
                    Connections {
                        target: dbus
                        onGotConfig: handleImgs.initialValue = config["handleimgs"]
                    }
                }


                ListModel {
                    id: handleReadModel
                    ListElement { name: QT_TR_NOOP("Show"); value: 0 }
                    ListElement { name: QT_TR_NOOP("Hide"); value: 1 }
                    ListElement { name: QT_TR_NOOP("Show after unread"); value: 2 }
                }

                SelectionItem {
                   id: handleRead
                   title: qsTr("Read articles")
                   width: parent.width - 40
                   anchors { top: handleImgs.bottom; topMargin: 10; horizontalCenter: parent.horizontalCenter }
                   model: handleReadModel
                   initialValue: "0"
                   Connections {
                       target: dbus
                       onGotConfig: handleRead.initialValue = config["handleread"]
                   }
                }

                LabeledSwitch {
                    id: sortAsc
                    width: parent.width - 40
                    anchors { top: handleRead.bottom; topMargin: 10; horizontalCenter: parent.horizontalCenter }
                    text: qsTr("Show oldest items on top")
                    Connections {
                        target: dbus
                        onGotConfig: sortAsc.checked = config["sortasc"]
                    }
                }

                Label {
                    id: fontSizeLabel
                    anchors { top: sortAsc.bottom; topMargin: 15; left: parent.left; leftMargin: 20 }
                    width: parent.width - 20
                    text: qsTr("Item view font size:") + " " + fontSizeSelector.value + "pt"
                    wrapMode: Text.WrapAtWordBoundaryOrAnywhere
                }

                Slider {
                    id: fontSizeSelector
                    width: parent.width
                    anchors { top: fontSizeLabel.bottom }
                    minimumValue: 10;
                    maximumValue: 40;
                    valueIndicatorVisible: false
                    stepSize: 1
                    Connections {
                        target: dbus
                        onGotConfig: fontSizeSelector.value = config["fontsize"]
                    }
                }

                LabeledSwitch {
                    id: feedsFoldersNotify
                    width: parent.width - 40
                    anchors { top: fontSizeSelector.bottom; topMargin: 10; horizontalCenter: parent.horizontalCenter }
                    text: qsTr("Notify about added/removed feeds and folders")
                    Connections {
                        target: dbus
                        onGotConfig: feedsFoldersNotify.checked = config["notifyFeedsFolders"]
                    }
                }

                LabeledSwitch {
                    id: newItemsNotify
                    width: parent.width - 40
                    anchors { top: feedsFoldersNotify.bottom; topMargin: 10; horizontalCenter: parent.horizontalCenter }
                    text: qsTr("Notify about new articles")
                    Connections {
                        target: dbus
                        onGotConfig: newItemsNotify.checked = config["notifyNewItems"]
                    }
                }

                LabeledSwitch {
                    id: quitEngine
                    width: parent.width - 40
                    anchors { top: newItemsNotify.bottom; topMargin: 10; horizontalCenter: parent.horizontalCenter }
                    text: qsTr("Quit engine on closing reader")
                    Connections {
                        target: dbus
                        onGotConfig: quitEngine.checked = config["quitengine"]
                    }
                }

                Text {
                    id: quitEngineDesc
                    text: qsTr("This option needs an application restart.")
                    width: parent.width - 20
                    anchors { top: quitEngine.bottom; topMargin: 7; left: parent.left; leftMargin: 20 }
                    wrapMode: Text.WordWrap
                    font.pointSize: 17
                    font.weight: Font.Light
                    color: theme.inverted ? "white" : "black"
                    textFormat: Text.PlainText
                }
            }

            ScrollDecorator {
                flickableItem: tab2Content
                anchors { right: parent.right }
            }
        }

        Page {
            id: tab3
            orientationLock: PageOrientation.LockPortrait

            function buildFolderList()
            {
                var fee = feeds.getFeeds();

                feedModelData.clear()

                for (var prop in fee)
                {
                    feedModelData.append({"name":prop, "value":fee[prop]})
                }
            }

            Component.onCompleted: buildFolderList()

            ListModel {
                id: feedModelData
                ListElement { name: "Root";  value: "0"; checked: "false" }
            }

            MultiSelectionDialog {
                id: feedSelectionDialog
                titleText: qsTr("Select feeds for the event view")
                model: feedModelData
                acceptButtonText: "OK"

                Connections {
                    target: dbus
                    onGotConfig: {
                        // convert the string got from the settings into the selected model items
                        var loadedFeedIds = JSON.parse("[" + config["eventfeeds"] + "]");
                        var selectedModelIdx = new Array()
                        for (var i=0, tot=loadedFeedIds.length; i < tot; i++) {
                            var found = false;
                            var ii = 0;
                            while ((!found) && (ii < feedModelData.count)) {
                                if (feedModelData.get(ii).value == loadedFeedIds[i]) {
                                    selectedModelIdx.push(ii);
                                    found = true;
                                }
                                ii++;
                            }
                        }
                        feedSelectionDialog.selectedIndexes = selectedModelIdx;
                    }

                }
            }

            Button {
                id: feedSelectionButton
                anchors { horizontalCenter: parent.horizontalCenter; top: parent.top; topMargin: 20 }
                text: qsTr("Select event view feeds")
                onClicked: feedSelectionDialog.open();
            }

            Text {
                id: feedSelectionDescription
                anchors { top: feedSelectionButton.bottom; topMargin: 20 }
                textFormat: Text.PlainText
                width: parent.width
                wrapMode: Text.WordWrap
                color: theme.inverted ? "white" : "black"
                font.pointSize: 17
                font.weight: Font.Light
                text: qsTr("Click the button to select the feeds that you want to aggregate to the Event screen. Only newly downloaded items will be added to the Events screen.")
            }

        }

        Page {
            id: tab4
            orientationLock: PageOrientation.LockPortrait


            Label {
                id: statsLabel
                text: qsTr("Database statistics")
                anchors { top: parent.top; left: parent.left; topMargin: 20; leftMargin: 20 }
                textFormat: Text.PlainText
            }

            Text {
                id: folderCount
                anchors { top: statsLabel.bottom; left: parent.left; topMargin: 10; leftMargin: 20 }
                font.pointSize: 17
                font.weight: Font.Light
                textFormat: Text.PlainText
                color: theme.inverted ? "white" : "black"
                Connections {
                    target: dbus
                    onGotStatistics: folderCount.text = qsTr("Folders: ") + stats["folderCount"]
                }
            }

            Text {
                id: feedCount
                anchors { top: folderCount.bottom; left: parent.left; topMargin: 10; leftMargin: 20 }
                font.pointSize: 17
                font.weight: Font.Light
                textFormat: Text.PlainText
                color: theme.inverted ? "white" : "black"
                Connections {
                    target: dbus
                    onGotStatistics: feedCount.text = qsTr("Feeds: ") + stats["feedCount"]
                }
            }

            Text {
                id: itemCount
                anchors { top: feedCount.bottom; left: parent.left; topMargin: 10; leftMargin: 20 }
                font.pointSize: 17
                font.weight: Font.Light
                textFormat: Text.PlainText
                color: theme.inverted ? "white" : "black"
                Connections {
                    target: dbus
                    onGotStatistics: itemCount.text = qsTr("Posts: ") + stats["itemCount"]
                }
            }

            Text {
                id: unreadCount
                anchors { top: itemCount.bottom; left: parent.left; topMargin: 10; leftMargin: 20 }
                font.pointSize: 17
                font.weight: Font.Light
                textFormat: Text.PlainText
                color: theme.inverted ? "white" : "black"
                Connections {
                    target: dbus
                    onGotStatistics: unreadCount.text = qsTr("Unread: ") + stats["unreadCount"]
                }
            }

            Text {
                id: lastFullUpdate
                anchors { top: unreadCount.bottom; left: parent.left; topMargin: 10; leftMargin: 20 }
                font.pointSize: 17
                font.weight: Font.Light
                textFormat: Text.StyledText
                color: theme.inverted ? "white" : "black"
                Connections {
                    target: dbus
                    onGotStatistics: lastFullUpdate.text = qsTr("Last full update:<br />") + Qt.formatDateTime(new Date(stats["lastFullUpdate"]), qsTr("d. MMMM yyyy, hh:mm"))
                }
            }


            Button {
                id: resetConfigButton
                anchors { horizontalCenter: parent.horizontalCenter; top: lastFullUpdate.bottom; topMargin: 40 }
                text: qsTr("Reset configuration")
                onClicked: resetConfigQuery.open()
            }

            Button {
                id: deleteDBButton
                anchors { horizontalCenter: parent.horizontalCenter; top: resetConfigButton.bottom; topMargin: 20 }
                text: qsTr("Delete database")
                onClicked: deleteDBQuery.open()
                Connections {
                    target: dbus
                    onGotStatistics: stats["feedCount"] + stats["folderCount"] + stats["itemCount"] == 0 ? deleteDBButton.enabled = false : deleteDBButton.enabled = true
                }
            }

            Button {
                id: deleteCertsButton
                anchors { horizontalCenter: parent.horizontalCenter; top: deleteDBButton.bottom; topMargin: 20 }
                text: qsTr("Remove certificates")
                onClicked: deleteCertsQuery.open()
            }
        }
    }



// ----------------- Toolbar Start --------------------

    ToolBarLayout {
        id: settingsTools
        visible: true
        ToolIcon {
            platformIconId: "toolbar-back"
            anchors.left: (parent === undefined) ? undefined : parent.left
            onClicked: { pageStack.pop(); }
        }
        ButtonRow {
            style: TabButtonStyle { }
            TabButton {
                iconSource: "image://theme/icon-m-toolbar-contact"
                tab: tab1
                onClicked: settingsPageHeader.text = qsTr("Account Settings")
            }
            TabButton {
                iconSource: "image://theme/icon-m-toolbar-settings"
                tab: tab2
                onClicked: settingsPageHeader.text = qsTr("Display & Update Settings")
            }
            TabButton {
                iconSource: "image://theme/icon-m-toolbar-list"
                tab: tab3
                onClicked: settingsPageHeader.text = qsTr("Event Feed Integration")
            }
            TabButton {
                iconSource: "image://theme/icon-m-toolbar-tools"
                tab: tab4
                onClicked: settingsPageHeader.text = qsTr("Maintenance")
            }
        }
    }
// ----------------- Toolbar End --------------------



// ----------------- Dialogs Start --------------------

    QueryDialog {
        id: resetConfigQuery
        icon: "image://theme/icon-m-bootloader-warning"
        acceptButtonText: qsTr("Reset")
        rejectButtonText: qsTr("Cancel")
        message: qsTr("Do you really want reset your configuration?")
        titleText: qsTr("Reset configuration?")
        onAccepted: dbus.resetConfig()
    }


    QueryDialog {
        id: deleteDBQuery
        icon: "image://theme/icon-m-bootloader-warning"
        acceptButtonText: qsTr("Delete")
        rejectButtonText: qsTr("Cancel")
        message: qsTr("Do you really want delete your complete database?")
        titleText: qsTr("Delete database?")
        onAccepted: dbus.cleanDatabase()
    }

    QueryDialog {
        id: deleteCertsQuery
        icon: "image://theme/icon-m-bootloader-warning"
        acceptButtonText: qsTr("Remove")
        rejectButtonText: qsTr("Cancel")
        message: qsTr("Do you really want remove your trustet SSL certificates (only for this application)?")
        titleText: qsTr("Remove trusted certificates?")
        onAccepted: dbus.cleanCertificates()
    }

    QueryDialog {
        id: disableSSLQuery
        icon: "image://theme/icon-m-bootloader-warning"
        acceptButtonText: qsTr("Disable")
        rejectButtonText: qsTr("Keep activated")
        message: qsTr("It is not a good idea to disable HTTPS (SSL), because your username and password then will be sent unencrypted with every request.")
        titleText: qsTr("Disable HTTPS?")
        onAccepted: sslCheck.checked = "false"
        onRejected: sslCheck.checked = "true"
    }

// ----------------- Dialogs End --------------------


// ----------------- Sheet Start --------------------

    PrivacySheet {
        id: settingsPrivacySheet
    }

// ----------------- Sheet End --------------------
}
