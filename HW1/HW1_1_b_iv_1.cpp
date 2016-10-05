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
    CvMat *lenamat_90Degree     = cvCreateMat(256, 256, CV_8UC1);
    
    /// 建立處理圖片的空間
    unsigned char *lenai;
    unsigned char *lenao_90Degree, *lenao_90Degree_2;
   
    lenai = new unsigned char[size];
    lenao_90Degree = new unsigned char[size];
    lenao_90Degree_2 = new unsigned char[size];
    
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
        
        /// 圖片90度翻轉 + 上下顛倒
        for(int x=0;x<256;x++)
            for(int y=0;y<256;y++)
            {
                lenao_90Degree[256*y+256-x] = lenai[256*x+y];//順時針旋轉90度
            }
        for(int x=0;x<256;x++)
            for(int y=0;y<256;y++)
            {
                lenao_90Degree_2[256*(255-x)+y] = lenao_90Degree[256*x+y];//上下鏡像
            }
        
        cvSetData(lenamat_90Degree, lenao_90Degree_2, lenamat_90Degree->step);
        
        /// Display 旋轉90度 + 上下顛倒
        cvNamedWindow("lena_90Degree", 0);
        cvResizeWindow("lena_90Degree", 256, 256);
        cvMoveWindow("lena_90Degree", 0, 310);
        cvShowImage("lena_90Degree", lenamat_90Degree);
        printf("Display lena_90Degree\n");
        
        cvWaitKey(0);
        
        cvDestroyWindow("lena_original");
        cvDestroyWindow("lena_90Degree");
      
        ///save the image
        FILE * name_Receive;
        // save lenamat_90Degree
        char FileNameRes_90Degree[] = "../../HW1/lena256_90Degree.raw";
        name_Receive = fopen(FileNameRes_90Degree, "wb");
        fwrite(lenao_90Degree_2, size, 1,  name_Receive);
        printf("lenamat_90Degree    save as lena256_90Degree.raw\n");

        fclose(lena);
        
        delete lenai;
        delete lenao_90Degree;
        delete lenao_90Degree_2;
    }
    
    return 0;
}
