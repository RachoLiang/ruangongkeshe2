

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
import QtCharts 2.14
import VideoShow 1.0
import ThumnailShow 1.0
import playlistclass 1.0
import Qt5Compat.GraphicalEffects
import LLM

Rectangle {
    id: overALLRectangle
    focus: true
    width: Constants.width
    height: Constants.height
    anchors.fill: parent

    property bool nowIsPlayingAudio: true    //记录当前正在播放的是音频还是视频，从而知道该操作哪个列表
    Keys.onPressed: {//所有快捷键操作
        if ((event.key == Qt.Key_I) && (event.modifiers & Qt.ControlModifier)){
            //ctrl + i
            fileDialog.open()
        }
        else if((event.key == Qt.Key_Right) && (event.modifiers & Qt.ControlModifier)){
            //ctrl + ->
            nextPlayBtn.nextPlayBtnClicked()
        }
        else if((event.key == Qt.Key_Left) && (event.modifiers & Qt.ControlModifier)){
            //ctrl + <-
            lastPlayBtn.lastPlayBtnClicked()
        }
        else if((event.key == Qt.Key_Down) && (event.modifiers & Qt.ControlModifier)){
            //ctrl + 下
            voice.decrease()
        }
        else if((event.key == Qt.Key_Up) && (event.modifiers & Qt.ControlModifier)){
            //ctrl + 上
            voice.increase()
        }
        else if((event.key == Qt.Key_Return)){
            //本来应该是空格键，但尝试后发现，按下空格键，结果是触发了鼠标最近点击的按钮，暂时不知道怎么解决，所以用回车键代替
            playbutton.playButtonActivate()
        }
        else if((event.key == Qt.Key_F) && (event.modifiers & Qt.ControlModifier)){
            //ctrl + f
            full_screen_MouseArea.full_screenClicked()
        }
    }


    PlayList{
        id:yinpinplaylist;  //在全局构造一个音频播放列表对象
        onAddAudioFileInGUI:function(audioPath,audioDuration)
        {
            yinpinmodel.append({yinpintext:audioPath,yinpinduration:audioDuration})
        }
        onShowAudio: function(audioPath)
        {
            if(!update_value.running){
                update_value.start();
            }
            videoShow.clearAlbum()
            videoShow.show(audioPath,"music");
            playbuttonimage.source="../content/images/pause.png"
        }
        onChangePlayModeButtonIcon: function(iconName) //改变播放模式按钮图片
        {
            playmodeimage.source=iconName
        }
        onChangeCurrentPlayingIndex: function(index) //改变列表中的高亮条目
        {
            yinpinlistview.currentIndex=index
        }
        onShowMessage: function(messageInfo)
        {
            console.log(messageInfo)
        }
    }
    PlayList{
        id:shipinplaylist;
        onAddVideoFileInGUI:function(videoPath,videoDuration)
        {
            shipinmodel.append({shipintext:videoPath,shipinduration:videoDuration})
        }
        onShowVideo:function(videoPath)
        {
            update_value.stop()
            videoShow.clearAlbum()
            videoShow.show(videoPath,"video")
            thumbnailShow.setPathAndStart(videoPath)
            playbuttonimage.source="../content/images/pause.png"
        }
        onChangeCurrentPlayingIndex: function(index) //改变列表中的高亮条目
        {
            shipinlistview.currentIndex=index
        }
        onShowMessage: function(messageInfo)
        {
            console.log(messageInfo)
        }
    }


    Image {
        id:leftarrow
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
                leftarrow.visible = false
            }
        }
    }
    Rectangle {
        id: leftList
        anchors.left: parent.left
        anchors.top: parent.top
        anchors.bottom: parent.bottom
        width: open ? parent.width/4 : 0
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
                        nameFilters: [ "视频文件 (*.ts *.mp4 *.avi *.flv *.mkv *.3gp)",
                            "音频文件 (*.mp3 *.ogg *.wav *.wma *.ape *.ra *.flac)"]
                        acceptLabel: "确定"
                        rejectLabel: "取消"
                        fileMode: FileDialog.OpenFile
                        onAccepted: {

                            console.log("对话框：选中的文件有")
                            console.log(selectedFile)
                            //选中的文件可能是音频，可能是视频，两个列表会自行取舍
                            yinpinplaylist.addFile(selectedFile)
                            shipinplaylist.addFile(selectedFile)

                        }
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
                            leftarrow.visible = true
                        }
                    }
                }
            }
            Item {
                id: yinpin
                width: parent.width
                property int count: 3
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
                Frame {
                    id: yinpinframe
                    anchors.left: parent.left
                    anchors.right: parent.right
                    anchors.top: parent.top
                    anchors.topMargin: 60
                    spacing: 12
                    height: 10000
                    clip: true
                    anchors.fill: parent
                    ListModel{
                        id:yinpinmodel


                    }

                    Component{
                        id:yinpindelegate

                        Item {
                            anchors.leftMargin: 15
                            anchors.left: parent.left
                            anchors.right: parent.right
                            height: 40
                            Column {
                                spacing: 0
                                Text {
                                    color: yinpinlistview.currentIndex==index?"red":"#707070"
                                    text: yinpintext
                                    font.pixelSize: yinpinlistview.currentIndex==index?18:15
                                }
                                Text {
                                    color: "#b6b6b6"
                                    font.pixelSize: 11
                                    text: yinpinduration
                                }
                            }
                            MouseArea {
                                anchors.fill: parent
                                onClicked:
                                {
                                    if(yinpinplaylist.setNowIndex(index))
                                    {
                                        yinpinlistview.currentIndex=index
                                        playbuttonimage.source="../content/images/pause.png"
                                        nowIsPlayingAudio=true
                                        console.log("点击第"+yinpinlistview.currentIndex+"个音频")
                                    }
                                    else
                                    {
                                        yinpinplaylist.playNextMedia(0)
                                        playbuttonimage.source="../content/images/pause.png"
                                        nowIsPlayingAudio=true
                                    }
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
                                        id:yinpinMenuItem
                                        text: '置顶'
                                        function toppingYinpin(idx)
                                        {
                                            console.log("置顶音频,index="+idx)
                                            yinpinmodel.move(idx,0,1)
                                            yinpinplaylist.toppingFile(idx)
                                            overALLRectangle.forceActiveFocus()
                                        }
                                        onTriggered: {
                                            toppingYinpin(index)

                                        }
                                    }
                                    MenuItem {
                                        text: '设置'
                                        onTriggered: {
                                            setting_for_music.show()
                                            overALLRectangle.forceActiveFocus()
                                        }
                                    }
                                    MenuItem {
                                        text: '详细信息'
                                        onTriggered: {
                                            subWindow.infoMap = yinpinplaylist.getMediaInfo(index,"music")
                                            subWindow.show()
                                            overALLRectangle.forceActiveFocus()
                                        }
                                    }
                                    MenuItem {
                                        text: '删除'
                                        function deleteYinPin(idx)
                                        {
                                            console.log("删除音频index="+idx)
                                            yinpinmodel.remove(idx)
                                            yinpinplaylist.removeFile(idx)
                                            if(nowIsPlayingAudio&&yinpinlistview.currentIndex==idx&&videoShow.isPaused()==false)
                                            {
                                                yinpinplaylist.playNextMedia(0)
                                            }
                                            overALLRectangle.forceActiveFocus()
                                        }

                                        onTriggered: {
                                            deleteYinPin(index)
                                        }
                                    }
                                }
                            }
                        }
                    }
                    ListView {
                        id:yinpinlistview
                        parent:yinpinframe
                        anchors.fill: parent
                        model: yinpinmodel
                        delegate: yinpindelegate
                        Component.onCompleted: {
                            yinpinplaylist.init(1)  //视频列表初始化
                        }
                        ScrollBar.vertical: ScrollBar {
                            parent: yinpinframe
                            policy: ScrollBar.AlwaysOn
                            anchors.top: parent.top
                            anchors.topMargin: yinpinframe.topPadding
                            anchors.right: parent.right
                            anchors.rightMargin: 1
                            anchors.bottom: parent.bottom
                            anchors.bottomMargin: yinpinframe.bottomPadding
                        }

                    }
                }

            }
            Item {
                id: shipin
                width: parent.width
                height: 60 + count * (40 + 12) - 12
                clip: true
                property int count: 6
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
                Frame {
                    id: shipinframe
                    anchors.left: parent.left
                    anchors.right: parent.right
                    anchors.top: parent.top
                    anchors.topMargin: 60
                    spacing: 12
                    height: 10000
                    clip: true

                    anchors.fill: parent


                    ListModel{
                        id:shipinmodel
                        //ListElement{shipintext:"ttteeess";}

                    }

                    Component{
                        id:shipindelegate

                        Item {
                            anchors.leftMargin: 15
                            //anchors.topMargin: 10
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
                                    color: shipinlistview.currentIndex==index?"red":"#707070"
                                    text: shipintext
                                    font.pixelSize: shipinlistview.currentIndex==index?18:15


                                }
                                Text {
                                    color: "#b6b6b6"
                                    font.pixelSize: 11
                                    text: shipinduration
                                }

                            }
                            MouseArea {
                                anchors.fill: parent
                                onClicked:
                                {
                                    if(shipinplaylist.setNowIndex(index))
                                    {
                                        shipinlistview.currentIndex=index
                                        playbuttonimage.source="../content/images/pause.png"
                                        nowIsPlayingAudio=false
                                        console.log("点击第"+shipinlistview.currentIndex+"个视频")
                                    }
                                    else
                                    {
                                        shipinplaylist.playNextMedia(0)
                                        playbuttonimage.source="../content/images/pause.png"
                                        nowIsPlayingAudio=true
                                    }



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
                                        id:shipinMenuItem
                                        text: '置顶'
                                        function toppingShipin(idx)
                                        {
                                            console.log("置顶视频,index="+idx)
                                            shipinmodel.move(idx,0,1)
                                            shipinplaylist.toppingFile(idx)
                                            overALLRectangle.forceActiveFocus()
                                        }
                                        onTriggered: {
                                            toppingShipin(index)
                                        }
                                    }
                                    MenuItem {
                                        text: '设置'
                                        onTriggered: {
                                            setting_for_video.show()
                                            overALLRectangle.forceActiveFocus()
                                        }
                                    }
                                    MenuItem {
                                        text: '倒放'
                                        onClicked:{
                                            shipinlistview.currentIndex=index
                                            playbuttonimage.source="../content/images/pause.png"
                                            videoShow.reverse(shipinplaylist.getFilename(index))
                                            nowIsPlayingAudio=false
                                        }
                                    }
                                    MenuItem {
                                        text: '详细信息'
                                        onTriggered: {
                                            subWindow3.infoMap = shipinplaylist.getMediaInfo(index,"video")
                                            subWindow3.show()
                                            overALLRectangle.forceActiveFocus()
                                        }
                                    }
                                    MenuItem{
                                        text: '删除'
                                        function deleteShiPin(idx)
                                        {
                                            console.log("删除视频index="+idx)
                                            shipinmodel.remove(idx)
                                            shipinplaylist.removeFile(idx)
                                            //如果删除的正好是当前播放的，则视为删除后点击了下一首
                                            if(nowIsPlayingAudio==false&&shipinlistview.currentIndex==idx&&videoShow.isPaused()==false)
                                            {
                                                shipinplaylist.playNextMedia(0)
                                            }
                                            overALLRectangle.forceActiveFocus()
                                        }
                                        onTriggered: {
                                            deleteShiPin(index)
                                        }
                                    }
                                }
                            }
                        }
                    }
                    ListView {
                        id:shipinlistview
                        parent:shipinframe
                        anchors.fill: parent
                        model: shipinmodel
                        delegate: shipindelegate
                        Component.onCompleted: {
                            shipinplaylist.init(2)  //视频列表初始化
                        }
                        ScrollBar.vertical: ScrollBar {
                            parent: shipinframe
                            policy: ScrollBar.AlwaysOn
                            anchors.top: parent.top
                            anchors.topMargin: shipinframe.topPadding
                            anchors.right: parent.right
                            anchors.rightMargin: 1
                            anchors.bottom: parent.bottom
                            anchors.bottomMargin: shipinframe.bottomPadding
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
                    if(nowIsPlayingAudio){
                        subWindow6.infoMap = yinpinplaylist.getMediaInfo(yinpinplaylist.getNowIndex(),"music")
                    }else{
                        subWindow6.infoMap = shipinplaylist.getMediaInfo(shipinplaylist.getNowIndex(),"video")
                    }
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
        Rectangle{
            id:show
            anchors.fill: parent
            visible: !nowIsPlayingAudio
//            anchors.top:parent.top
//            anchors.left: parent.left
//            anchors.right: parent.right
//            anchors.bottom: controls.top

            border.width: 5
            border.color: "black"
            color: "black"
            radius: 10

            Image {
                id:basePic
                anchors.fill: parent
                source: "images/basepic.png"
//                fillMode: Image.PreserveAspectFit
            }

            VideoShow{
                id: videoShow;
                anchors.centerIn: parent
                //anchors.bottom: slider.top
                //anchors.fill: parent
                //自适应大小 16x9
                nWidth: (parent.width/16)>(parent.height/9)?parent.height/9*16:parent.width
                nHeight: (parent.width/16)<(parent.height/9)?parent.width/16*9:parent.height
                width: (parent.width/16)>(parent.height/9)?parent.height/9*16:parent.width
                height: (parent.width/16)<(parent.height/9)?parent.width/16*9:parent.height

                property bool fullScreen: true
                onPlayStateChanged: {
                    //当前是stop状态或者finish状态，则自动播放下一首
                    console.log("状态改变了")
                    if(videoShow.isFinish()){
                        playbuttonimage.source="../content/images/play.png"
                        console.log("视频finish才调用下一首播放")
                        if(nowIsPlayingAudio){
                            console.log("自动音频下一首")
                            yinpinplaylist.autoPlayNextMedia()
                        }else{
                            console.log("自动视频下一首")
                            shipinplaylist.autoPlayNextMedia()
                        }
                    }
                }
            }
        }


        Rectangle{
            id:albumShow
            visible: nowIsPlayingAudio
            anchors.fill:parent

//            y: main.y + 50
//            spacing: 50

//            Rectangle{
//                width: 150
//                height: 50
//                color: "transparent"
//            }
            Image {
                id:basePic2
                anchors.fill: parent
                source: "images/basepic.png"
//                fillMode: Image.PreserveAspectFit
            }

            Rectangle{
                id: albumMain
                anchors.left: parent.left
                anchors.top: parent.top
                anchors.topMargin: 70
                anchors.leftMargin: 70
                width: main.width/3
                height: width
                radius: width/2
                color: "#666666"

                transform: Rotation{
                    //设置图像原点
                    origin.x: albumMain.width/2
                    origin.y: albumMain.height/2
                    axis{
                        x: 0
                        y: 0    //设置围绕y轴旋转
                        z: 1
                    }
                    NumberAnimation on angle{   //定义角度上的动画
                        from: 0
                        to: 360
                        duration: 20000
                        loops: Animation.Infinite
                    }
                }


//                DropShadow {
//                           anchors.fill: albumMain
//                           horizontalOffset: 2
//                           verticalOffset: 2
//                           radius: 2.0
//                           color: "#444444"
//                           spread: 0.0
//                           source: albumMain
//                    }

                Rectangle{
                    id:middle
                    width: albumMain.width-40
                    height: albumMain.height-40
                    radius: width/2
                    color: "#444444"
                    anchors.centerIn: parent
                }

                Rectangle{
                    id:small
                    width: main.width/18
                    height: main.width/18
                    radius: width/2
                    color: "#F5F5F5"
                    anchors.centerIn: parent
                    visible: true
                }


                //专辑封面
                OpacityMask {
                    id: album
                    anchors.centerIn: parent

                    width: middle.width-30
                    height: width
                    source: albumImage
                    maskSource: audioImage
                }

                Image {
                    id: albumImage
                    width: middle.width-10
                    height: width
                    visible: false
                    fillMode:Image.PreserveAspectFit

                    smooth: true
                    source: "file:///C:\\Users\\xgy\\Desktop\\mp3_test\\frame\\1.png"

                }

                Rectangle{
                    id: audioImage
                    visible: false
                    width: middle.width-10
                    height: width
                    radius: width/2

                }

            }

            //音频波形图
            Rectangle {
                id: audioWave
                anchors.left:albumMain.right
                anchors.leftMargin: main.width/10
                anchors.right: parent.right
                anchors.verticalCenter: albumMain.verticalCenter

                width: main.width/3
                height: main.width/3.2
                radius: 20

                color: "transparent"

                DropShadow {
                           anchors.fill: audioWave
                           horizontalOffset: -5
                           verticalOffset: -5
                           radius: 12.0
                           color: "#20000000"
                           spread: 0.0
                           source: audioWave
                    }

                ColumnLayout{
                    spacing: 20
                    anchors.fill:parent

                    GridLayout{
                        rows: 2
                        columns: 4
                        columnSpacing: 20
                        rowSpacing: 20

                        Text {
                            text: qsTr("专辑标题:")
                            font.pixelSize: 30
                            fontSizeMode: Text.Fit
                            minimumPixelSize: 25

                        }
                        Text {
                            id: albumTitle
                            text: qsTr("线")
                            color: "#0066FF"
                            font.pixelSize: 30
                            fontSizeMode: Text.Fit
                            minimumPixelSize: 25
                        }

                        Text {
                            text: qsTr("")
                        }

                        Text {
                            text: qsTr("")
                        }

                        Text {
                            text: qsTr("专辑名字：")
                            font.pixelSize: 15

                        }

                        Text{
                            id: albumName
                            text: qsTr("线的专辑")
                            color: "#0066FF"
                            font.pixelSize: 15
                        }

                        Text {
                            text: qsTr("专辑作者：")
                            font.pixelSize: 15
                        }

                        Text {
                            id: albumArtist
                            text: qsTr("Amber")
                            color: "#0066FF"
                            font.pixelSize: 15
                        }
                    }

                    GridLayout{

//                        anchors.centerIn: parent
                        columns: 2
                        rows: 1
                        visible: true

                        FastBlur {
                                  anchors.fill: main
                                  source: main
                                  radius: 32
                              }


                        ChartView{
                            id:right_shake
                            title: qsTr("音频波形图")
                            titleFont.pixelSize: 18
                            Layout.minimumWidth:200
                            Layout.minimumHeight:200
                            Layout.preferredWidth: main.width/4
                            Layout.preferredHeight: width
//                            width: main.width/4
//                            height: width
                            antialiasing:true
                            legend.visible:false
                            backgroundColor: "transparent"

                            ValueAxis{
                                id:shake_value_X
                                gridVisible:false
                                visible:false
                                min:0
                                max:10
                            }

                            ValueAxis{
                                id:shake_value_Y
                                labelsVisible:false
                                gridVisible:false
                                visible:true
                                min:0
                                max:200
                            }

                            SplineSeries{
                                id:line
                                axisX:shake_value_X
                                axisY:shake_value_Y
                            }
                        }

    //                    FastBlur {
    //                              anchors.fill: bug
    //                              source: bug
    //                              radius: 32
    //                          }


                        ChartView{
                            id:right_bar
                            title: qsTr("")
                            titleFont.pixelSize: 18
                            Layout.minimumWidth:200
                            Layout.minimumHeight:200
                            Layout.preferredWidth: main.width/4
                            Layout.preferredHeight: width
                            antialiasing:true
                            legend.visible:false
                            backgroundColor: "transparent"

                            BarCategoryAxis{
                                id:right_bar_value_X
                                gridVisible:false
                                labelsVisible:false
                                categories: ["0", "1", "2", "3", "4", "5", "6", "7", "8", "9"]
                            }

                            ValueAxis{
                                id:right_bar_value_Y
                                min:0
                                max:200
                                visible:false
                            }

                            BarSeries{
                                axisX:right_bar_value_X
                                axisY:right_bar_value_Y

                                BarSet {
                                    id:bar
                                    color:"green"
                                    label: "Bob";
                                    values: [1, 2, 3, 4, 5, 6, 7, 8, 9, 10]
                                }
                            }
                        }
                    }
                }



                //定时器
                Timer{
                    id:update_value
                    repeat:true
                    running:true
                    interval:40
                    onTriggered:{
                        //检测窗口是否处于活跃状态
                        if(audioWave.visible == true){
                            line.clear()

                            //获取数据点
                            var available_num = videoShow.getPointNum()
                            var shake_value = videoShow.getShakeList()
                            var bar_value = videoShow.getBarList()

                            for(var i = 0; i < available_num; i = i + 1){
                                line.append(i, shake_value[i])
                            }

                            for(var j = 0; j < available_num; j++){
                                bar.replace(j, bar_value[j])
                            }
                        }
                    }
                }
            }
        }
            Item {
                id: controls
                anchors.left: parent.left
                anchors.right: parent.right
                anchors.bottom: parent.bottom
                height:80
                opacity: open?1:0

                property bool open: false
                MouseArea{
                    anchors.fill: parent
                    hoverEnabled: true
                    onEntered: {
                        if(timer.running){
                            timer.stop()
                        }
                        controls.open = true
                    }
                    onExited: {
                        if(!playbutton.hovered&&!lastPlayBtn.hovered&&!lastFrameButton.hovered&&!nextPlayBtn.hovered&&!nextFrameButton.hovered&&!playModeBtn.hovered){
                             timer.start()
                            //controls.open=false
                        }
                    }
                }

                Timer{
                    id:timer
                    interval: 3000
                    repeat: false
                    onTriggered: {
                        controls.open = false
                    }
                }

                RowLayout{
                    id: slider
                    anchors.left:parent.left
                    anchors.right: parent.right
                    anchors.top:parent.top
                    anchors.topMargin: 10
                    Text {
                        id: lefTime
                        color: "#ffffff"
                        text: '00:00'
                        anchors.verticalCenter: parent.verticalCenter
                        Layout.leftMargin: 30
                        font.pixelSize: 20
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
                        value: 0.0
                        anchors.centerIn: parent
                        //                width: 200
                        //                height: 20
                        //height: 10
                        Layout.rightMargin: 10
                        Layout.leftMargin: 10
                        anchors.verticalCenter: parent.verticalCenter
                        Layout.fillWidth: true


                        MouseArea{
                            id:control_area
                            anchors.fill: parent
                            hoverEnabled: true

                            onPositionChanged: {
                                if(timer.running){
                                    timer.stop()
                                }
                                if(!nowIsPlayingAudio){
                                    yulan.visible = true
                                    yulan.x = (mouseX/control.availableWidth)*(control.availableWidth - yulan.implicitWidth)
                                    thumbnailShow.getFrame(mouseX/control.availableWidth)
                                }
                            }
                            onExited: {
                                yulan.visible = false
                            }
                            propagateComposedEvents :true
                            //释放鼠标事件的覆盖，让slider接收事件
                            onClicked: mouse.accepted=false
                            onPressed: mouse.accepted=false
                            onPressAndHold: mouse.accepted=false

                        }

                        background: Rectangle {
                            id: rect1
                            width: control.availableWidth
                            anchors.verticalCenter: parent.verticalCenter
                            radius: 7
                            color: "gray"
                            height: 5

                            Rectangle {
                                id: rect2
                                width: control.visualPosition * rect1.width
                                height: rect1.height
                                color: "steelblue"
                                radius: 7
                            }
                        }

                        handle: Rectangle {

                            id: handle111
                            x: control.visualPosition * (control.availableWidth - implicitWidth)
                            anchors.verticalCenter: parent.verticalCenter
                            implicitWidth: 24
                            implicitHeight: 24
                            radius: 12
                            border.color: "steelblue"
                            border.width: 2
                            color: control.pressed ? "steelblue" : "white"
                        }

                        Rectangle {
                            visible: false

                            id: yulan
                            width: 150
                            height: 100
                            x: handle111.x
                            y: handle111.y - 110

                            ThumbnailShow{
                                id: thumbnailShow;
                                anchors.centerIn: parent;
                                nWidth: 150;
                                nHeight: 100;
                                width: 150;
                                height: 100;
                            }
                        }
                        onValueChanged: {
                            if(control.pressed){
                                //改变播放进度
                                videoShow.setProcess(control.visualPosition)
                            }
                            overALLRectangle.forceActiveFocus()
                        }
                    }

                    //绑定进度条属性
                    Binding{
                        target: control
                        property: "value"
                        value: videoShow.process
                    }

                    //绑定进度条播放时间
                    Binding{
                        target: lefTime
                        property: "text"
                        value: videoShow.leftTime
                    }

                    //绑定播放总时长
                    Binding{
                        target: rightTime
                        property: "text"
                        value: videoShow.rightTime
                    }

                    //绑定专辑信息
                    Binding{
                        target: albumTitle
                        property: "text"
                        value: videoShow.title
                    }
                    Binding{
                        target: albumName
                        property: "text"
                        value: videoShow.album
                    }
                    Binding{
                        target: albumArtist
                        property: "text"
                        value: videoShow.title
                    }
                    Binding{
                        target: albumImage
                        property: "source"
                        value: videoShow.imagePath
                    }

                    Text {
                        id: rightTime
                        color: "#ffffff"
                        text: "00:00"
                        anchors.verticalCenter: parent.verticalCenter
                        font.pixelSize: 20
                        Layout.rightMargin: 30
                    }
                }

                RowLayout {
                    id:control_buttons
                    anchors.left: parent.left
                    anchors.right: parent.right
                    anchors.top:slider.bottom

                    Image {
                        source: "images/M_left.png"
                        anchors.right: lastFrame.left
                        anchors.rightMargin: 10

                        Layout.preferredHeight: 30
                        Layout.preferredWidth: 30
                        RoundButton {
                            id:lastPlayBtn
                            //anchors.fill: parent
                            flat: true
                            ToolTip.visible: hovered
                            ToolTip.text: qsTr("上一集")
                            function lastPlayBtnClicked()
                            {
                                if(nowIsPlayingAudio){
                                    yinpinplaylist.playLastMedia(0)
                                }
                                else{
                                    shipinplaylist.playLastMedia(0)
                                }
                            }

                            onClicked: {
                                lastPlayBtn.lastPlayBtnClicked()
                            }
                        }
                    }
                    Image {
                        id:lastFrame
                        Layout.preferredHeight: 40
                        Layout.preferredWidth: 40
                        anchors.right: playbuttonimage.left
                        anchors.rightMargin: 10
                        source: "../content/images/13.png"
                        RoundButton {
                            id:lastFrameButton
                            anchors.fill: parent
                            flat: true
                            ToolTip.visible: hovered
                            ToolTip.text: qsTr("上一帧")
                            onClicked: {
                                videoShow.seekSlow()
                                overALLRectangle.forceActiveFocus()
                            }
                        }
                    }
                    Image {
                        id:playbuttonimage
                        source: "../content/images/play.png"
                        anchors.centerIn: parent
                        Layout.preferredHeight: 20
                        Layout.preferredWidth: 20
                        RoundButton {
                            id:playbutton
                            anchors.fill: parent
                            flat: true
                            focus: false

                            ToolTip.visible: hovered
                            ToolTip.text: qsTr("播放")
                            function playButtonActivate()
                            {
                                //如果当前处于暂停状态，则播放一个视频
                                if(videoShow.isStop())
                                {
                                    console.log("Stop状态");  //很怪，只有程序刚启动，什么都没播放过时，点击播放按钮，才会进入这个if
                                }
                                else
                                {
                                    videoShow.pause();
                                    console.log("pause状态")
                                    if(videoShow.isPaused())
                                    {
                                        playbuttonimage.source="../content/images/play.png"
                                        playbutton.ToolTip.text=qsTr("播放")
                                    }
                                    else
                                    {
                                        playbuttonimage.source="../content/images/pause.png"
                                        playbutton.ToolTip.text=qsTr("暂停")
                                    }
                                }
                            }
                            onClicked: {
                                playbutton.playButtonActivate()
                            }
                        }
                    }
                    Image {
                        id:nextFrame
                        Layout.preferredHeight: 40
                        Layout.preferredWidth: 40
                        anchors.left: playbuttonimage.right
                        anchors.leftMargin: 10
                        source: "../content/images/12.png"
                        RoundButton {
                            id:nextFrameButton
                            anchors.fill: parent
                            flat: true
                            ToolTip.visible: hovered
                            ToolTip.text: qsTr("下一帧")
                            onClicked: {
                                videoShow.seekFast()
                                overALLRectangle.forceActiveFocus()
                            }
                        }
                    }
                    Image {
                        id:nextPlay
                        source: "images/M_right.png"
                        Layout.preferredHeight: 30
                        Layout.preferredWidth: 30
                        anchors.left:nextFrame.right
                        anchors.leftMargin: 10
                        RoundButton {
                            id: nextPlayBtn
                            //anchors.fill: parent
                            flat: true
                            ToolTip.visible: hovered
                            ToolTip.text: qsTr("下一集")
                            function nextPlayBtnClicked()
                            {
                                if(nowIsPlayingAudio){
                                    yinpinplaylist.playNextMedia(0)
                                }
                                else{
                                    shipinplaylist.playNextMedia(0)
                                }
                            }

                            onClicked: {
                                console.log("下一首被点击")
                                nextPlayBtn.nextPlayBtnClicked()
                            }
                        }
                    }
                    Image {
                        id:full_screen
                        Layout.preferredHeight: 25
                        Layout.preferredWidth: 25
                        source: "images/full_screen(1).png"
    //                    Layout.rightMargin: 12
                        anchors.right: parent.right
                        anchors.rightMargin: 18
                        MouseArea {
                            id:full_screen_MouseArea
                            anchors.fill: parent
                            function full_screenClicked()
                            {
                                if(!isFullScreen){
                                    showFullScreen()
                                    if(leftList.open)
                                    {
                                       leftList.open = false
                                       leftarrow.visible = false
                                    }else{
                                       leftarrow.visible = false
                                    }

                                    mainWin.isFullScreen = true
                                }else{
                                    showNormal()
                                    if(!leftList.open){
                                        leftList.open = true
                                    }
                                    mainWin.isFullScreen = false
                                }
                                basePic.visible = !basePic.visible
                                videoShow.fullScreen = !videoShow.fullScreen
                            }
                            onClicked: {
                                full_screenClicked()
                            }
                        }
                    }
                    Image {
                        id:playmodeimage
                        source: "images/singlePlay.png"
                        anchors.left:nextPlay.right
                        anchors.leftMargin: 25
                        Layout.preferredHeight: 35
                        Layout.preferredWidth: 35
                        RoundButton {
                            id:playModeBtn
                            anchors.fill: parent
                            flat: true
                            ToolTip.visible: hovered
                            ToolTip.text: qsTr("改变播放模式")
                            onClicked: {
                                yinpinplaylist.changePlayMode()
                                shipinplaylist.changePlayMode()
                            }
                        }
                    }

                    Image {
                        id: cutOff
                        source: "images/cut.png"
                        anchors.left:playmodeimage.right
                        anchors.leftMargin: 60
                        Layout.preferredHeight: 35
                        Layout.preferredWidth: 35
                        RoundButton {
                            id:cutBtn
                            anchors.fill: parent
                            flat: true
                            ToolTip.visible: hovered
                            ToolTip.text: qsTr("截图")
                            onClicked: {
                                videoShow.cutOff()
                            }
                        }
                    }

                    Image {
                        id:voice_button
                        Layout.preferredHeight: 35
                        Layout.preferredWidth: 35
                        source: "images/11.png"
                        anchors.left: parent.left
                        anchors.leftMargin: 12
                        Slider {
                            visible: false
                            id: voice
                            y: -80
                            height: 10
                            anchors.horizontalCenter: parent.horizontalCenter
                            value: 0.5
                            rotation: 270
                            onValueChanged: {
                                //修改音量
                                console.log("当前音量：",value * 100)
                                videoShow.setVolume(value * 100)
                                overALLRectangle.forceActiveFocus()
                            }
                            MouseArea{
                                anchors.fill:parent
                                hoverEnabled: true
                                onPositionChanged: {
                                    if(timer.running){
                                        timer.stop()
                                    }
                                }
                                onExited: {
                                    timer.start()
                                    //voice.visible = false
                                }
                                propagateComposedEvents :true
                                //释放鼠标事件的覆盖，让slider接收事件
                                onClicked: mouse.accepted=false
                                onPressed: mouse.accepted=false
                                onPressAndHold: mouse.accepted=false
                            }
                            background: Rectangle{
                                width:voice.availableWidth
                                height:voice.availableHeight
                                anchors.horizontalCenter: parent.horizontalCenter
                                color:"gray"
                                radius: 4
                                //rotation: 270

                                Rectangle{
                                    width:voice.visualPosition * parent.width
                                    height: voice.availableHeight
                                    color: "steelblue"
                                    radius: 4
                                }
                            }
                            handle: Rectangle{
                                x:voice.visualPosition * (voice.availableWidth - implicitHeight)
                                anchors.verticalCenter: parent.verticalCenter
                                implicitWidth: 20
                                implicitHeight: 20
                                radius:10
                                border.color: "steelblue"
                                border.width: 4
                                color: voice.pressed?"steelblue":"white"
                            }
                        }

                        MouseArea {
                            anchors.fill: parent
                            onClicked: {
                                voice.visible = !voice.visible
                            }
                        }
                    }
                    Image {
                        id:speedImage
                        source: "images/speed100.png"
                        Layout.preferredHeight: 30
                        Layout.preferredWidth: 30
                        //anchors.verticalCenter: parent.verticalCenter
                        anchors.left:voice_button.right
                        anchors.leftMargin: 12
                        MouseArea {
                            anchors.fill: parent
                            onClicked: {
                                speedBox.visible = !speedBox.visible
                            }
                        }

                        Rectangle {
                            visible: false
                            id: speedBox
                            y: -250
                            width: 65
                            height: 220
                            color: "grey"
                            radius: 5
                            anchors.horizontalCenterOffset: 0
                            anchors.horizontalCenter: parent.horizontalCenter
                            Column {
                                id: column
                                anchors.fill: parent
                                anchors.topMargin: 10
                                spacing: 10
                                Text {
                                    color: "white"

                                    text: '8.0x'
                                    font.pixelSize: 15
                                    anchors.horizontalCenter: parent.horizontalCenter
                                    MouseArea {
                                        anchors.fill: parent
                                        onClicked: {
                                            speedBox.visible = false
                                            videoShow.setSpeed(8.00)
                                            speedImage.source="images/speed800.png"
                                        }
                                    }
                                }
                                Text {
                                    color: "white"

                                    text: '4.0x'
                                    font.pixelSize: 15
                                    anchors.horizontalCenter: parent.horizontalCenter
                                    MouseArea {
                                        anchors.fill: parent
                                        onClicked: {
                                            speedBox.visible = false
                                            videoShow.setSpeed(4.00)
                                            speedImage.source="images/speed400.png"
                                        }
                                    }
                                }
                                Text {
                                    color: "white"

                                    text: '2.0x'
                                    font.pixelSize: 15
                                    anchors.horizontalCenter: parent.horizontalCenter
                                    MouseArea {
                                        anchors.fill: parent
                                        onClicked: {
                                            speedBox.visible = false
                                            videoShow.setSpeed(2.00)
                                            speedImage.source="images/speed200.png"
                                        }
                                    }
                                }
                                Text {
                                    color: "white"

                                    text: '1.5x'
                                    font.pixelSize: 15
                                    anchors.horizontalCenter: parent.horizontalCenter
                                    MouseArea {
                                        anchors.fill: parent
                                        onClicked: {
                                            speedBox.visible = false
                                            videoShow.setSpeed(1.50)
                                            speedImage.source="images/speed150.png"
                                            overALLRectangle.forceActiveFocus()
                                        }
                                    }
                                }
                                Text {
                                    color: "white"
                                    text: '1.0x'
                                    font.pixelSize: 15
                                    anchors.horizontalCenter: parent.horizontalCenter
                                    MouseArea {
                                        anchors.fill: parent
                                        onClicked: {
                                            speedBox.visible = false
                                            videoShow.setSpeed(1.00)
                                            speedImage.source="images/speed100.png"
                                        }
                                    }
                                }
                                Text {
                                    color: "white"
                                    text: '0.75x'
                                    font.pixelSize: 15
                                    anchors.horizontalCenter: parent.horizontalCenter
                                    MouseArea {
                                        anchors.fill: parent
                                        onClicked: {
                                            speedBox.visible = false
                                            videoShow.setSpeed(0.75)
                                            speedImage.source="images/speed075.png"
                                        }
                                    }
                                }
                                Text {
                                    color: "white"
                                    text: '0.5x'
                                    font.pixelSize: 15
                                    anchors.horizontalCenter: parent.horizontalCenter
                                    MouseArea {
                                        anchors.fill: parent
                                        onClicked: {
                                            speedBox.visible = false
                                            videoShow.setSpeed(0.50)
                                            speedImage.source="images/speed050.png"
                                        }
                                    }
                                }
                            }
                        }
                    }
                }
            }
        }
        Image {
            anchors.right: parent.right
            anchors.top: parent.top
            source: "images/more1.png"
            anchors.rightMargin: 50
            anchors.topMargin: 40
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
                    function toppingyinpin(idx)
                    {
                        console.log("置顶音频,index="+idx)
                        yinpinmodel.move(idx,0,1)
                        yinpinplaylist.toppingFile(idx)
                        overALLRectangle.forceActiveFocus()
                    }
                    function toppingshipin(idx)
                    {
                        console.log("置顶视频,index="+idx)
                        shipinmodel.move(idx,0,1)
                        shipinplaylist.toppingFile(idx)
                        overALLRectangle.forceActiveFocus()
                    }
                    onTriggered: {
                        if(nowIsPlayingAudio)
                        {
                            toppingyinpin(yinpinlistview.currentIndex)
                        }
                        else
                        {
                            toppingshipin(shipinlistview.currentIndex)
                        }
                    }
                }
                MenuItem {
                    text: '设置'
                    onTriggered: {
                        setting_for_rup.show()
                        overALLRectangle.forceActiveFocus()
                    }
                }
                MenuItem {
                    text: '详细信息'
                    onTriggered: {
                        if(nowIsPlayingAudio){
                            subWindow2.infoMap = yinpinplaylist.getMediaInfo(yinpinplaylist.getNowIndex(),"music")
                        }else{
                            subWindow2.infoMap = shipinplaylist.getMediaInfo(shipinplaylist.getNowIndex(),"video")
                        }
                        subWindow2.show()
                        overALLRectangle.forceActiveFocus()
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

