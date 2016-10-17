//
//  HW2_1_b.cpp
//  ADIP_Homework
//
//  Created by WeiTingChen on 10/17/16.
//  Copyright © 2016 WeiTingChen. All rights reserved.
//

#include <stdio.h>
#include <iostream>
#include "zoomingAndShrinking.hpp"

CvMat *lenamat;
int sizDst_width;
int sizDst_height;

void OpenRAW(char FileNameOri[], int width, int height){
    int size = width*height;
    /// 建立一個單通道256*256大小的
    lenamat = cvCreateMat(width, height, CV_8UC1);
    
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
}

void showImage(cv::Mat& matSrc, cv::Mat& matDst1, cv::Mat& matDst2){
    /// Display the sourse 
    namedWindow("Display_matSrc", cv::WINDOW_AUTOSIZE);
    cvMoveWindow("Display_matSrc", 0, 0);
    imshow("Display_matSrc", matSrc);
    printf("Display matSrc\n");
    /// Display the method from MySelf
    namedWindow("Display_matDst1", cv::WINDOW_AUTOSIZE);
    cvMoveWindow("Display_matDst1", 0, 0);
    imshow("Display_matDst1", matDst1);
    printf("Display matDst1\n");
    /// Display the method from OpenCV
    namedWindow("Display_matDst2", cv::WINDOW_AUTOSIZE);
    cvMoveWindow("Display_matDst2", sizDst_width, 0);
    imshow("Display_matDst2", matDst2);
    printf("Display matDst2\n");
    
    cvWaitKey(0);
    
    cvDestroyWindow("Display_matSrc");
    cvDestroyWindow("Display_matDst1");
    cvDestroyWindow("Display_matDst2");
}

int main(int argc, const char * argv[]) {
    // insert code here...
    std::cout << "This is homework2.\n\n";
    
    int size = 256;
    char FileNameOri[] = "../../doc/HW1/lena256.raw";
    int sizeSrc_width = size;
    int sizeSrc_height = size;
    OpenRAW(FileNameOri, sizeSrc_width, sizeSrc_height);
    
    size = 200; // Zooming to size of 200x200
    cv::Mat matSrc, matDst1, matDst2;
    sizDst_width = size;
    sizDst_height = size;
# ifdef color
    matSrc = cv::imread("../../doc/lena_256.jpg", 2 | 4);
# else
    //注意：當將参数 copyData 設為 true 後，則為深拷貝（複製整個圖像數據）
    matSrc = cv::cvarrToMat(lenamat, true); //lenamat copy to matSrc
# endif
    matDst1 = cv::Mat(cv::Size(sizDst_width, sizDst_height), matSrc.type(), cv::Scalar::all(0));
    matDst2 = cv::Mat(matDst1.size(), matSrc.type(), cv::Scalar::all(0));
    
    double scale_x = (double)matSrc.cols / matDst1.cols;
    double scale_y = (double)matSrc.rows / matDst1.rows;
    
    /// Nearest neighbor
    nearest_neighbor(matSrc, matDst1, matDst2, scale_x, scale_y);
    showImage(matSrc, matDst1, matDst2);
    // Save image
    cv::imwrite("../../HW2/output/nearest_1_b.jpg", matDst1);
    printf("Save matDst1 as nearest_1_b.jpg\n");
//    cv::imwrite("../../HW2/output/nearest_2_b.jpg", matDst2);
//    printf("Save matDst2 as nearest_2_b.jpg\n");
    
    /// Bilinear
    bilinear(matSrc, matDst1, matDst2, scale_x, scale_y);
    showImage(matSrc, matDst1, matDst2);
    // Save image
    cv::imwrite("../../HW2/output/linear_1_b.jpg", matDst1);
    printf("Save matDst1 as linear_1_b.jpg\n");
//    cv::imwrite("../../HW2/output/linear_2_b.jpg", matDst2);
//    printf("Save matDst2 as linear_2_b.jpg\n");
    
    /// Bicubic
    bicubic(matSrc, matDst1, matDst2, scale_x, scale_y);
    showImage(matSrc, matDst1, matDst2);
    // Save image
    cv::imwrite("../../HW2/output/bicubic_1_b.jpg", matDst1);
    printf("Save matDst1 as bicubic_1_b.jpg\n");
//    cv::imwrite("../../HW2/output/bicubic_2_b.jpg", matDst2);
//    printf("Save matDst2 as bicubic_2_b.jpg\n");
    
    return 0;
}

