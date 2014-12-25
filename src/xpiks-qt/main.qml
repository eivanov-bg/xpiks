/*
 * This file is a part of Xpiks - cross platform application for
 * keywording and uploading images for microstocks
 * Copyright (C) 2014 Taras Kushnir <kushnirTV@gmail.com>
 *
 * Xpiks is distributed under the GNU General Public License, version 3.0
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

import QtQuick 2.4
import QtQuick.Dialogs 1.2
import QtQuick.Controls 1.3
import QtQuick.Controls.Styles 1.2
import QtQuick.Layouts 1.1
import xpiks 1.0
import "Constants"
import "Constants/Colors.js" as Colors
import "Components"
import "StyledControls"
import "Dialogs"

ApplicationWindow {
    id: applicationWindow
    visible: true
    width: 900
    height: 640
    minimumHeight: 640
    minimumWidth: 900
    title: qsTr("Xpiks")

    onClosing: {
        if (artItemsModel.modifiedArtworksCount > 0) {
            close.accepted = false
            configExitDialog.open()
        }
    }

    menuBar: MenuBar {
        Menu {
            title: qsTr("File")
            MenuItem {
                text: qsTr("&Settings")
                onTriggered: {
                    console.log("Settings action triggered");
                    var component = Qt.createComponent("Dialogs/SettingsWindow.qml");
                    var window = component.createObject(applicationWindow);
                    window.show();
                }
            }
            MenuItem {
                text: qsTr("Exit")
                onTriggered: Qt.quit();
            }
        }
    }

    MessageDialog {
        id: configExitDialog

        title: "Confirmation"
        text: qsTr("You have some artworks modified. Really exit?")
        standardButtons: StandardButton.Yes | StandardButton.No
        onYes: {
            Qt.quit()
        }
    }

    MessageDialog {
        id: confirmRemoveSelectedDialog
        property int itemsCount
        title: "Confirmation"
        text: qsTr("Are you sure you want to remove %1 item(s)?").arg(itemsCount)
        standardButtons: StandardButton.Yes | StandardButton.No
        onYes: {
            artItemsModel.removeSelectedArtworks()
        }
    }

    MessageDialog {
        id: confirmRemoveDirectoryDialog
        property int directoryIndex
        title: "Confirmation"
        text: qsTr("Are you sure you want to remove this directory?")
        standardButtons: StandardButton.Yes | StandardButton.No
        onYes: {
            artItemsModel.removeArtworksDirectory(directoryIndex)
        }
    }

    FileDialog {
        id: chooseArtworksDialog
        title: "Please choose artworks"
        selectExisting: true
        selectMultiple: true
        nameFilters: [ "Jpeg images (*.jpg), All files (*)" ]

        onAccepted: {
            console.log("You chose: " + chooseArtworksDialog.fileUrls)
            artItemsModel.addLocalArtworks(chooseArtworksDialog.fileUrls)
            Qt.createComponent("Dialogs/ImportMetadata.qml").createObject(applicationWindow, {})
        }

        onRejected: {
            console.log("Artworks dialog canceled")
        }
    }

    FileDialog {
        id: chooseDirectoryDialog
        title: "Please choose artworks"
        selectExisting: true
        selectMultiple: false
        selectFolder: true

        onAccepted: {
            console.log("You chose: " + chooseDirectoryDialog.fileUrl)
            artItemsModel.addLocalDirectory(chooseDirectoryDialog.fileUrl)
            Qt.createComponent("Dialogs/ImportMetadata.qml").createObject(applicationWindow, {})
        }

        onRejected: {
            console.log("Directory dialog canceled")
        }
    }

    MessageDialog {
        id: mustSaveWarning
        title: "Warning"
        text: qsTr("Please, save selected items before upload")
    }

    MessageDialog {
        id: mustSelectDialog
        title: "Warning"
        text: qsTr("Please, select some artworks first")
    }

    Rectangle {
        color: Colors.defaultDarkColor
        anchors.fill: parent

        SplitView {
            id: mainGrid
            anchors.fill: parent
            orientation: Qt.Horizontal

            handleDelegate: Rectangle {
                height: parent.height
                width: 2
                color: Colors.defaultDarkColor
            }

            ColumnLayout {
                Layout.minimumWidth: 250
                Layout.preferredWidth: 250
                Layout.maximumWidth: 350

                spacing: 0

                Rectangle {
                    width: parent.width
                    height: 45
                    color: Colors.defaultDarkColor

                    RowLayout {
                        spacing: 10
                        anchors.fill: parent
                        anchors.margins: { top: 10; left: 10 }

                        StyledButton {
                            text: qsTr("Add directory")
                            width: 120
                            onClicked: chooseDirectoryDialog.open()
                        }

                        StyledButton {
                            text: qsTr("Add files")
                            width: 100
                            onClicked: chooseArtworksDialog.open()
                        }

                        Item {
                            Layout.fillWidth: true
                        }
                    }
                }

                Rectangle {
                    Layout.fillHeight: true
                    Layout.fillWidth: true
                    Layout.minimumWidth: 250
                    Layout.maximumWidth: 350

                    color: Colors.defaultControlColor

                    ListView {
                        id: sourcesListView
                        model: artworkRepository
                        boundsBehavior: Flickable.StopAtBounds
                        anchors.fill: parent
                        anchors.margins: { left: 10; top: 5; right: 10 }

                        spacing: 10

                        delegate: Rectangle {
                            id: sourceWrapper
                            property int indexOfThisDelegate: index
                            color: isselected ? Colors.itemsSourceSelected : Colors.itemsSourceBackground
                            width: parent.width
                            height: 31
                            Layout.minimumWidth: 237

                            MouseArea {
                                anchors.fill: parent
                                onClicked: {
                                    artItemsModel.selectDirectory(sourceWrapper.indexOfThisDelegate)
                                }
                            }

                            RowLayout {
                                spacing: 10
                                anchors.fill: parent

                                Item {
                                    id: placeholder1
                                    width: 1
                                }

                                StyledText {
                                    id: directoryPath
                                    Layout.fillWidth: true
                                    anchors.verticalCenter: parent.verticalCenter
                                    height: 31
                                    color: Colors.itemsSourceForeground
                                    text: path + " (" + usedimagescount + ")"
                                    elide: Text.ElideMiddle
                                    font.bold: true
                                }

                                CloseIcon {
                                    width: 14
                                    height: 14
                                    anchors.verticalCenter: directoryPath.verticalCenter
                                    anchors.verticalCenterOffset: 1
                                    isActive: false

                                    onItemClicked: {
                                        confirmRemoveDirectoryDialog.directoryIndex = sourceWrapper.indexOfThisDelegate
                                        confirmRemoveDirectoryDialog.open()
                                    }
                                }

                                Item {
                                    id: placeholder2
                                    width: 1
                                }
                            }
                        }
                    }
                }
            }

            ColumnLayout {
                spacing: 0

                Rectangle {
                    height: 45
                    color: Colors.defaultDarkColor
                    z: 10000
                    Layout.fillWidth: true

                    RowLayout {
                        spacing: 10
                        anchors.fill: parent
                        anchors.margins: { top: 10; left: 10; right: 10 }
                        enabled: artworkRepository.artworksSourcesCount > 0

                        Item {
                            width: 1
                        }

                        StyledCheckbox {
                            id: selectAllCheckbox
                            text: artItemsModel.selectedArtworksCount == 0 ? qsTr("Select all") : qsTr("Select none")
                            checked: artItemsModel.selectedArtworksCount > 0

                            onClicked: {
                                if (checked) {
                                    artItemsModel.selectAllArtworks();
                                }
                                else {
                                    artItemsModel.unselectAllArtworks();
                                }
                            }

                            Connections {
                                target: artItemsModel
                                onSelectedArtworksCountChanged: {
                                    selectAllCheckbox.checked = artItemsModel.selectedArtworksCount > 0
                                }
                            }
                        }

                        Item {
                            Layout.fillWidth: true
                        }

                        StyledButton {
                            text: qsTr("Remove")
                            width: 90
                            onClicked: {
                                if (artItemsModel.selectedArtworksCount == 0) {
                                    mustSelectDialog.open()
                                }
                                else {
                                    var itemsCount = artItemsModel.selectedArtworksCount
                                    if (itemsCount > 0) {
                                        confirmRemoveSelectedDialog.itemsCount = itemsCount
                                        confirmRemoveSelectedDialog.open()
                                    }
                                }
                            }
                        }

                        StyledButton {
                            text: qsTr("Edit")
                            width: 80
                            onClicked: {
                                if (artItemsModel.selectedArtworksCount == 0) {
                                    mustSelectDialog.open()
                                }
                                else {
                                    if (artItemsModel.selectedArtworksCount > 0) {
                                        combinedArtworks.resetModelData();
                                        artItemsModel.combineSelectedArtworks();
                                        Qt.createComponent("Dialogs/CombinedArtworksDialog.qml").createObject(applicationWindow, {});
                                    }
                                }
                            }
                        }

                        StyledButton {
                            text: qsTr("Save")
                            width: 80
                            onClicked: {
                                if (artItemsModel.selectedArtworksCount == 0) {
                                    mustSelectDialog.open()
                                }
                                else {
                                    if (artItemsModel.selectedArtworksCount > 0) {
                                        iptcProvider.resetModel()
                                        artItemsModel.patchSelectedArtworks()
                                        Qt.createComponent("Dialogs/ExportMetadata.qml").createObject(applicationWindow, {})
                                    }
                                }
                            }
                        }

                        StyledButton {
                            text: qsTr("Upload")
                            width: 90
                            onClicked: {
                                if (artItemsModel.selectedArtworksCount == 0) {
                                    mustSelectDialog.open()
                                }
                                else {
                                    if (artItemsModel.areSelectedArtworksSaved()) {
                                        artworkUploader.resetModel()
                                        artItemsModel.uploadSelectedArtworks()
                                        Qt.createComponent("Dialogs/UploadArtworks.qml").createObject(applicationWindow, {})
                                    } else {
                                        mustSaveWarning.open()
                                    }
                                }
                            }
                        }

                        Item {
                            width: 10
                        }
                    }
                }

                Rectangle {
                    Layout.fillWidth: true
                    Layout.fillHeight: true
                    color: Colors.defaultControlColor

                    StyledScrollView {
                        anchors.fill: parent
                        anchors.topMargin: 5

                        ListView {
                            id: imagesListView
                            model: artItemsModel
                            boundsBehavior: Flickable.StopAtBounds
                            spacing: 4

                            delegate: Rectangle {
                                id: rowWrapper
                                property bool isHighlighted: (isselected || descriptionTextInput.activeFocus || flv.isFocused)
                                color: isHighlighted ? Colors.selectedArtworkColor : Colors.artworkImageBackground
                                property int indexOfThisDelegate: index

                                width: parent.width
                                height: 200

                                RowLayout {
                                    anchors.fill: parent
                                    anchors.rightMargin: 10
                                    spacing: 5

                                    Rectangle {
                                        id: isModifiedRectangle
                                        color: ismodified ? Colors.artworkModifiedColor : Colors.artworkSavedColor
                                        width: 6
                                        Layout.fillHeight: true
                                    }

                                    Item {
                                        width: 5
                                    }

                                    StyledCheckbox {
                                        id: itemCheckedCheckbox
                                        //checked: isselected
                                        onClicked: editisselected = checked
                                        Component.onCompleted: itemCheckedCheckbox.checked = isselected
                                        Connections {
                                            target: artItemsModel
                                            onSelectedArtworksCountChanged: {
                                                itemCheckedCheckbox.checked = isselected
                                            }
                                        }
                                    }

                                    Rectangle {
                                        width: 180
                                        height: parent.height
                                        color: "transparent"

                                        ColumnLayout {
                                            anchors.fill: parent
                                            anchors.margins: { left: 15; right: 15 }
                                            spacing: 5

                                            Item {
                                                height: 8
                                            }

                                            Rectangle {
                                                width: 150
                                                Layout.minimumHeight: 100
                                                Layout.maximumHeight: 150
                                                anchors.horizontalCenter: parent.horizontalCenter
                                                color: "transparent"
                                                Image {
                                                    anchors.fill: parent
                                                    source: "image://global/" + filename
                                                    sourceSize.width: 150
                                                    sourceSize.height: 150
                                                    fillMode: Image.PreserveAspectFit
                                                    asynchronous: true
                                                }
                                            }

                                            Item {
                                                height: 5
                                            }

                                            StyledText {
                                                Layout.fillWidth: true
                                                elide: Text.ElideMiddle
                                                horizontalAlignment: Text.AlignHCenter
                                                text: filename.split(/[\\/]/).pop()
                                            }

                                            Item {
                                                Layout.fillHeight: true
                                            }
                                        }
                                    }

                                    Rectangle {
                                        id: columnRectangle
                                        height: parent.height
                                        Layout.fillWidth: true
                                        color: rowWrapper.isHighlighted  ? Colors.selectedMetadataColor : Colors.artworkBackground

                                        ColumnLayout {
                                            id: columnLayout
                                            spacing: 3
                                            anchors.fill: parent
                                            anchors.margins: { left: 20; right: 20 }

                                            StyledText {
                                                text: qsTr("Description:")
                                                anchors.left: parent.left
                                            }

                                            Rectangle {
                                                id: rect
                                                Layout.fillWidth: true
                                                height: 30
                                                color: rowWrapper.isHighlighted ? Colors.defaultInputBackground : Colors.defaultControlColor
                                                border.color: Colors.artworkActiveColor
                                                border.width: descriptionTextInput.activeFocus ? 1 : 0

                                                StyledTextInput {
                                                    id: descriptionTextInput
                                                    anchors.left: parent.left
                                                    anchors.right: parent.right
                                                    anchors.leftMargin: 5
                                                    anchors.rightMargin: 5
                                                    anchors.verticalCenter: parent.verticalCenter
                                                    maximumLength: 250
                                                    text: description
                                                    color: rowWrapper.isHighlighted ? Colors.defaultLightColor : Colors.artworkActiveColor
                                                    font.family: "Helvetica"
                                                    font.pixelSize: 12
                                                    onTextChanged: model.editdescription = text
                                                    Keys.onTabPressed: {
                                                        flv.activateEdit()
                                                    }
                                                }
                                            }

                                            Item {
                                                height: 1
                                            }

                                            RowLayout {
                                                anchors.left: parent.left
                                                spacing: 5

                                                StyledText {
                                                    id: keywordsLabel
                                                    text: qsTr("Keywords:")
                                                }

                                                StyledText {
                                                    text: qsTr("(comma-separated)")
                                                    visible: rowWrapper.isHighlighted
                                                    color: Colors.defaultInputBackground
                                                }
                                            }

                                            Rectangle {
                                                id: keywordsWrapper
                                                Layout.fillWidth: true
                                                height: 80
                                                anchors.rightMargin: 20
                                                border.color: Colors.artworkActiveColor
                                                border.width: flv.isFocused ? 1 : 0
                                                color: rowWrapper.isHighlighted ? Colors.defaultInputBackground : Colors.defaultControlColor


                                                function removeKeyword(index) {
                                                    artItemsModel.removeKeywordAt(rowWrapper.indexOfThisDelegate, index)
                                                }

                                                function removeLastKeyword() {
                                                    artItemsModel.removeLastKeyword(rowWrapper.indexOfThisDelegate)
                                                }

                                                function appendKeyword(keyword) {
                                                    artItemsModel.appendKeyword(rowWrapper.indexOfThisDelegate, keyword)
                                                }

                                                MouseArea {
                                                    anchors.fill: parent
                                                    propagateComposedEvents: true
                                                    onClicked: {
                                                        flv.activateEdit()
                                                        mouse.accepted = false
                                                    }
                                                }

                                                StyledScrollView {
                                                    id: scroller
                                                    height: parent.height
                                                    width: parent.width + 15
                                                    highlightOnFocus: true

                                                    EditableTags {
                                                        id: flv
                                                        model: keywords
                                                        anchors.margins: { left: 5; top: 5; right: 0; bottom: 5 }

                                                        delegate: Rectangle {
                                                            id: itemWrapper
                                                            property int indexOfThisDelegate: index
                                                            property string keyword: modelData
                                                            color: rowWrapper.isHighlighted ? Colors.defaultLightColor : Colors.artworkActiveColor

                                                            width: childrenRect.width
                                                            height: childrenRect.height

                                                            RowLayout {
                                                                spacing: 1

                                                                Rectangle {
                                                                    id: tagTextRect
                                                                    width: childrenRect.width + 5
                                                                    height: 20
                                                                    color: "transparent"

                                                                    StyledText {
                                                                        anchors.left: parent.left
                                                                        anchors.leftMargin: 5
                                                                        anchors.top: parent.top
                                                                        anchors.bottom: parent.bottom
                                                                        verticalAlignment: Text.AlignVCenter
                                                                        text: modelData
                                                                        color: rowWrapper.isHighlighted ? Colors.defaultControlColor : Colors.defaultLightColor
                                                                    }
                                                                }

                                                                CloseIcon {
                                                                    width: 14
                                                                    height: 14
                                                                    isActive: rowWrapper.isHighlighted
                                                                    anchors.verticalCenter: tagTextRect.verticalCenter
                                                                    anchors.verticalCenterOffset: 1
                                                                    onItemClicked: keywordsWrapper.removeKeyword(itemWrapper.indexOfThisDelegate)
                                                                }

                                                                Item {
                                                                    width: 1
                                                                }
                                                            }
                                                        }

                                                        onTagAdded: {
                                                            keywordsWrapper.appendKeyword(text)
                                                        }

                                                        onRemoveLast: {
                                                            keywordsWrapper.removeLastKeyword()
                                                        }
                                                    }
                                                }
                                            }

                                            RowLayout {
                                                spacing: 15

                                                StyledText {
                                                    text: keywordscount
                                                    color: rowWrapper.isHighlighted ? Colors.defaultControlColor : Colors.selectedArtworkColor
                                                }

                                                Item {
                                                    Layout.fillWidth: true
                                                }

                                                StyledText {
                                                    text: qsTr("More Edits")
                                                    color: Colors.artworkActiveColor

                                                    MouseArea {
                                                        anchors.fill: parent
                                                        cursorShape: Qt.PointingHandCursor
                                                        onClicked: {
                                                            combinedArtworks.resetModelData();
                                                            artItemsModel.combineArtwork(rowWrapper.indexOfThisDelegate);
                                                            Qt.createComponent("Dialogs/CombinedArtworksDialog.qml").createObject(applicationWindow, {});
                                                        }
                                                    }
                                                }

                                                StyledText {
                                                    text: qsTr("Copy keywords")
                                                    color: Colors.artworkActiveColor

                                                    MouseArea {
                                                        anchors.fill: parent
                                                        cursorShape: Qt.PointingHandCursor
                                                        onClicked: clipboard.setText(keywordsstring)
                                                    }
                                                }
                                            }

                                            Item {
                                                height: 1
                                            }
                                        }
                                    }
                                }
                            }
                        }
                    }
                }

                ClipboardHelper {
                    id: clipboard
                }
            }
        }
    }

    statusBar: StatusBar {
        RowLayout {
            width: parent.width
            height: 20
            implicitHeight: 20
            spacing: 5

            Item {
                Layout.fillWidth: true
            }

            StyledText {
                text: qsTr("(%1) item(s) selected").arg(artItemsModel.selectedArtworksCount)
                color: Colors.selectedMetadataColor
                verticalAlignment: Text.AlignVCenter
            }

            StyledText {
                text: "|"
                color: Colors.selectedMetadataColor
                verticalAlignment: Text.AlignVCenter
            }

            StyledText {
                text: qsTr("(%1) item(s) modified").arg(artItemsModel.modifiedArtworksCount)
                verticalAlignment: Text.AlignVCenter
                color: artItemsModel.modifiedArtworksCount > 0 ? Colors.artworkModifiedColor : Colors.selectedMetadataColor
            }

            Item {
                width: 20
            }
        }

        style: StatusBarStyle {
            background: Rectangle {
                implicitHeight: 20
                implicitWidth: 200
                color: Colors.defaultDarkColor
            }
        }
    }
}
