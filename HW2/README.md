@ 本次作業目的：
1. 使用三種演算法實現圖片檔放大縮小。
    a. 將 lena256.raw 放大 2.25 倍，使用 nearest neighbor, bilinear, bicubic interpolation  方法。
    b. 同上，將圖片縮小 1.28 倍
    c. 放大 5.76 縮小 2.56, 縮小 2.56 放大 5.76, 放大 2.25, 比較其差異
    d. 說明"lanczos method" 與上述演算法的差異。
2. ...


@ 好的教學網站：
- http://yzu1022cs368s991412.blogspot.tw/2014/03/digital-image-processing1-scale-rotate.html


@ 解釋
- nearest_1.jpg - 使用 Nearest neighbor interpolation approaches 將圖片放大至576*576 的大小
- nearest_2.jpg - 使用 OpenCV API



@ OpenCV 中的 Resize 圖像大小變換
- 原型：
    void cv::resize(constCvArr*src,CvArr*dst,intinterpolation=CV_INTER_LINEAR);
- 參數：
    src输入图像.
    dst输出图像.
    interpolation 插值方法:
        CV_INTER_NN - 最近邻插值,
        CV_INTER_LINEAR - 双线性插值(缺省使用)
        CV_INTER_AREA - 使用象素关系, 重采样。当图像缩小时候，该方法可以避免波纹出现。当图像放大时，类似于CV_INTER_NN方法..
        CV_INTER_CUBIC - 立方插值.
        CV_INTER_LANCZOS4 - 兰索斯插值：由相邻的8*8像素计算得出。
- http://baike.baidu.com/view/7317818.htm


@ OpenCV：CvMat 转 Mat
- http://blog.csdn.net/holybin/article/details/17711013


@ 操作像素
- at()用來訪問像素，可返回左值或右值，所以我們可用at()得到或改變某個像素值，這函式使用模板，所以使用時除了輸入位置，還必須需入影像的像素型態，使用at()函式時，輸入參數順序同樣為先高再寬。
- Ex:
    - 8位元灰階圖
        Mat gray_img(100, 100, CV_8U, Scalar(100));
        gray_img.at<uchar>(30,20) =255;            
        uchar value1 = gray_img.at<uchar>(30,20); 
- http://monkeycoding.com/?p=531#i-6


