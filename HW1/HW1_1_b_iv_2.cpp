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
    CvMat *lenamat_cut          = cvCreateMat(256, 256, CV_8UC1);
   
    /// 建立處理圖片的空間
    unsigned char *lenai;
    unsigned char *lenao_90Degree, *lenao_90Degree_2;
    unsigned char *lenao_cut;
   
    lenai = new unsigned char[size];
    lenao_90Degree = new unsigned char[size];
    lenao_90Degree_2 = new unsigned char[size];
    lenao_cut = new unsigned char[size];
   
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
        
        /// 將 lena 切割排序
        int m1 = 0;
        int m2 = 0;
        for(int x=0;x<256;x++)
            for(int y=0;y<256;y++)
            {
                // 建立線性方程式切割圖片
                m1 = 127-x; // 切角：-
                m2 = y-127; // 切角：|
                
                if(m1>=0 && m2<=0)// II
                {
                    lenao_cut[256*(127-x)+(127-y)] = lenao_90Degree_2[256*x+y];
                }
                else if(m1<=0 && m2>=0)// IIII
                {
                    lenao_cut[256*(383-x)+(383-y)] = lenao_90Degree_2[256*x+y];
                }
                else if(m1<=0 && m2<=0)// III
                {
                    lenao_cut[256*(383-x)+(383-y)] = lenao_90Degree_2[256*x+y];
                }
                else if(m1>=0 && m2>=0)// I
                {
                    lenao_cut[256*(127-x)+(383-y)] = lenao_90Degree_2[256*x+y];//左右鏡像
                }
            }
        cvSetData(lenamat_cut, lenao_cut, lenamat_cut->step);
       
        /// Display 旋轉90度 + 上下顛倒 + 切割圖片
        cvNamedWindow("lena_cut", 0);
        cvResizeWindow("lena_cut", 256, 256);
        cvMoveWindow("lena_cut", 260, 0);
        cvShowImage("lena_cut", lenamat_cut);
        printf("Display lena_cut\n");
        
        cvWaitKey(0);
        
        cvDestroyWindow("lena_cut");
       
        ///save the image
        FILE * name_Receive;
        // save lenamat_cut
        char FileNameRes_cut[] = "../../HW1/lena256_cut.raw";
        name_Receive = fopen(FileNameRes_cut, "wb");
        fwrite(lenao_cut, size, 1, name_Receive);
        printf("lenamat_cut         save as lena256_cut.raw\n");

        fclose(lena);
        
        delete lenai;
        delete lenao_90Degree;
        delete lenao_90Degree_2;
        delete lenao_cut;
    }
    
    return 0;
}
