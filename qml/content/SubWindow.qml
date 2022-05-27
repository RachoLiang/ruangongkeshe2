

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
    property string fileName: "未知文件名"
    property string fileType: "未知类型"
    property string path: "未知路径"
    property string totalTime: "未获取到具体时间"
    property string videoBitRate: "0kbps"
    property string videoFrameRate: "0fps"
    property string resolvingPower: "未识别到分辨率"

    property string audioBitRate: "0kbps"
    property string numberOfChannels: "未获取到声道数"
    property string sample_rate: "未识别到采样率"

    ColumnLayout {
        anchors.fill: parent
        spacing: 0
        anchors.rightMargin: 12
        anchors.leftMargin: 12
        Text {
            //text: '名称.mp3'
            text:fileName
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
            text: '文件类型：\t '+fileType
        }

        Text {
            Layout.leftMargin: 12
            text: '文件位置：\t '+path
        }

        Text {
            Layout.leftMargin: 12
            text: '时长：\t ' + totalTime + "s"
        }

        Text {
            Layout.leftMargin: 12
            text: '视频分辨率：\t '+ resolvingPower
        }

        Text {
            Layout.leftMargin: 12
            text: '视频码率：\t '+ videoBitRate
        }
        Text {
            Layout.leftMargin: 12
            text: '视频帧率：\t '+ videoFrameRate
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
            text: '音频码率：\t '+audioBitRate
        }
        Text {
            Layout.leftMargin: 12
            text: '音频声道数：\t '+numberOfChannels
        }
        Text {
            Layout.leftMargin: 12
            text: '音频采样率：\t '+sample_rate
        }

    }
}

/*##^##
Designer {
    D{i:0;formeditorZoom:1.25}
}
##^##*/

