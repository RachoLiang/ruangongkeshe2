# 一、环境介绍
操作系统: win10 64位
QT版本:  QT5.12.6
编译器:  MinGW 32
FFMPEG版本: 4.2.2
win32下使用FFMPEG 4.2.2库下载地址：https://download.csdn.net/download/xiaolong1126626497/12321684

# 二、功能说明
实现功能:  实现视频倒放功能，就是视频倒着播放。
说明:   因为视频本身编码特性，解码只能顺序解码。 
思路:   首先得确定一个起始位置，如果默认就从视频结尾向前播放，那么这个起始位置就是视频结尾的值； 然后使用av_seek_frame 向后偏移指定的时间(比如2秒)，然后在向前正常读取视频帧解码，并将数据保存到队列。 当数据读取到这个起始位置之后，就停止读取，将队列里的数据倒着取出来渲染到屏幕上，显示完毕继续重复即可。因为倒放一般只是处理图像，不处理音频，所以代码里没有对音频进行处理。

CSDN地址:  https://blog.csdn.net/xiaolong1126626497/article/details/115466526

# 三、运行示例
![image](https://img-blog.csdnimg.cn/20210510181227495.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3hpYW9sb25nMTEyNjYyNjQ5Nw==,size_16,color_FFFFFF,t_70)

# 四、演示效果
![image](https://img-blog.csdnimg.cn/20210511094224781.gif)

