# screen-crack-detection
手机屏幕裂纹检测
先对图像进行canny变化，中值滤波，二值化，再对二值化的图像进行Hough变换检测手机边缘，缩进，再对缩进后的图像依据连通域来求出他们的面积