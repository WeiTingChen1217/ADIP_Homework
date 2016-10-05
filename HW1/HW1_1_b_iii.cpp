//
//  main.cpp
//  HW1
//
//  Created by WeiTingChen on 10/1/16.
//  Copyright © 2016 WeiTingChen. All rights reserved.
//

#include <iostream>
#define _CRT_SECURE_NO_DEPRECATE // 去除 fopen 報警告
#include <stdio.h>
#include <opencv/cv.h>
#include <opencv/highgui.h>

int main(int argc, const char * argv[])
{
    int size = 256*256;
    char FileNameOri[] = "../../doc/HW1/lena256.raw";

    /// 建立一個單通道256*256大小的
    CvMat *lenamat              = cvCreateMat(256, 256, CV_8UC1);
    
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
        /// Display lenamat
        cvNamedWindow("lena_original", 0);
        cvResizeWindow("lena_original", 256, 256);
        cvMoveWindow("lena_original", 0, 0);
        cvShowImage("lena_original", lenamat);
        printf("Display lena_original\n");
        
        cvWaitKey(0);
        
        cvDestroyWindow("lena_original");
       

        fclose(lena);
        
        delete lenai;
   }
    
    return 0;
}
