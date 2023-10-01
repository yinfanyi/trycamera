# trycamera
用了c++17的一些特性

写了个camera类，直接获取深度图帧和彩色图帧
```cpp
auto camera = Camera::getInstance();
camera->start();
std::shared_ptr<ob::DepthFrame> depthFrame;
std::shared_ptr<ob::ColorFrame> colorFrame;
```
获取cv格式的彩色图
```cpp
cv::Mat color_image = camera->colorImage;
```
获取特定像素点的对应深度
```cpp
auto distance = camera->getDistance(320, 200); //获取像素点320，200的深度，深度图的像素为640*400，所以这个是获得中点的深度
```

TODO：有时会发生溢出错误，正在寻找原因

