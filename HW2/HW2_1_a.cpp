//
//  main.cpp
//  HW2
//
//  Created by WeiTingChen on 10/7/16.
//  Copyright © 2016 WeiTingChen. All rights reserved.
//

#include <iostream>
#include <stdio.h>
//#include <opencv2/core/core.hpp>
//#include <opencv2/highgui/highgui.hpp>
#include <opencv2/opencv.hpp>

int main(int argc, const char * argv[]) {
    // insert code here...
    std::cout << "This is homework2.\n";

    char FileNameOri[] = "../../doc/HW1/lena256.raw";
    int size = 256*256;
    /// 建立一個單通道256*256大小的
    CvMat *lenamat = cvCreateMat(256, 256, CV_8UC1);
    
    /// 建立處理圖片的空間
    unsigned char *lenai;
    
    lenai = new unsigned char[size];

    FILE *lena;
    lena = fopen(FileNameOri, "rb");
    
    if(lena == NULL)
    {
        // 檢查檔案是否可以開啟
        puts("Loading File Error!");
    }
    else
    {
        fread(lenai, 1, size, lena);    // lenai 為要處理的影像; 第一個參數為陣列或結構的指標
        /// 顯示原始影像
        cvSetData(lenamat, lenai, lenamat->step);
    }
    
    
    cv::Mat matSrc, matDst1, matDst2;
    
//    matSrc = cv::imread("../../doc/lena_256.jpg", 2 | 4);
//    matSrc = cv::imread("../../doc/lena_256.jpg", 0);

    //注意：当将参数copyData设为true后，则为深拷贝（复制整个图像数据）
    matSrc = cv::cvarrToMat(lenamat, true); //lenamat copy to matSrc
    matDst1 = cv::Mat(cv::Size(576, 576), matSrc.type(), cv::Scalar::all(0));
    matDst2 = cv::Mat(matDst1.size()    , matSrc.type(), cv::Scalar::all(0));
    
    double scale_x = (double)matSrc.cols / matDst1.cols;
    double scale_y = (double)matSrc.rows / matDst1.rows;
    
    for (int i = 0; i < matDst1.cols; ++i)
    {
        int sx = cvFloor(i * scale_x);
        sx = std::min(sx, matSrc.cols - 1);
        for (int j = 0; j < matDst1.rows; ++j)
        {
            int sy = cvFloor(j * scale_y);
            sy = std::min(sy, matSrc.rows - 1);
//            matDst1.at<cv::Vec3b>(j, i) = matSrc.at<cv::Vec3b>(sy, sx);//for Color Image
            matDst1.at<uchar>(j, i) = matSrc.at<uchar>(sy, sx);//for Gray Image

        }
    }
    cv::resize(matSrc, matDst2, matDst1.size(), 0, 0, cv::INTER_NEAREST);


    /// Display lenamat
    namedWindow("Display_lenamat", cv::WINDOW_AUTOSIZE);
    cvMoveWindow("Display_lenamat", 0, 0);
    cvShowImage("Display_lenamat", lenamat);
    printf("Display_lenamat\n");
    /// Display matSrc
    namedWindow("Display_matSrc", cv::WINDOW_AUTOSIZE);
    cvMoveWindow("Display_matSrc", 0, 256);
    imshow("Display_matSrc", matSrc);
    printf("Display_matSrc\n");
    /// Display matDst1
    namedWindow("Display_matDst1", cv::WINDOW_AUTOSIZE);
    cvMoveWindow("Display_matDst1", 256, 0);
    imshow("Display_matDst1", matDst1);
    printf("Display_matDst1\n");
    /// Display matDst2
    namedWindow("Display_matDst2", cv::WINDOW_AUTOSIZE);
    cvMoveWindow("Display_matDst2", 832, 0);
    imshow("Display_matDst2", matDst2);
    printf("Display_matDst2\n");
    
    cvWaitKey(0);

    cv::imwrite("../../HW2/nearest_1.jpg", matDst1);
    
    cv::imwrite("../../HW2/nearest_2.jpg", matDst2);
    
    return 0;
}
