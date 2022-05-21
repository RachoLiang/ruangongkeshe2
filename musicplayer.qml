/****************************************************************************
**
** Copyright (C) 2017 The Qt Company Ltd.
** Contact: https://www.qt.io/licensing/
**
** This file is part of the examples of the Qt Toolkit.
**
** $QT_BEGIN_LICENSE:BSD$
** Commercial License Usage
** Licensees holding valid commercial Qt licenses may use this file in
** accordance with the commercial license agreement provided with the
** Software or, alternatively, in accordance with the terms contained in
** a written agreement between you and The Qt Company. For licensing terms
** and conditions see https://www.qt.io/terms-conditions. For further
** information use the contact form at https://www.qt.io/contact-us.
**
** BSD License Usage
** Alternatively, you may use this file under the terms of the BSD license
** as follows:
**
** "Redistribution and use in source and binary forms, with or without
** modification, are permitted provided that the following conditions are
** met:
**   * Redistributions of source code must retain the above copyright
**     notice, this list of conditions and the following disclaimer.
**   * Redistributions in binary form must reproduce the above copyright
**     notice, this list of conditions and the following disclaimer in
**     the documentation and/or other materials provided with the
**     distribution.
**   * Neither the name of The Qt Company Ltd nor the names of its
**     contributors may be used to endorse or promote products derived
**     from this software without specific prior written permission.
**
**
** THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
** "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
** LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
** A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
** OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
** SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
** LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
** DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
** THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
** (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
** OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE."
**
** $QT_END_LICENSE$
**
****************************************************************************/

