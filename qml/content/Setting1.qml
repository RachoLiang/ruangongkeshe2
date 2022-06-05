

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

    width: 300
    height: 400
    opacity: 1
    title: "设置"
    ColumnLayout {
        anchors.fill: parent
        spacing: 10
        anchors.rightMargin: 12
        anchors.leftMargin: 12
        Text {
            text: '播放器设置'
            font.pixelSize: 18
        }
        Rectangle {
            color: "#b4b4b4"
            Layout.preferredHeight: 2
            Layout.fillWidth: true
        }

        Text {
            text: '视频色彩设置：'
            font.pixelSize: 15
            font.styleName: "Bold"
        }
        RowLayout {
            spacing: 10
            Text {
                id: liangdu
                text: qsTr("亮   度")
            }

            Slider {
                id: slider_liangdu
                value: 0.5
                onValueChanged: {
                    //console.log(slider_liangdu.visualPosition);
                    videoShow.setArgs(-1,slider_liangdu.visualPosition,-1);
                    liangduzhi.text = Math.ceil(value * 100)
                }
            }

            Text {
                anchors.left: slider_liangdu.right
                anchors.leftMargin: 11
                id: liangduzhi
                text: qsTr("50")
            }

            Image {
                Layout.preferredHeight: 15
                Layout.preferredWidth: 15
                //anchors.left: liangduzhi.right
                Layout.leftMargin: 30
                fillMode:Image.PreserveAspectFit
                source: "../content/images/reset.png"
                RoundButton {
                    anchors.fill: parent
                    flat: true
                    ToolTip.visible: hovered
                    ToolTip.text: qsTr("重置")
                    onClicked: {
                        slider_liangdu.value = 0.5
                    }
                }
            }

        }
        RowLayout {
            spacing: 10
            Text {
                id: duibidu
                text: qsTr("对比度")
            }

            Slider {
                id: slider_duibidu
                value: 0.5
                onValueChanged: {
                    videoShow.setArgs(slider_duibidu.visualPosition,-1,-1);
                    duibiduzhi.text = Math.ceil(value * 100)
                }
            }

            Text {
                anchors.left: slider_liangdu.right
                anchors.leftMargin: 5
                id: duibiduzhi
                text: qsTr("50")
            }

            Image {
                Layout.preferredHeight: 15
                Layout.preferredWidth: 15
                anchors.left: liangduzhi.right
                Layout.leftMargin: 30
                fillMode:Image.PreserveAspectFit
                source: "../content/images/reset.png"
                RoundButton {
                    anchors.fill: parent
                    flat: true
                    ToolTip.visible: hovered
                    ToolTip.text: qsTr("重置")
                    onClicked: {
                        slider_duibidu.value = 0.5
                    }
                }
            }
        }
        RowLayout {
            spacing:10
            Text {
                id: baohedu
                text: qsTr("饱和度")
            }

            Slider {
                id: slider_baohedu
                value: 0.33
                onValueChanged: {
                    videoShow.setArgs(-1,-1,slider_baohedu.visualPosition);
                    baoheduzhi.text = Math.ceil(value * 100)
                }
            }

            Text {
                anchors.left: slider_liangdu.right
                anchors.leftMargin: 5
                id: baoheduzhi
                text: qsTr("33")
            }

            Image {
                Layout.preferredHeight: 15
                Layout.preferredWidth: 15
                //anchors.left: liangduzhi.right
                Layout.leftMargin: 30
                fillMode:Image.PreserveAspectFit
                source: "../content/images/reset.png"
                RoundButton {
                    anchors.fill: parent
                    flat: true
                    ToolTip.visible: hovered
                    ToolTip.text: qsTr("重置")
                    onClicked: {
                        slider_baohedu.value = 0.33
                    }
                }
            }
        }
//        RowLayout {
//            Text {
//                id: sexiang
//                text: qsTr("色   相")
//            }

//            Slider {
//                id: slider_sexiang
//                value: 0.5
//                onValueChanged: {
//                    sexiangzhi.text = Math.ceil(value * 100)
//                }
//            }

//            Text {
//                anchors.left: slider_liangdu.right
//                anchors.leftMargin: 5
//                id: sexiangzhi
//                text: qsTr("50")
//            }

//            Image {
//                Layout.preferredHeight: 15
//                Layout.preferredWidth: 15
//                anchors.left: liangduzhi.right
//                anchors.leftMargin: 10
//                fillMode:Image.PreserveAspectFit
//                source: "../content/images/reset.png"
//                RoundButton {
//                    anchors.fill: parent
//                    flat: true
//                    ToolTip.visible: hovered
//                    ToolTip.text: qsTr("重置")
//                    onClicked: {
//                        slider_sexiang.value = 0.5
//                    }
//                }
//            }
//        }
        Rectangle {
            color: "#b4b4b4"
            Layout.preferredHeight: 2
            Layout.fillWidth: true
        }
        Text {
            text: '截图设置'
            font.pixelSize: 15
            font.styleName: "Bold"
        }
        ColumnLayout {
            RowLayout {
                Text {
                    id: shuchuweizhi
                    text: qsTr("输出位置：")
                }
                Rectangle {
                    border.color: "#353535"
                    Layout.fillHeight: true
                    Layout.fillWidth: true
                    TextInput {
                        id: textInput
                        text: qsTr("")
                        anchors.fill: parent
                        font.pixelSize: 12
                    }
                }
            }
            RowLayout {
                Text {
                    id: text111
                    text: qsTr("输出格式:")
                }
                ComboBox {

                    width: parent.width
                    transformOrigin: Item.Center
                    model: [".png", ".jpg"]
                }
            }
        }

        Button {
            anchors.leftMargin: 100
            id: baocun
            text: qsTr("保存")
        }
    }
}

/*##^##
Designer {
    D{i:0;formeditorZoom:1.25}D{i:23}
}
##^##*/

