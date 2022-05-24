﻿

/*
This is a UI file (.ui.qml) that is intended to be edited in Qt Design Studio only.
It is supposed to be strictly declarative and only uses a subset of QML. If you edit
this file manually, you might introduce QML code that is not supported by Qt Design Studio.
Check out https://doc.qt.io/qtcreator/creator-quick-ui-forms.html for details on .ui.qml files.
*/
import QtQuick
import QtQuick.Controls
import QtQuick.Window 2.15
import QtQuick.Layouts 1.15
import QtQuick.Dialogs
import LLM
import VideoShow 1.0

Rectangle {
    id:"window"
    width: Constants.width
    height: Constants.height
    anchors.fill: parent

    property real multiplierH: (window.height/640)*1000;
    property real multiplierW: (window.width/360)*1000;

    function dpH(numbers) {
       return numbers*multiplierH/10000;
    }
    function dpW(numbers) {
       return numbers*multiplierW/10000;
    }
    function dpX(numbers){
        return (dpW(numbers)+dpH(numbers))/2;
    }

    Image {
        visible: !leftList.open
        z: 2
        width: 40
        height: 40
        anchors.left: parent.left
        anchors.top: parent.top
        source: "images/leftarrow(1).png"
        anchors.leftMargin: 15
        anchors.topMargin: 14
        anchors.rightMargin: 78
        rotation: 180
        MouseArea {
            anchors.fill: parent
            onClicked: {
                leftList.open = true
            }
        }
    }
    Rectangle {
        id: leftList
        anchors.left: parent.left
        anchors.top: parent.top
        anchors.bottom: parent.bottom
        width: open ? 358 : 0
        color: '#F8F8F8'
        property bool open: true
        Behavior on width {
            RotationAnimation {
                duration: 500
                easing.type: Easing.OutBack
            }
        }
        Column {
            anchors.fill: parent
            Item {
                id: item1
                width: parent.width
                height: 60
                Text {
                    color: "#707070"
                    text: '媒体库'
                    anchors.verticalCenter: parent.verticalCenter
                    anchors.left: parent.left
                    font.pixelSize: 30
                    anchors.leftMargin: 40
                }

                Image {
                    x: 155
                    anchors.verticalCenter: parent.verticalCenter
                    source: "images/dir(1).png"
                    MouseArea {
                        anchors.fill: parent
                        onClicked: {
                            fileDialog.open()
                        }
                    }

                    FileDialog {
                        id: fileDialog
                        title: "导入音频或者视频文件"
                        nameFilters: ["视频文件 (*.ts *.mp4 *.avi *.flv *.mkv *.3gp)", "音频文件 (*.mp3 *.ogg *.wav *.wma *.ape *.ra)"]
                        acceptLabel: "确定"
                        rejectLabel: "取消"
                        fileMode: FileDialog.OpenFile
                    }
                }

                Image {
                    width: 40
                    height: 40
                    anchors.verticalCenter: parent.verticalCenter
                    anchors.right: parent.right
                    source: "images/leftarrow(1).png"
                    anchors.rightMargin: 23
                    MouseArea {
                        anchors.fill: parent
                        onClicked: {
                            leftList.open = false
                        }
                    }
                }
            }
            Item {
                id: yinpin
                width: parent.width
                property int count: 2
                height: 60 + count * (40 + 12) - 12
                clip: true
                Behavior on height {
                    RotationAnimation {
                        duration: 500
                        easing.type: Easing.OutBack
                    }
                }

                Item {
                    anchors.left: parent.left
                    anchors.right: parent.right
                    anchors.top: parent.top
                    height: 60
                    Rectangle {
                        width: 14
                        height: 40
                        color: '#22B61D'
                        anchors.verticalCenter: parent.verticalCenter
                        anchors.left: parent.left
                        anchors.leftMargin: 0
                    }
                    Text {
                        color: "#707070"
                        text: '音频'
                        anchors.verticalCenter: parent.verticalCenter
                        anchors.left: parent.left
                        font.pixelSize: 25
                        anchors.verticalCenterOffset: 0
                        anchors.leftMargin: 30
                    }
                    Image {
                        id: yinpinBtn
                        width: 40
                        height: 40
                        anchors.verticalCenter: parent.verticalCenter
                        anchors.right: parent.right
                        source: "images/leftarrow.png"
                        anchors.rightMargin: 23
                        rotation: yinpinBtn.opened ? 0 : 180
                        property bool opened: true
                        MouseArea {
                            anchors.fill: parent
                            onClicked: {
                                if (yinpinBtn.opened)
                                    yinpin.height = 60
                                else
                                    yinpin.height = 60 + yinpin.count * (40 + 12) - 12
                                yinpinBtn.opened = !yinpinBtn.opened
                            }
                        }
                    }
                }
                Column {
                    anchors.left: parent.left
                    anchors.right: parent.right
                    anchors.top: parent.top
                    anchors.topMargin: 60
                    spacing: 12
                    Repeater {
                        model: 2
                        Item {
                            anchors.leftMargin: 15
                            anchors.left: parent.left
                            anchors.right: parent.right
                            height: 40
                            Column {
                                spacing: 0
                                Text {
                                    color: "#707070"
                                    text: 'Feel my rhythm'
                                    font.pixelSize: 18
                                }
                                Text {
                                    color: "#b6b6b6"
                                    font.pixelSize: 13
                                    text: 'Red Velvet'
                                }
                            }
                            Image {
                                anchors.verticalCenter: parent.verticalCenter
                                anchors.right: parent.right
                                source: "images/more(2).png"
                                anchors.rightMargin: 28
                                MouseArea {
                                    anchors.fill: parent
                                    acceptedButtons: Qt.LeftButton | Qt.RightButton
                                    onClicked: {
                                        if (mouse.button === Qt.LeftButton)
                                            contextMenu.popup()
                                    }
                                    onPressAndHold: {
                                        if (mouse.source === Qt.MouseEventNotSynthesized)
                                            contextMenu.popup()
                                    }
                                }
                                SubWindow {
                                    id: subWindow
                                }
                                Setting1 {
                                    id: setting_for_music
                                }

                                Menu {
                                    id: contextMenu
                                    MenuItem {
                                        text: '置顶'
                                    }
                                    MenuItem {
                                        text: '设置'
                                        onTriggered: {
                                            setting_for_music.show()
                                        }
                                    }
                                    MenuItem {
                                        text: '详细信息'
                                        onTriggered: {
                                            subWindow.show()
                                        }
                                    }
                                }
                            }
                        }
                    }
                }
            }
            Item {
                id: shipin
                width: parent.width
                height: 60 + count * (40 + 12) - 12
                clip: true
                property int count: 3
                Behavior on height {
                    RotationAnimation {
                        duration: 500
                        easing.type: Easing.OutBack
                    }
                }
                Item {
                    anchors.left: parent.left
                    anchors.right: parent.right
                    anchors.top: parent.top
                    height: 60
                    Rectangle {
                        width: 14
                        height: 40
                        color: '#22B61D'
                        anchors.verticalCenter: parent.verticalCenter
                        anchors.left: parent.left
                        anchors.leftMargin: 0
                    }
                    Text {
                        color: "#707070"
                        text: '视频'
                        anchors.verticalCenter: parent.verticalCenter
                        anchors.left: parent.left
                        font.pixelSize: 25
                        anchors.verticalCenterOffset: 0
                        anchors.leftMargin: 30
                    }
                    Image {
                        id: shipinBtn
                        width: 40
                        height: 40
                        anchors.verticalCenter: parent.verticalCenter
                        anchors.right: parent.right
                        source: "images/leftarrow.png"
                        anchors.rightMargin: 23
                        rotation: shipinBtn.opened ? 0 : 180
                        property bool opened: true
                        MouseArea {
                            anchors.fill: parent
                            onClicked: {
                                if (shipinBtn.opened)
                                    shipin.height = 60
                                else
                                    shipin.height = 60 + shipin.count * (40 + 12) - 12
                                shipinBtn.opened = !shipinBtn.opened
                            }
                        }
                    }
                }
                Column {
                    anchors.left: parent.left
                    anchors.right: parent.right
                    anchors.top: parent.top
                    anchors.topMargin: 60
                    spacing: 12
                    Repeater {
                        model: 3
                        Item {
                            anchors.leftMargin: 15
                            anchors.left: parent.left
                            anchors.right: parent.right
                            height: 40
                            Image {
                                source: "images/group_1.png"
                            }

                            Column {
                                anchors.left: parent.left
                                anchors.leftMargin: 62
                                spacing: 0
                                Text {
                                    color: "#707070"
                                    text: '202204141315'
                                    font.pixelSize: 18
                                }
                                Text {
                                    color: "#b6b6b6"
                                    font.pixelSize: 13
                                    text: '5:10:00'
                                }
                            }
                            Image {
                                anchors.verticalCenter: parent.verticalCenter
                                anchors.right: parent.right
                                source: "images/more(2).png"
                                anchors.rightMargin: 28
                                MouseArea {
                                    anchors.fill: parent
                                    acceptedButtons: Qt.LeftButton | Qt.RightButton
                                    onClicked: {
                                        if (mouse.button === Qt.LeftButton)
                                            contextMenu3.popup()
                                    }
                                    onPressAndHold: {
                                        if (mouse.source === Qt.MouseEventNotSynthesized)
                                            contextMenu3.popup()
                                    }
                                }
                                SubWindow {
                                    id: subWindow3
                                }
                                Setting1 {
                                    id: setting_for_video
                                }

                                Menu {
                                    id: contextMenu3
                                    MenuItem {
                                        text: '置顶'
                                    }
                                    MenuItem {
                                        text: '设置'
                                        onTriggered: {
                                            setting_for_video.show()
                                        }
                                    }
                                    MenuItem {
                                        text: '倒放'
                                    }
                                    MenuItem {
                                        text: '详细信息'
                                        onTriggered: {
                                            subWindow3.show()
                                        }
                                    }
                                }
                            }
                        }
                    }
                }
            }
        }
    }
    Item {
        id: main
        anchors.left: leftList.right
        anchors.right: parent.right
        anchors.top: parent.top
        anchors.bottom: parent.bottom
        anchors.rightMargin: 0
        anchors.bottomMargin: 0
        anchors.topMargin: 0
        anchors.leftMargin: 0

        SubWindow {
            id: subWindow6
        }

        Introduction {
            id: subWindow5
        }

        Menu {
            id: contextMenu5

            MenuItem {
                text: '设置'
                onTriggered: {
                    setting_for_rb.show()
                }
            }
            MenuItem {
                text: '详细信息'
                onTriggered: {
                    subWindow6.show()
                }
            }
            MenuItem {
                text: '使用帮助'
                onTriggered: {
                    subWindow5.show()
                }
            }
        }

        MouseArea {
            anchors.fill: parent
            acceptedButtons: Qt.LeftButton | Qt.RightButton
            onClicked: {
                if (mouse.button === Qt.RightButton)
                    contextMenu5.popup()
            }
            onPressAndHold: {
                if (mouse.source === Qt.MouseEventNotSynthesized)
                    contextMenu5.popup()
            }
            Setting1 {
                id: setting_for_rb
            }
        }
//        Image {
//            anchors.fill: parent
//            source: "images/basepic.png"
//        }
        Rectangle{
//            anchors.fill: parent
            width: dpW(3000)
            height: dpH(5000)
            border.width: 5
            border.color: "black"
            radius: 10
            VideoShow{
                id: videoShow;
                anchors.centerIn: parent;
                nWidth: dpW(3000);
                nHeight: dpH(5000);
                width: dpW(3000);
                height: dpH(5000);
            }
        }
        Image {
            anchors.right: parent.right
            anchors.top: parent.top
            source: "images/more1.png"
            anchors.rightMargin: 65
            anchors.topMargin: 66
            MouseArea {
                anchors.fill: parent
                acceptedButtons: Qt.LeftButton | Qt.RightButton
                onClicked: {
                    if (mouse.button === Qt.LeftButton)
                        contextMenu2.popup()
                }
                onPressAndHold: {
                    if (mouse.source === Qt.MouseEventNotSynthesized)
                        contextMenu2.popup()
                }
            }
            Setting1 {
                id: setting_for_rup
            }

            SubWindow {
                id: subWindow2
            }

            Menu {
                id: contextMenu2
                MenuItem {
                    text: '置顶'
                }
                MenuItem {
                    text: '设置'
                    onTriggered: {
                        setting_for_rup.show()
                    }
                }
                MenuItem {
                    text: '详细信息'
                    onTriggered: {
                        subWindow2.show()
                    }
                }
            }
        }

        RowLayout {
            anchors.left: parent.left
            anchors.right: parent.right
            anchors.bottom: parent.bottom
            anchors.rightMargin: 40
            anchors.leftMargin: 40
            anchors.bottomMargin: 56
            Image {
                source: "images/M_left.png"
                RoundButton {
                    anchors.fill: parent
                    flat: true
                    ToolTip.visible: hovered
                    ToolTip.text: qsTr("上一集")
                }
            }
            Image {
                Layout.preferredHeight: 80
                Layout.preferredWidth: 80
                source: "../content/images/13.png"
                RoundButton {
                    anchors.fill: parent
                    flat: true
                    ToolTip.visible: hovered
                    ToolTip.text: qsTr("上一帧")
                }
            }
            Image {
                source: "../content/images/play.png"
                RoundButton {
                    anchors.fill: parent
                    flat: true
                    ToolTip.visible: hovered
                    ToolTip.text: qsTr("播放")
                }
            }
            Image {
                Layout.preferredHeight: 80
                Layout.preferredWidth: 80
                source: "../content/images/12.png"
                RoundButton {
                    anchors.fill: parent
                    flat: true
                    ToolTip.visible: hovered
                    ToolTip.text: qsTr("下一帧")
                }
            }
            Image {
                source: "images/M_right.png"
                RoundButton {
                    anchors.fill: parent
                    flat: true
                    ToolTip.visible: hovered
                    ToolTip.text: qsTr("下一集")
                }
            }
            Text {
                color: "#ffffff"
                text: '00:00'
                anchors.verticalCenter: parent.verticalCenter
                font.pixelSize: 30
            }
            //            Slider {
            //                height: 6
            //                Layout.rightMargin: 30
            //                Layout.leftMargin: 30
            //                Layout.fillWidth: true
            //                value: 0.5
            //            }
            ///// 自定义美化样式，请同学们自己放开注释
            Slider {
                id: control
                value: 0.5
                //                anchors.centerIn: parent
                //                width: 200
                //                height: 20
                height: 20
                Layout.rightMargin: 30
                Layout.leftMargin: 60
                Layout.fillWidth: true
                background: Rectangle {
                    id: rect1
                    width: control.availableWidth
                    height: 20
                    radius: 7
                    color: "orange"

                    Rectangle {
                        id: rect2
                        width: control.visualPosition * rect1.width
                        height: rect1.height
                        color: "yellow"
                        radius: 7
                    }
                }

                handle: Rectangle {

                    id: handle111
                    x: control.visualPosition * (control.availableWidth - implicitWidth)
                    y: control.availableHeight / 2 - implicitHeight / 2
                    implicitWidth: 20
                    implicitHeight: 26
                    radius: 13
                    color: control.pressed ? "green" : "white"
                    border.color: "black"
                }

                Rectangle {
                    visible: control.pressed

                    id: yulan
                    width: 150
                    height: 100
                    x: handle111.x
                    y: handle111.y - 110
                }
                onValueChanged: {
                    console.log(control.visualPosition)
                }
            }
            Text {
                color: "#ffffff"
                text: '5:12'
                anchors.verticalCenter: parent.verticalCenter
                font.pixelSize: 30
                Layout.rightMargin: 30
            }
            Image {
                Layout.preferredHeight: 60
                Layout.preferredWidth: 60
                source: "images/full_screen(1).png"
                Layout.rightMargin: 12
                MouseArea {
                    anchors.fill: parent
                    onClicked: {

                    }
                }
            }
            Image {
                Layout.preferredHeight: 60
                Layout.preferredWidth: 60
                source: "images/11.png"
                Layout.rightMargin: 12
                Slider {
                    visible: false
                    id: voice
                    y: -130
                    anchors.horizontalCenter: parent.horizontalCenter
                    value: 0.5
                    rotation: 270
                }

                MouseArea {
                    anchors.fill: parent
                    onClicked: {
                        voice.visible = !voice.visible
                    }
                }
            }
            Text {
                color: "#ffffff"
                text: '倍速'
                anchors.verticalCenter: parent.verticalCenter
                font.pixelSize: 30
                MouseArea {
                    anchors.fill: parent
                    onClicked: {
                        speedBox.visible = true
                    }
                }

                Item {
                    visible: false
                    id: speedBox
                    y: -146
                    width: 76
                    height: 143
                    anchors.horizontalCenterOffset: 0
                    anchors.horizontalCenter: parent.horizontalCenter
                    Column {
                        id: column
                        anchors.fill: parent
                        anchors.topMargin: -10
                        Text {
                            color: "#676767"

                            text: '1.00'
                            font.pixelSize: 24
                            anchors.horizontalCenter: parent.horizontalCenter
                            MouseArea {
                                anchors.fill: parent
                                onClicked: {
                                    speedBox.visible = false
                                }
                            }
                        }
                        Text {
                            color: "#676767"

                            text: '1.25'
                            font.pixelSize: 24
                            anchors.horizontalCenter: parent.horizontalCenter
                            MouseArea {
                                anchors.fill: parent
                                onClicked: {
                                    speedBox.visible = false
                                }
                            }
                        }
                        Text {
                            color: "#676767"

                            text: '1.50'
                            font.pixelSize: 24
                            anchors.horizontalCenter: parent.horizontalCenter
                            MouseArea {
                                anchors.fill: parent
                                onClicked: {
                                    speedBox.visible = false
                                }
                            }
                        }
                        Text {
                            color: "#676767"

                            text: '1.75'
                            font.pixelSize: 24
                            anchors.horizontalCenter: parent.horizontalCenter
                            MouseArea {
                                anchors.fill: parent
                                onClicked: {
                                    speedBox.visible = false
                                }
                            }
                        }
                        Text {
                            color: "#676767"
                            text: '2.00'
                            font.pixelSize: 24
                            anchors.horizontalCenter: parent.horizontalCenter
                            MouseArea {
                                anchors.fill: parent
                                onClicked: {
                                    speedBox.visible = false
                                }
                            }
                        }
                    }
                }
            }
        }
    }
}

/*##^##
Designer {
    D{i:0;formeditorZoom:0.9}
}
##^##*/

