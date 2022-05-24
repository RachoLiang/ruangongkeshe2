

/*
This is a UI file (.ui.qml) that is intended to be edited in Qt Design Studio only.
It is supposed to be strictly declarative and only uses a subset of QML. If you edit
this file manually, you might introduce QML code that is not supported by Qt Design Studio.
Check out https://doc.qt.io/qtcreator/creator-quick-ui-forms.html for details on .ui.qml files.
*/
import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Layouts 1.15

Window {
    width: 260
    height: 380
    ColumnLayout {
        anchors.fill: parent
        spacing: 0
        anchors.rightMargin: 12
        anchors.leftMargin: 12
        Text {
            text: '名称.mp3'
            font.pixelSize: 18
        }
        Rectangle {
            color: "#b4b4b4"
            Layout.preferredHeight: 2
            Layout.fillWidth: true
        }

        Text {
            text: '基本信息'
            font.pixelSize: 12
            font.styleName: "Bold"
        }

        Text {
            Layout.leftMargin: 12
            text: '文件类型：\t mp3'
        }

        Text {
            Layout.leftMargin: 12
            text: '文件大小：\t 200.01MB'
        }

        Text {
            Layout.leftMargin: 12
            text: '文件位置：\t C:/Desktop/test'
        }

        Text {
            Layout.leftMargin: 12
            text: '文件码率：\t 500fp'
        }
        Text {
            Layout.leftMargin: 12
            text: '文件时长：\t 3分15秒'
        }
        //        Repeater {
        //            model: 5
        //            Text {
        //                Layout.leftMargin: 12
        //                text: '文件类型：\tmp3'
        //            }

        //        }
        Rectangle {
            color: "#b4b4b4"
            Layout.preferredHeight: 2
            Layout.fillWidth: true
        }
        Text {
            text: '音频信息'
            font.pixelSize: 12
            font.styleName: "Bold"
        }

        Repeater {
            model: 3
            Text {
                Layout.leftMargin: 12
                text: '文件类型：\tmp3'
            }
        }
    }
}

/*##^##
Designer {
    D{i:0;formeditorZoom:1.25}
}
##^##*/

