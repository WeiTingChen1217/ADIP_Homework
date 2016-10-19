//
//  HW2_2_b.cpp
//  ADIP_Homework
//
//  Created by WeiTingChen on 10/18/16.
//  Copyright © 2016 WeiTingChen. All rights reserved.
//

#include <stdio.h>
#include <iostream>
#include <opencv2/opencv.hpp>

CvMat *lenamat;

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

int main(){
    // insert code here...
    std::cout << "This is homework2_2_a.\n\n";
    
    float size = 256;
    char FileNameOri[] = "../../doc/HW1/lena256.raw";
    int sizeSrc_width = size;
    int sizeSrc_height = size;
    OpenRAW(FileNameOri, sizeSrc_width, sizeSrc_height);
    
    
    
    cv::Mat matSrc, matDst1;

    for (int d = 0; d < 2; d++) {
        // Initialize
        matSrc = cv::cvarrToMat(lenamat, true); //lenamat copy to matSrc
        matDst1 = cv::Mat(cv::Size(sizeSrc_width, sizeSrc_width), matSrc.type(), cv::Scalar::all(0));
//        // 從最低為元開始移除
//        unsigned char a = 0x40;
//        a = ~a;
        unsigned char a = 0x01;
        a = ~a;
        
        for (int z = 0; z < 7; z++){
            printf("%x\n", a);
            for (int i = 0; i < matDst1.cols; ++i)
                for (int j = 0; j < matDst1.rows; ++j){
                    matDst1.at<uchar>(j, i) = matSrc.at<uchar>(j, i) & a; // 只抽掉一個 bit 做顯示
//                    // 觀察每個 bit 的型態
//                    if ((matSrc.at<uchar>(j, i) & a) == 0) {
//                        matDst1.at<uchar>(j, i) = 0;
//                    }
//                    else{
//                        matDst1.at<uchar>(j, i) = 255;
//                    }
                }

            
                a = ~a;
                a = a << 1;
                a = ~a;


            /// Display the method from MySelf
            namedWindow("Display_matDst1", cv::WINDOW_AUTOSIZE);
            cvMoveWindow("Display_matDst1", 0, 0);
            imshow("Display_matDst1", matDst1);
            printf("Display matDst1\n");
            
            cvWaitKey(0);
            
            cvDestroyWindow("Display_matDst1");
            
            char str1[30];
            char str2[10];
            strcpy(str1, "../../HW2/output/2_b_");
            strcat(str1, "2_");
            sprintf(str2, "%d", d);
            strcat(str1, str2);
            strcat(str1, "_");
            sprintf(str2, "%d", z);
            strcat(str1, str2);
            strcat(str1, ".jpg");
            puts(str1);
            // Save image
            cv::imwrite(str1, matDst1);
        }
      char FileNameOri_baboon[] = "../../doc/HW2/baboon_256.raw";
      OpenRAW(FileNameOri_baboon, sizeSrc_width, sizeSrc_height);
    }
    return 0;
}