/* Copyright (C) 2013 Martin Grimme <martin.grimme _AT_ gmail.com>
*
* This file was apapted from Tidings
* Copyright (C) 2013 Martin Grimme <martin.grimme _AT_ gmail.com>
*
* This program is free software; you can redistribute it and/or modify
* it under the terms of the GNU General Public License as published by
* the Free Software Foundation; either version 2 of the License, or
* (at your option) any later version.
*
* This program is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
* GNU General Public License for more details.
*
* You should have received a copy of the GNU General Public License
* along with this program; if not, write to the Free Software
* Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA 02111-1307 USA
*/

import QtQuick 1.1

/* Pretty fancy element for displaying rich text fitting the width.
 *
 * Images are scaled down to fit the width, or, technically speaking, the
 * rich text content is actually scaled down so the images fit, while the
 * font size is scaled up to keep the original font size.
 */

Item {
    id: root

    property string text
    property string scaledText
    property color color
    property real fontSize: 17

    property string _RICHTEXT_STYLESHEET_PREAMBLE: "<html><head><meta http-equiv=\"Content-Type\" content=\"text/html; charset=UTF-8\"><style>a { text-decoration: none; color: '" + theme.selectionColor + "' }</style></head><body>";
    property string _RICHTEXT_STYLESHEET_APPENDIX: "</body></html>";

    property real scaling: 1

    signal linkActivated(string link)

    height: contentText.height * scaling
    clip: true

    onWidthChanged: {
        if (!rescaleTimer.running)
            rescaleTimer.restart();
    }

    onTextChanged: {
        if (!rescaleTimer.running)
            rescaleTimer.restart();
    }



    Text {
        id: layoutText
        visible: false
        width: parent.width
        wrapMode: Text.WrapAtWordBoundaryOrAnywhere
        textFormat: config.useRichText ? Text.RichText : Text.StyledText

        text: config.useRichText ? "<style>* { font-size: 1px }</style>" + parent.text : parent.text

        onPaintedWidthChanged: {
            console.log("contentWidth: " + paintedWidth)
            rescaleTimer.restart()
        }

        onTextChanged: rescaleTimer.restart()
    }

    Loader {
        id: contentText
        sourceComponent: rescaleTimer.running ? null : textComponent
    }

    Component {
        id: textComponent
        Text {
            width: root.width / scaling
            scale: scaling

            transformOrigin: Item.TopLeft
            font.pointSize: root.fontSize / scaling
            color: root.color
            wrapMode: Text.WrapAtWordBoundaryOrAnywhere
            textFormat: config.useRichText ? Text.RichText : Text.StyledText
            smooth: true
            font.weight: Font.Light

    //        text: useRichText ? _RICHTEXT_STYLESHEET_PREAMBLE + parent.text + _RICHTEXT_STYLESHEET_APPENDIX : parent.text
            text: config.useRichText ? _RICHTEXT_STYLESHEET_PREAMBLE + root.scaledText + _RICHTEXT_STYLESHEET_APPENDIX : root.scaledText

            onLinkActivated: {
                root.linkActivated(link);
            }
        }
    }

    Timer {
        id: rescaleTimer
        interval: 100

        onTriggered: {
            var paintedWidth = Math.floor(layoutText.paintedWidth);
            scaling = Math.min(1, parent.width / (layoutText.paintedWidth + 0.0));
            console.log("scaling: " + scaling);

            root.scaledText = root.text
        }
    }
}
