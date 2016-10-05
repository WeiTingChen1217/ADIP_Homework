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
    int size_512 = 512*512;
    char FileNameOri[] = "../../doc/HW1/lena256.raw";
    char FileNameOri_512[] = "../../doc/HW1/lena512.raw";

    /// 建立一個單通道256*256大小的
    CvMat *lenamat              = cvCreateMat(256, 256, CV_8UC1);
    CvMat *lenamat_90Degree     = cvCreateMat(256, 256, CV_8UC1);
    CvMat *lenamat_cut          = cvCreateMat(256, 256, CV_8UC1);
    CvMat *lenamat_brightness   = cvCreateMat(256, 256, CV_8UC1);
    CvMat *lenamat_studentID    = cvCreateMat(512, 512, CV_8UC1);
    
    /// 建立處理圖片的空間
    unsigned char *lenai;
    unsigned char *lenao_90Degree, *lenao_90Degree_2;
    unsigned char *lenao_cut;
    unsigned char *lenao_brightness;
    unsigned char *lenai_studentID;
    
    lenai = new unsigned char[size];
    lenao_90Degree = new unsigned char[size];
    lenao_90Degree_2 = new unsigned char[size];
    lenao_cut = new unsigned char[size];
    lenao_brightness = new unsigned char[size];
    lenai_studentID = new unsigned char[size_512];
    
    FILE *lena, *lena_512;
    lena = fopen(FileNameOri, "rb");
    lena_512 = fopen(FileNameOri_512, "rb");
    
    if((lena == NULL)||(lena_512 == NULL))
    {
        // 檢查檔案是否可以開啟
        puts("Loading File Error!");
    }
    else
    {
        fread(lenai, 1, size, lena);    // lenai 為要處理的影像; 第一個參數為陣列或結構的指標
        
        /// 顯示(200, 156)的亮度
        printf("Intensity values of (200, 156) : %d\n", lenai[256*199+155]);
        
        /// 顯示原始影像
        cvSetData(lenamat, lenai, lenamat->step);

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
                m1 = 127-x; // y = m1+x 斜角：/
                m2 = y-127; // y = m2-x+256 斜角：\
                
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
        
        /// 貼字在圖片上
        fread(lenai_studentID, 1, size_512, lena_512);
        cvSetData(lenamat_studentID, lenai_studentID, lenamat_studentID->step);
        
        CvPoint CircleCenter=cvPoint(256, 256);
        CvSize EllipseAxes=cvSize(250,100);     //橢圓軸
        double RotateAngle=0;
        double StartDrawingAngle=0;
        double StopDrawingAngle=360;
        CvScalar Color=CV_RGB(255,255,255);
        int Thickness=5;
        int Shift=0;
        
        cvEllipse(lenamat_studentID,CircleCenter,EllipseAxes,RotateAngle,StartDrawingAngle,StopDrawingAngle,Color,Thickness,CV_AA,Shift);
        
        CvFont Font1=cvFont(1,1);
        cvInitFont(&Font1,CV_FONT_HERSHEY_SIMPLEX,2,1,0.1,2,CV_AA);
        
        CvPoint TextPosition1=cvPoint(25, 270);
        Color=CV_RGB(0,0,0);
        
        cvPutText(lenamat_studentID,"1 0 4 3 6 8 0 2 2",TextPosition1,&Font1,Color);
        
        //        cvEqualizeHist(DFT,DFT);
        //        cvSaveImage(FileNameRes,lenamat);
        //        fwrite(depthDFT,1,size,camerasave);
        
        /// Display lenamat
        cvNamedWindow("lena_original", 0);
        cvResizeWindow("lena_original", 256, 256);
        cvMoveWindow("lena_original", 0, 0);
        cvShowImage("lena_original", lenamat);
        printf("Display lena_original\n");
        
        /// Display 旋轉90度 + 上下顛倒
        cvNamedWindow("lena_90Degree", 0);
        cvResizeWindow("lena_90Degree", 256, 256);
        cvMoveWindow("lena_90Degree", 0, 310);
        cvShowImage("lena_90Degree", lenamat_90Degree);
        printf("Display lena_90Degree\n");
        
        /// Display 旋轉90度 + 上下顛倒 + 切割圖片
        cvNamedWindow("lena_cut", 0);
        cvResizeWindow("lena_cut", 256, 256);
        cvMoveWindow("lena_cut", 260, 0);
        cvShowImage("lena_cut", lenamat_cut);
        printf("Display lena_cut\n");
        
        /// Display 條亮度
        cvNamedWindow("lena_brightness", 0);
        cvResizeWindow("lena_brightness", 256, 256);
        cvMoveWindow("lena_brightness", 260, 310);
        cvShowImage("lena_brightness", lenamat_brightness);
        printf("Display lena_brightness\n");
        
        /// Display lenamat_studentID
        cvNamedWindow("lena_studentID", 0);
        cvResizeWindow("lena_studentID", 512, 512);
        cvMoveWindow("lena_studentID", 530, 0);
        cvShowImage("lena_studentID", lenamat_studentID);
        printf("Display lena_studentID\n");
        
        cvWaitKey(0);
        
        cvDestroyWindow("lena_original");
        cvDestroyWindow("lena_90Degree");
        cvDestroyWindow("lena_cut");
        cvDestroyWindow("lena_brightness");
        cvDestroyWindow("lena_studentID");
        
        ///save the image
        FILE * name_Receive;
        // save lenamat_90Degree
        char FileNameRes_90Degree[] = "../../HW1/lena256_90Degree.raw";
        name_Receive = fopen(FileNameRes_90Degree, "wb");
        fwrite(lenao_90Degree_2, size, 1, name_Receive);
        printf("lenamat_90Degree    save as lena256_90Degree.raw\n");
        // save lenamat_cut
        char FileNameRes_cut[] = "../../HW1/lena256_cut.raw";
        name_Receive = fopen(FileNameRes_cut, "wb");
        fwrite(lenao_cut, size, 1, name_Receive);
        printf("lenamat_cut         save as lena256_cut.raw\n");
        // save lenamat_brightness
        char FileNameRes_brightness[] = "../../HW1/lena256_brightness.raw";
        name_Receive = fopen(FileNameRes_brightness, "wb");
        fwrite(lenao_brightness, size, 1, name_Receive);
        printf("lenamat_brightness  save as lena256_brightness.raw\n");
        // save lenamat_studentID
        char FileNameRes_studentID[] = "../../HW1/lena256_studentID.png";
//        name_Receive = fopen(FileNameRes_studentID, "wb");
//        fwrite(lenamat_studentID, sizeof(lenamat_studentID), 1, name_Receive);
        cvSaveImage(FileNameRes_studentID, lenamat_studentID);
        printf("lenamat_studentID   save as lena256_studentID.png\n");

        fclose(lena);
        fclose(lena_512);
        
        delete lenai;
        delete lenao_90Degree;
        delete lenao_90Degree_2;
        delete lenao_cut;
        delete lenao_brightness;
        delete lenai_studentID;
        
    }
    
    return 0;
}
