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
        
        /// 顯示(200, 156)的亮度
        printf("Intensity values of (200, 156) : %d\n", lenai[256*199+155]);
    }
    
    return 0;
}
