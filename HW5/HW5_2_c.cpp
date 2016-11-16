//
//  HW5_2_b.cpp
//  ADIP_Homework
//
//  Created by WeiTingChen on 11/16/16.
//  Copyright © 2016 WeiTingChen. All rights reserved.
//

#include <iostream>
#include "OpenRawFile.hpp"
#include "FFT.hpp"

int main() {
    
    std::cout << "This is homework5_2_c.\n\n";
    
    /// Load file
    char str_input_image_1[] = "../../doc/HW5/clown_128x128.raw";
    char str_input_image_2[] = "../../doc/HW5/clown_mask_128x128.raw";
    
    int sizeSrc_width = 128;
    int sizeSrc_height = 128;
    
    CvMat *image_cvMat_1 = cvCreateMat(sizeSrc_width, sizeSrc_height, CV_8UC1);
    CvMat *image_cvMat_2 = cvCreateMat(sizeSrc_width, sizeSrc_height, CV_8UC1);
    
    OpenRAW(str_input_image_1, image_cvMat_1);
    OpenRAW(str_input_image_2, image_cvMat_2);
    
    // Chamge the format to Mat
    Mat inputImg_1 = cv::cvarrToMat(image_cvMat_1, true);
    Mat inputImg_2 = cv::cvarrToMat(image_cvMat_2, true);
    
    
    Mat magI_1 = inputImg_1.clone();
    Mat ifft = inputImg_1.clone();
    Mat padded = inputImg_1.clone();

    Mat complexImg_1;
    
    FFT_opencv(inputImg_1, magI_1, &complexImg_1);
    
    Mat planes[] = {Mat_<float>(padded), Mat::zeros(padded.size(), CV_32F)};
    
    split(complexImg_1, planes);
    // 分離通道，planes[0]為實數部分，planes[1]為虛數部分
    
    
    //將區塊重排，讓原點在影像的中央
    int cx = inputImg_2.cols/2;
    int cy = inputImg_2.rows/2;
    
    Mat q0(inputImg_2, Rect(0, 0, cx, cy));   // II
    Mat q1(inputImg_2, Rect(cx, 0, cx, cy));  // I
    Mat q2(inputImg_2, Rect(0, cy, cx, cy));  // III
    Mat q3(inputImg_2, Rect(cx, cy, cx, cy)); // IV
    
    Mat tmp;
    q0.copyTo(tmp);
    q3.copyTo(q0);
    tmp.copyTo(q3);
    q1.copyTo(tmp);
    q2.copyTo(q1);
    tmp.copyTo(q2);

    
    for(int i = 0; i < magI_1.rows; i++)
        for(int j = 0; j < magI_1.cols; j++){
            planes[0].at<float>(i, j) *= inputImg_2.at<uchar>(i, j);
            planes[1].at<float>(i, j) *= inputImg_2.at<uchar>(i, j);

        }

    merge(planes, 2, complexImg_1);
    
    //逆向傅立葉轉換
    idft(complexImg_1, ifft, DFT_REAL_OUTPUT);
    normalize(ifft, ifft, 0, 1, CV_MINMAX);
    
    imshow("Imput_1", inputImg_1);
    moveWindow("Imput_1", 0, 0);
    imshow("Imput_2", inputImg_2);
    moveWindow("Imput_2", 130, 0);
    
    imshow("Spectrum_1", magI_1);
    moveWindow("Spectrum_1", 0, 200);
//    imshow("Spectrum_2", magI_2);
//    moveWindow("Spectrum_2", 130, 200);

    imshow("Reverse to origion", ifft);
    moveWindow("Reverse to origion", 0, 400);
    
    waitKey(0);
    
    ifft.convertTo(ifft, CV_8UC1, 255.0);
    // Save image
    imwrite("../../HW5/output/homework5_2_c.jpg", ifft);
    
    return 0;
}