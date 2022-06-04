

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
    property var infoMap: ({})

    ColumnLayout {
        anchors.fill: parent
        spacing: 0
        anchors.rightMargin: 12
        anchors.leftMargin: 12
        Text {
            //text: '名称.mp3'
            text: (infoMap["fileName"]===undefined?"未识别文件名":infoMap["fileName"])
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
            text: '文件类型：\t '+(infoMap["fileType"]===undefined?"未识别文件类型":infoMap["fileType"])
        }

        Text {
            Layout.leftMargin: 12
            text: '文件位置：\t '+(infoMap["path"]===undefined?"未识别文件路径":infoMap["path"])
        }

        Text {
            Layout.leftMargin: 12
            text: '时长：\t ' + (infoMap["totalTime"]===undefined?"未识别时长":infoMap["totalTime"]) + "s"
        }

        Text {
            Layout.leftMargin: 12
            text: '视频分辨率：\t '+ (infoMap["resolvingPower"]===undefined?"未识别视频分辨率":infoMap["resolvingPower"])
        }

        Text {
            Layout.leftMargin: 12
            text: '视频码率：\t '+ (infoMap["videoBitRate"]===undefined?"未识别视频码率":infoMap["videoBitRate"])
        }
        Text {
            Layout.leftMargin: 12
            text: '视频帧率：\t '+ (infoMap["videoFrameRate"]===undefined?"未识别视频帧率":infoMap["videoFrameRate"])
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
        Text {
            Layout.leftMargin: 12
            text: '音频码率：\t '+(infoMap["audioBitRate"]===undefined?"未识别音频码率":infoMap["audioBitRate"])
        }
        Text {
            Layout.leftMargin: 12
            text: '音频声道数：\t '+(infoMap["numberOfChannels"]===undefined?"未识别音频声道数":infoMap["numberOfChannels"])
        }
        Text {
            Layout.leftMargin: 12
            text: '音频采样率：\t '+(infoMap["sample_rate"]===undefined?"未识别音频采样率":infoMap["sample_rate"])
        }

    }
}

/*##^##
Designer {
    D{i:0;formeditorZoom:1.25}
}
##^##*/