import QtQuick
import QtQuick.Layouts
import QtQuick.Controls
import QtQuick.Window
import Qt.labs.platform 1.1
import playlistclass 1.0      //这个类已经在musicplayer.cpp中注册
import VideoShow 1.0
ApplicationWindow {
    id: window
    width: 1280
    height: 720
    visible: true
    title: "朝阳创客队 音视频播放器"

    //用来适配手机屏幕的一些转换函数
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

    Component.onCompleted: {
        x = Screen.width / 2 - width / 2
        y = Screen.height / 2 - height / 2
    }

    Shortcut {
        sequence: "Ctrl+Q"
        onActivated:
        {
            playlist.saveToDataBase()
            Qt.quit()
        }
    }
    PlayList{
        id:playlist;  //在全局构造一个播放列表对象
        onAddFileInGUI:function(filePath) //在PlayList.h中定义的信号void addFileInGUI(QString filePath);
        {
            listviewmodel.append({Path:filePath})  //给ListViewModel起了个id，以便在此处对其操作
        }
    }


    header: ToolBar {
        RowLayout {
            id: headerRowLayout
            anchors.fill: parent
            spacing: 0

            ToolButton {
                icon.name: "grid"
            }
            ToolButton {
                id:zidingyi
                icon.name: "settings"

            }
            ToolButton {
                icon.name: "filter"

            }
            ToolButton {
                icon.name: "message"
            }
            ToolButton {
                icon.name: "music"
            }
//            ToolButton {
//                icon.name: "cloud"
//            }
//            ToolButton {
//                icon.name: "bluetooth"
//            }
//            ToolButton {
//                icon.name: "cart"
//            }

            Item {
                Layout.fillWidth: true
            }

            ToolButton {
                icon.name: "power"
                onClicked:
                {
                    playlist.saveToDataBase()
                    Qt.quit()
                }
            }
        }
    }


    Label {
        text: "做敏捷、稳定的优秀播放器！"
        font.pixelSize: Qt.application.font.pixelSize * 1.3
        anchors.centerIn: header
        z: header.z + 1

    }


    RowLayout {
        spacing: 115
        anchors.fill: parent
        anchors.margins: 70

        ColumnLayout {
            spacing: 16
            Layout.preferredWidth: 100

            ButtonGroup {
                buttons: libraryRowLayout.children
            }

            RowLayout {
                id: libraryRowLayout
                Layout.alignment: Qt.AlignHCenter

                Button {
                    text: "媒体库"
                    checked: true
                }
                Button {
                    text: "播放列表"
                    checkable: true
                    onClicked: playlist.showFileList() //用于测试，点击界面的“播放列表”按钮将在控制台展示整个列表
                }
                Button {
                    text: "我的收藏"
                    checkable: true
                }
            }

            RowLayout {
                TextField {
                    Layout.fillWidth: true
                }
                FileDialog {
                    id: fileDialog
                    title: "导入音频或者视频文件"
                    nameFilters: [ "视频文件 (*.ts *.mp4 *.avi *.flv *.mkv *.3gp)",
                        "音频文件 (*.mp3 *.ogg *.wav *.wma *.ape *.ra)"]
                    acceptLabel: "确定"
                    rejectLabel: "取消"
                    fileMode: FileDialog.OpenFile
                    onAccepted: {
                        console.log("对话框：选中的文件有")
                        for (var i in files) {
                            playlist.addFile(files[i]) //调用播放列表对象的函数，将用户选中的路径加入播放列表中
                        }
                    }
                }
                Button {
                    icon.name: "folder"
                    onClicked: fileDialog.open()
                }
            }

            Frame {
                id: filesFrame
                leftPadding: 1
                rightPadding: 1

                Layout.fillWidth: true
                Layout.fillHeight: true

                ListView {
                    id: filesListView
                    clip: true
                    anchors.fill: parent


                    model: ListModel {
                            id: listviewmodel  //在这里给组件起了个id，这样在外面也可以操作它，用于向列表视图中添加条目

                        Component.onCompleted: {
                            playlist.init()  //ListViewModel已经准备好，调用init在后台恢复出playlist，并发射信号，在界面同步恢复列表
//                            for (var i = 0; i < 100; ++i) {
//                                append({
//                                   author: "作者",
//                                   album: "梁立名",
//                                   track: "第 0" + (i % 9 + 1)+"集",
//                                });
//                            }
                        }

                    }

                    delegate: ItemDelegate {
                        text: model.Path
                        width: filesListView.width
                    }

                    ScrollBar.vertical: ScrollBar {
                        parent: filesFrame
                        policy: ScrollBar.AlwaysOn
                        anchors.top: parent.top
                        anchors.topMargin: filesFrame.topPadding
                        anchors.right: parent.right
                        anchors.rightMargin: 1
                        anchors.bottom: parent.bottom
                        anchors.bottomMargin: filesFrame.bottomPadding
                    }
                }
            }
        }



        ColumnLayout {
            spacing: 26
            Layout.preferredWidth: 230

            Item {
                Layout.fillWidth: true
                Layout.fillHeight: true

//                Image {
//                    anchors.fill: parent
//                    fillMode: Image.PreserveAspectCrop
//                    source: "images/album-cover.jpg"
//                }
                Rectangle{
                    width: dpW(2000)
                    height: dpH(3200)
                    border.width: 5
                    border.color: "black"
                    radius: 10
                    VideoShow{
                        id: videoShow;
                        anchors.centerIn: parent;
                        nWidth: dpW(2000);
                        nHeight: dpH(3200);
                        width: dpW(2000);
                        height: dpH(3200);
                        sourPath: "C:\\Users\\xgy\\Desktop\\mp3_test\\test.mp4"
                    }
                }

            }
            Item{
                Rectangle {
                        id: button

                        width: 100
                        height: 30
                        color: "red"
                        radius: 5
                        anchors.centerIn: parent

                        Text {
                            id: buttonText
                            text: qsTr("提高音量,每次加3")
                            color: "white"
                            anchors.centerIn: parent
                        }

                        MouseArea {
                            anchors.fill: parent
                            onClicked: {
                                //音量调节
//                                var x = videoShow.getVolume();
//                                videoShow.setVolume(x + 3);
                                //暂停
//                                videoShow.pause();
//                                buttonText.text = "暂停状态："+videoShow.isPaused();
                                //静音
                                videoShow.silence();
                                buttonText.text = "静音状态："+videoShow.isSilence();
                            }
                        }
                    }
            }

            Item {
                id: songLabelContainer
                clip: true

                Layout.fillWidth: true
                Layout.preferredHeight: songNameLabel.implicitHeight

                SequentialAnimation {
                    running: true
                    loops: Animation.Infinite

                    PauseAnimation {
                        duration: 2000
                    }
                    ParallelAnimation {
                        XAnimator {
                            target: songNameLabel
                            from: 0
                            to: songLabelContainer.width - songNameLabel.implicitWidth
                            duration: 5000
                        }
                        OpacityAnimator {
                            target: leftGradient
                            from: 0
                            to: 1
                        }
                    }
                    OpacityAnimator {
                        target: rightGradient
                        from: 1
                        to: 0
                    }
                    PauseAnimation {
                        duration: 1000;
                    }
                    OpacityAnimator {
                        target: rightGradient
                        from: 0
                        to: 1
                    }
                    ParallelAnimation {
                        XAnimator {
                            target: songNameLabel
                            from: songLabelContainer.width - songNameLabel.implicitWidth
                            to: 0
                            duration: 5000
                        }
                        OpacityAnimator {
                            target: leftGradient
                            from: 0
                            to: 1
                        }
                    }
                    OpacityAnimator {
                        target: leftGradient
                        from: 1
                        to: 0
                    }
                }

                Rectangle {
                    id: leftGradient
                    gradient: Gradient {
                        GradientStop {
                            position: 0
                            color: "#dfe4ea"
                        }
                        GradientStop {
                            position: 1
                            color: "#00dfe4ea"
                        }
                    }

                    width: height
                    height: parent.height
                    anchors.left: parent.left
                    z: 1
                    rotation: -90
                    opacity: 0
                }

                Label {
                    id: songNameLabel
                    text: "速度与激情7 -保罗"
                    font.pixelSize: Qt.application.font.pixelSize * 1.4
                }

                Rectangle {
                    id: rightGradient
                    gradient: Gradient {
                        GradientStop {
                            position: 0
                            color: "#00dfe4ea"
                        }
                        GradientStop {
                            position: 1
                            color: "#dfe4ea"
                        }
                    }

                    width: height
                    height: parent.height
                    anchors.right: parent.right
                    rotation: -90
                }
            }

            RowLayout {
                spacing: 8
                Layout.alignment: Qt.AlignHCenter

                RoundButton {
                    icon.name: "favorite"
                    icon.width: 32
                    icon.height: 32
                }
                RoundButton {
                    icon.name: "stop"
                    icon.width: 32
                    icon.height: 32
                }
                RoundButton {
                    icon.name: "previous"
                    icon.width: 32
                    icon.height: 32
                }
                RoundButton {
                    icon.name: "pause"
                    icon.width: 32
                    icon.height: 32
                }
                RoundButton {
                    icon.name: "next"
                    icon.width: 32
                    icon.height: 32
                }
                RoundButton {
                    icon.name: "repeat"
                    icon.width: 32
                    icon.height: 32
                }
                RoundButton {
                    icon.name: "shuffle"
                    icon.width: 32
                    icon.height: 32
                }
            }

            Slider {
                id: seekSlider
                value: 113
                to: 261

                Layout.fillWidth: true

                ToolTip {
                    parent: seekSlider.handle
                    visible: seekSlider.pressed
                    text: pad(Math.floor(value / 60)) + ":" + pad(Math.floor(value % 60))
                    y: parent.height

                    readonly property int value: seekSlider.valueAt(seekSlider.position)

                    function pad(number) {
                        if (number <= 9)
                            return "0" + number;
                        return number;
                    }
                }
            }
        }

        }
}
