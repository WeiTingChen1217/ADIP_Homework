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
    CvMat *lenamat_brightness   = cvCreateMat(256, 256, CV_8UC1);
    
    /// 建立處理圖片的空間
    unsigned char *lenai;
    unsigned char *lenao_brightness;
    
    lenai = new unsigned char[size];
    lenao_brightness = new unsigned char[size];
    
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
        
        /// 將圖片亮度調高80
        int w = 80;
        for(int x=0;x<256;x++)
            for(int y=0;y<256;y++)
            {
                int depth = lenai[256*x+y]+w;
                if(depth >=255)
                {
                    lenao_brightness[256*x+y] = 255;
                }
                else
                {
                    lenao_brightness[256*x+y] = depth;
                }
            }
        cvSetData(lenamat_brightness, lenao_brightness, lenamat_brightness->step);
        
        /// Display 條亮度
        cvNamedWindow("lena_brightness", 0);
        cvResizeWindow("lena_brightness", 256, 256);
        cvMoveWindow("lena_brightness", 260, 310);
        cvShowImage("lena_brightness", lenamat_brightness);
        printf("Display lena_brightness\n");
        
        cvWaitKey(0);
        
        cvDestroyWindow("lena_brightness");
        
        ///save the image
        FILE * name_Receive;
        // save lenamat_brightness
        char FileNameRes_brightness[] = "../../HW1/lena256_brightness.raw";
        name_Receive = fopen(FileNameRes_brightness, "wb");
        fwrite(lenao_brightness, size, 1, name_Receive);
        printf("lenamat_brightness  save as lena256_brightness.raw\n");

        fclose(lena);
        
        delete lenai;
        delete lenao_brightness;
    }
    
    return 0;
}
