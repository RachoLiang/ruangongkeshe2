import QtQuick

import QtQuick.Controls
import QtQuick.Window

Window{
    id: pop_window
    visible: false
    color: "transparent"
    // 透明度
    opacity: 0
    // 取消边框
    flags:Qt.FramelessWindowsHint | Qt.ToolTip
    // 设置为非模态
    modality: Qt.NonModal
    //设置初始位置(外部设置会覆盖此设置)
    x: 0
    y: 100
    //根据Loader设置大小
    width: content_loader.width
    height: content_loader.height
    property int true_x: 1350

    MouseArea{
        id:content_mouse
        anchors.fill: parent
        hoverEnabled: true
    }

    // 设置出现后显示时间的计时器
    Timer{
        id:show_timer
        interval: 1000
        repeat: true
        onTriggered:{
            //当未捕获到鼠标停留事件时，关闭窗口
            if(!loader_mouse.containsMouse){
                console.log("捕获到了信号")
                hideWindow();
            }
        }
    }

    //加载外部组件的占位组件
    property alias source_component:content_loader.sourceComponent
    Loader{
        id:content_loader
        MouseArea{
            id:loader_mouse
            anchors.fill:parent
            hoverEnabled:true
        }
    }

//---------------------------动画部分开始-------------------
    //设置出现显示动画
    property int x_offset: 0
    //设置sign标志，防止多次showWindow调用造成多次动画显示
    property int sign: 0
    ParallelAnimation{
        id: show_anim
        //透明度变化
        PropertyAnimation{
            target:pop_window
            property: "opacity"
            easing.type: Easing.InQuart
            from:pop_window.opacity
            to: 1
            duration:350
        }
        //位置移动动画
        PropertyAnimation{
            target:pop_window
            property: "x"
            easing.type: Easing.OutBounce
            //从当前值开始移动
            from: 600
            to: true_x - content_loader.width - x_offset
            duration:800
        }
        onStarted:{
            pop_window.show()
        }
        //出现动画结束信号
        onFinished:{
            console.log("开始结束")
            show_timer.start()
        }
    }

    //关闭动画
    ParallelAnimation{
        id: hide_anim
        //透明度变化
        PropertyAnimation{
            target:pop_window
            property: "opacity"
            easing.type: Easing.OutCubic
            from:pop_window.opacity
            to: 0
            duration:800
        }
        //位置移动
        PropertyAnimation{
            target:pop_window
            property: "x"
            easing.type: Easing.InExpo
            //从当前值开始移动
            from: pop_window.x
            to: 600
            duration:800
        }
        //结束动画结束之后隐藏窗口
        onFinished:{
            //复原popWindow的x位置
            show_timer.stop()
            pop_window.disattack()
        }
    }

    //显示弹窗
    function showWindow(){
        if(sign === 0){
            //恢复大小
            pop_window.width = content_loader.width
            pop_window.height = content_loader.height
            console.log("开始动了：",pop_window.x)
            show_anim.start()
            sign = 1
        }
    }

    //隐藏弹窗
    function hideWindow(){
        if(sign === 1){
            show_anim.stop()
            hide_anim.start()
            sign = 0
        }
    }

    function disattack(){
        pop_window.width = 0
        pop_window.height = 0
    }
//---------------------------动画部分结束-------------------
}

