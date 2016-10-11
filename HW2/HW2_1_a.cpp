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
    /// Display matSrc
    namedWindow("Display_matSrc", cv::WINDOW_AUTOSIZE);
    cvMoveWindow("Display_matSrc", 0, 0);
    imshow("Display_matSrc", matSrc);
    printf("Display matSrc\n");
    /// Display matDst1
    namedWindow("Display_matDst1", cv::WINDOW_AUTOSIZE);
    cvMoveWindow("Display_matDst1", 0, 0);
    imshow("Display_matDst1", matDst1);
    printf("Display matDst1\n");
    /// Display matDst2
    namedWindow("Display_matDst2", cv::WINDOW_AUTOSIZE);
    cvMoveWindow("Display_matDst2", sizDst_width, 0);
    imshow("Display_matDst2", matDst2);
    printf("Display matDst2\n");

    cvWaitKey(0);
    
    cvDestroyWindow("Display_matSrc");
    cvDestroyWindow("Display_matDst1");
    cvDestroyWindow("Display_matDst2");
}

void nearest_neighbor(cv::Mat& matSrc, cv::Mat& matDst1, cv::Mat& matDst2, double scale_x, double scale_y){
    std::cout << "Doing nearest neighbor interpolation.\n";
    
    for (int i = 0; i < matDst1.cols; ++i)
    {
        int sx = cvFloor(i * scale_x);  // 無條件捨去
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
    
    showImage(matSrc, matDst1, matDst2);
    
//    // Save image
//    cv::imwrite("../../HW2/nearest_1.jpg", matDst1);
//    printf("Save matDst1 as nearest_1.jpg\n");
//    cv::imwrite("../../HW2/nearest_2.jpg", matDst2);
//    printf("Save matDst2 as nearest_2.jpg\n");

    std::cout << "nearest neighbor interpolation done!!.\n\n";
}

void bilinear(cv::Mat& matSrc, cv::Mat& matDst1, cv::Mat& matDst2, double scale_x, double scale_y){
    std::cout << "Doing bilinear interpolation.\n";

    uchar* dataDst = matDst1.data;  // 將記憶體位置給dataDst，也就是dataDst等於matDst的圖片
    int stepDst = (int)matDst1.step;    // (int)matDst1.step也就是取matDst1.step[0]的意思; 代表一列所有的數據大小; 以256*256灰階圖為例，matDst1.step[0]=matDst1.cols*matDst1.Size()=256*1=256
    uchar* dataSrc = matSrc.data;
    int stepSrc = (int)matSrc.step;
    int iWidthSrc = matSrc.cols;
    int iHiehgtSrc = matSrc.rows;

    for (int j = 0; j < matDst1.rows; ++j)
    {
        float fy = (float)((j + 0.5) * scale_y - 0.5);
        int sy = cvFloor(fy);
        fy -= sy;   // 取得差值的小數點
        sy = std::min(sy, iHiehgtSrc - 2);
        sy = std::max(0, sy);
        
        short cbufy[2];
        cbufy[0] = cv::saturate_cast<short>((1.f - fy) * 2048);// 使數值不要溢位
        cbufy[1] = 2048 - cbufy[0];
        
        for (int i = 0; i < matDst1.cols; ++i)
        {
            float fx = (float)((i + 0.5) * scale_x - 0.5);
            int sx = cvFloor(fx);
            fx -= sx;
            
            if (sx < 0) {
                fx = 0, sx = 0;
            }
            if (sx >= iWidthSrc - 1) {
                fx = 0, sx = iWidthSrc - 2;
            }
            
            short cbufx[2];
            cbufx[0] = cv::saturate_cast<short>((1.f - fx) * 2048);
            cbufx[1] = 2048 - cbufx[0];
            
//            for (int k = 0; k < matSrc.channels(); ++k)
//            {
//                *(dataDst+ j*stepDst + 3*i + k) = (
//                *(dataSrc +  sy   *stepSrc + 3* sx    + k) * cbufx[0] * cbufy[0] +
//                *(dataSrc + (sy+1)*stepSrc + 3* sx    + k) * cbufx[0] * cbufy[1] +
//                *(dataSrc +  sy   *stepSrc + 3*(sx+1) + k) * cbufx[1] * cbufy[0] +
//                *(dataSrc + (sy+1)*stepSrc + 3*(sx+1) + k) * cbufx[1] * cbufy[1]
//                                                   ) >> 22;
//            }

            *(dataDst+ j*stepDst + i) = (
                *(dataSrc +  sy   *stepSrc +  sx   ) * cbufx[0] * cbufy[0] +
                *(dataSrc + (sy+1)*stepSrc +  sx   ) * cbufx[0] * cbufy[1] +
                *(dataSrc +  sy   *stepSrc + (sx+1)) * cbufx[1] * cbufy[0] +
                *(dataSrc + (sy+1)*stepSrc + (sx+1)) * cbufx[1] * cbufy[1]
                                               ) >> 22;

        }
    }
    cv::resize(matSrc, matDst2, matDst1.size(), 0, 0, cv::INTER_LINEAR);

    showImage(matSrc, matDst1, matDst2);
    
//    // Save image
//    cv::imwrite("../../HW2/linear_1.jpg", matDst1);
//    printf("Save matDst1 as linear_1.jpg\n");
//    cv::imwrite("../../HW2/linear_2.jpg", matDst2);
//    printf("Save matDst2 as linear_1.jpg\n");
    
    std::cout << "Bilinear interpolation done!!.\n\n";
}

void bicubic(cv::Mat& matSrc, cv::Mat& matDst1, cv::Mat& matDst2, double scale_x, double scale_y){
    
}

int main(int argc, const char * argv[]) {
    // insert code here...
    std::cout << "This is homework2.\n\n";

    char FileNameOri[] = "../../doc/HW1/lena256.raw";
    int sizeSrc_width = 256;
    int sizeSrc_height = 256;
    OpenRAW(FileNameOri, sizeSrc_width, sizeSrc_height);
    
    
    cv::Mat matSrc, matDst1, matDst2;
    sizDst_width = 576;
    sizDst_height = 576;
//    matSrc = cv::imread("../../doc/lena_256.jpg", 2 | 4);
    //注意：当将参数copyData设为true后，则为深拷贝（复制整个图像数据）
    matSrc = cv::cvarrToMat(lenamat, true); //lenamat copy to matSrc
    matDst1 = cv::Mat(cv::Size(sizDst_width, sizDst_height), matSrc.type(), cv::Scalar::all(0));
    matDst2 = cv::Mat(matDst1.size(), matSrc.type(), cv::Scalar::all(0));
    
    double scale_x = (double)matSrc.cols / matDst1.cols;
    double scale_y = (double)matSrc.rows / matDst1.rows;
    
//    // Nearest neighbor
//    nearest_neighbor(matSrc, matDst1, matDst2, scale_x, scale_y);
    
//    // Bilinear
//    bilinear(matSrc, matDst1, matDst2, scale_x, scale_y);
    
    // Bicubic
    bicubic(matSrc, matDst1, matDst2, scale_x, scale_y);
    
    return 0;
}
