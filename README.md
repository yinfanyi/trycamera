# trycamera
用了c++17的一些特性

写了个camera类，直接获取深度图帧和彩色图帧
```cpp
auto camera = Camera::getInstance();
camera->start();
camera->receiveFrames();
```
获取cv格式的彩色图
```cpp
cv::Mat color_image = camera->colorImage;
```
获取特定像素点的对应深度
```cpp
auto distance = camera->getDistance(320, 200); //获取像素点320，200的深度，深度图的像素为640*400，所以这个是获得中点的深度
```
结束
```cpp
camera->stop();
```

TODO：有时或运行久了会发生错误：Segmentation fault（访问了无效的内存地址或者访问了未分配的内存导致），正在寻找出错的地方

