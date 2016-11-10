//
//  main.cpp
//  HW5
//
//  Created by WeiTingChen on 11/10/16.
//  Copyright © 2016 WeiTingChen. All rights reserved.
//

#include <iostream>
#include "OpenRawFile.hpp"

int main(int argc, const char * argv[]) {

    std::cout << "This is homework5_2_a.\n\n";
    
    /// Load file
    char str_input_image[] = "../../doc/HW5/sine4_128x128.raw";
    int sizeSrc_width = 128;
    int sizeSrc_height = 128;
    CvMat *image_cvMat = cvCreateMat(sizeSrc_width, sizeSrc_height, CV_8UC1);
    OpenRAW(str_input_image, image_cvMat);
    // Chamge the format to Mat
    Mat inputImg = cv::cvarrToMat(image_cvMat, true);

    Mat padded;
    int m = getOptimalDFTSize(inputImg.rows);
    // m為大於等於inputImg.rows裡的最小值，且須為2、3、5的次方相乘
    int n = getOptimalDFTSize(inputImg.cols);
    copyMakeBorder(inputImg, padded, 0, m-inputImg.rows, 0, n-inputImg.cols, BORDER_CONSTANT, Scalar::all(0)); //為了效率，所以對影像邊界拓展
    
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
    
    Mat q0(magI, Rect(0, 0, cx, cy));
    Mat q1(magI, Rect(cx, 0, cx, cy));
    Mat q2(magI, Rect(0, cy, cx, cy));
    Mat q3(magI, Rect(cx, cy, cx, cy));
    
    Mat tmp;
    q0.copyTo(tmp);
    q3.copyTo(q0);
    tmp.copyTo(q3);
    q1.copyTo(tmp);
    q2.copyTo(q1);
    tmp.copyTo(q2);
    
    normalize(magI, magI, 0, 1, CV_MINMAX);
    
    imshow("Imput", inputImg);
    moveWindow("Imput", 0, 0);

    imshow("Spectrum", magI);
    moveWindow("Spectrum", 0, 200);

    //逆向傅立葉轉換
    Mat ifft;  
    idft(complexImg,ifft,DFT_REAL_OUTPUT);  
    normalize(ifft,ifft,0,1,CV_MINMAX);
    
    imshow("Reverse to origion",ifft);
    moveWindow("Reverse to origion", 0, 400);
    
    waitKey(0);
    
    return 0;
}
