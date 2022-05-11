#ifndef PLAYLISTNODE_H
#define PLAYLISTNODE_H
struct PlayListNode  //vector和deque中的元素，暂时只放文件路径，待修改完善
{
    int mediaType;
    QString filePath;
    //    Audio *audioPtr;
    //    Video *videoPtr;
    //    PlayListNode(int MediaType=1,Audio *AudioPtr=nullptr,Video *VideoPtr=nullptr)
    //    {
    //        mediaType=MediaType;
    //        audioPtr=AudioPtr;
    //        videoPtr=VideoPtr;
    //    }
};
#endif // PLAYLISTNODE_H
