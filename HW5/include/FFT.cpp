//
//  FFT.cpp
//  ADIP_Homework
//
//  Created by WeiTingChen on 11/11/16.
//  Copyright © 2016 WeiTingChen. All rights reserved.
//

#include "FFT.hpp"

Mat FFT_opencv(Mat src, Mat img){
    
    Mat padded;
    int m = getOptimalDFTSize(src.rows);
    // m為大於等於inputImg.rows裡的最小值，且須為2、3、5的次方相乘
    int n = getOptimalDFTSize(src.cols);
    copyMakeBorder(src, padded,
                   0, m-src.rows, 0, n-src.cols,
                   BORDER_CONSTANT, // 外推的值為常數，常在仿射變換、透視變換中使用
                   Scalar::all(0)); // 為了效率，所以對影像邊界拓展
    
    Mat planes[] = {Mat_<float>(padded), Mat::zeros(padded.size(), CV_32F)};
    Mat complexImg;
    merge(planes, 2, complexImg);
    dft(complexImg, complexImg);
    
    
    split(complexImg, planes);
    // 分離通道，planes[0]為實數部分，planes[1]為虛數部分
    
    magnitude(planes[0], planes[1], planes[0]);
    // planes[0] = sqrt((planes[0])^2 + (planes[1])^2
    
    Mat magI = planes[0];
    magI += Scalar::all(1);
    // magI = log(1+planes[0])
    
    log(magI, magI);
    
    magI = magI(Rect(0, 0, magI.cols & -2, magI.rows & -2));  //令邊長為偶數
    
    //將區塊重排，讓原點在影像的中央
    int cx = magI.cols/2;
    int cy = magI.rows/2;
    
    Mat q0(magI, Rect(0, 0, cx, cy));   // II
    // 只 copy 矩陣 magI 的一小部分
    Mat q1(magI, Rect(cx, 0, cx, cy));  // I
    Mat q2(magI, Rect(0, cy, cx, cy));  // III
    Mat q3(magI, Rect(cx, cy, cx, cy)); // IV
    
    Mat tmp;
    q0.copyTo(tmp);
    q3.copyTo(q0);
    tmp.copyTo(q3);
    q1.copyTo(tmp);
    q2.copyTo(q1);
    tmp.copyTo(q2);
    
    normalize(magI, magI, 0, 1, CV_MINMAX);
    
    magI.convertTo(img, CV_8UC1, 255.0);
    
    return complexImg;
}