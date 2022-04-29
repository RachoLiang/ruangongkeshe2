#ifndef CAPTION_H
#define CAPTION_H

#endif // CAPTION_H

class caption
{
public:
    caption() {}
    caption(QString captionsPath);
    bool readCaption();     //读取字幕文件
    bool analyzeCaption();  //解析字幕文本
    void setCaptionsPath(QString captionPath);  //设置字幕文件路径
    void getCaptionsText();  //获取字幕文本
    void getCaptionsTime();  //获取字幕时间

private:
    QString captionPath;    //字幕文件路径
    QList<QString> captionText; //字幕文本
    QList<int> captionTime;     //字幕对应的播放时间

};
