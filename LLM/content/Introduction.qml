

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
            text: '欢迎您使用本播放器！'
            font.pixelSize: 18
        }
        Rectangle {
            color: "#b4b4b4"
            Layout.preferredHeight: 2
            Layout.fillWidth: true
        }

        Text {
            text: '快捷键说明：'
            font.pixelSize: 12
            font.styleName: "Bold"
        }

        Text {
            Layout.leftMargin: 12
            text: '暂停和播放：空格'
        }

        Text {
            Layout.leftMargin: 12
            text: '退出全屏：Esc'
        }

        Text {
            Layout.leftMargin: 12
            text: '上一帧：左箭头'
        }

        Text {
            Layout.leftMargin: 12
            text: '下一帧：右箭头'
        }

        Rectangle {
            color: "#b4b4b4"
            Layout.preferredHeight: 2
            Layout.fillWidth: true
        }
        Text {
            text: '功能说明和产品亮点'
            font.pixelSize: 12
            font.styleName: "Bold"
        }
        Text {
            Layout.leftMargin: 12
            text: '主流格式音视频播放'
        }
        Text {
            Layout.leftMargin: 12
            text: '含快捷键的播放控制'
        }
        Text {
            Layout.leftMargin: 12
            text: '媒体库功能'
        }
        Text {
            Layout.leftMargin: 12
            text: '查看媒体详细信息'
        }
        Text {
            Layout.leftMargin: 12
            text: '进度微调（上一帧、下一帧）'
        }
        Text {
            Layout.leftMargin: 12
            text: '倍速播放（1.0，1.25，1.5，1.75，2.0）'
        }
        Text {
            Layout.leftMargin: 12
            text: '进度条预览功能'
        }
        Text {
            Layout.leftMargin: 12
            text: '视频倒放'
        }
        Text {
            Layout.leftMargin: 12
            text: '音频波形图'
        }

        //        Repeater {
        //            model: 3
        //            Text {
        //                Layout.leftMargin: 12
        //                text: '文件类型：\tmp3'
        //            }
        //        }
    }
}

/*##^##
Designer {
    D{i:0;formeditorZoom:1.25}
}
##^##*/

