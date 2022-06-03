#ifndef PLAYLISTNODE_H
#define PLAYLISTNODE_H
struct PlayListNode  //vector和deque中的元素，暂时只放文件路径，待修改完善
{
    int mediaType;      //音频为1，视频为2
    QString filePath;   //格式为 "C:\\a.mp3"
    int duration;       //时长
    int id;             //数据库表中的id
};
#endif // PLAYLISTNODE_H
