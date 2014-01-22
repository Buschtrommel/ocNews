import QtQuick 1.1
import com.nokia.meego 1.0

import "../Common"
import "../Sheets"

Page {
    id: about

    tools: commonTools

    orientationLock: PageOrientation.LockPortrait

    property string _RICHTEXT_STYLESHEET_PREAMBLE: "<html><style>a { text-decoration: none; color: '" + theme.selectionColor + "' }</style><body>";
    property string _RICHTEXT_STYLESHEET_APPENDIX: "</body></html>";


    // ------------- Header Start ----------------

        Header {
            id: aboutPageHeader
            text: qsTr("About")
        }

    // ------------- Header End ----------------


    Image {
        id: ocnewsLogo
        anchors { horizontalCenter: parent.horizontalCenter; top: parent.top; topMargin: 120 }
        asynchronous: true
        width: 256
        height: 256
        sourceSize.width: 256
        sourceSize.height: 256
        source: "/opt/ocNewsReader/images/ocNews256.png"
    }

    Label {
        id: labelName
        anchors { horizontalCenter: parent.horizontalCenter; top: ocnewsLogo.bottom; topMargin: 5 }
        textFormat: Text.PlainText
        text: "ocNews " + versionString
    }

    Text {
        id: description
        anchors { horizontalCenter: parent.horizontalCenter; top: labelName.bottom; topMargin: 5 }
        text: qsTr("The ownCloud News App client for Harmattan")
        width: parent.width
        font.pointSize: 17
        font.weight: Font.Light
        wrapMode: Text.WordWrap
        horizontalAlignment: Text.AlignHCenter
        textFormat: Text.PlainText
        color: theme.inverted ? "white" : "black"
    }

    Text {
        id: copyright
        anchors { horizontalCenter: parent.horizontalCenter; top: description.bottom; topMargin: 5 }
        font.pointSize: 17
        font.weight: Font.Light
        text: "© 2013-2014, Buschtrommel"
        textFormat: Text.PlainText
        color: theme.inverted ? "white" : "black"
    }

    Text {
        id: license
        anchors { horizontalCenter: parent.horizontalCenter; top: copyright.bottom; topMargin: 5 }
        font.pointSize: 17
        font.weight: Font.Light
        text: _RICHTEXT_STYLESHEET_PREAMBLE + qsTr("Licensed under the <a href='http://www.gnu.org/licenses/gpl-2.0.en.html'>GNU GPL v2</a>") + _RICHTEXT_STYLESHEET_APPENDIX
        textFormat: Text.RichText
        color: theme.inverted ? "white" : "black"
        onLinkActivated: { Qt.openUrlExternally(link) }
    }

    Button {
        id: privacyPolicyButton
        anchors { horizontalCenter: parent.horizontalCenter; top: license.bottom; topMargin: 30 }
        text: qsTr("Privacy policy")
        onClicked: {
            aboutPrivacySheet.policy = dbus.getSetting("display/privacypolicy", false) == "true" ? true : false;
            aboutPrivacySheet.open();
        }
    }

    Button {
        id: websiteLink
        anchors { horizontalCenter: parent.horizontalCenter; top: privacyPolicyButton.bottom; topMargin: 10 }
        text: qsTr("Visit website")
        onClicked: Qt.openUrlExternally("http://ocnews.buschmann23.de")
    }

    Button {
        id: licenseLink
        anchors { horizontalCenter: parent.horizontalCenter; top: websiteLink.bottom; topMargin: 10 }
        text: qsTr("Donate")
        onClicked: Qt.openUrlExternally("https://www.paypal.com/cgi-bin/webscr?cmd=_s-xclick&hosted_button_id=RDZAG64WD34PL")
    }


    PrivacySheet {
        id: aboutPrivacySheet
    }

}
