import QtQuick 2.0
import VideoShow 1.0


Rectangle{
    width: dpW(500)
    height: dpH(800)
    border.width: 5
    border.color: "black"
    radius: 10
    VideoShow{
        anchors.centerIn: parent;
        nWidth: dpw(500);
        nHeight: dpH(800);
        width: dpW(500);
        height: dpH(800);
        sourPath: "D:/chat/wechat/WeChat Files/wxid_mnkxh12c0w5n22/FileStorage/Video/2022-04/0b6d50c476ea6be54ec82e2790ad3f90.mp4"
    }
}
