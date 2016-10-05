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
    int size_512 = 512*512;
    char FileNameOri_512[] = "../../doc/HW1/lena512.raw";

    /// 建立一個單通道256*256大小的
    CvMat *lenamat_studentID    = cvCreateMat(512, 512, CV_8UC1);
    
    /// 建立處理圖片的空間
    unsigned char *lenai_studentID;
    
    lenai_studentID = new unsigned char[size_512];
    
    FILE *lena_512;
    lena_512 = fopen(FileNameOri_512, "rb");
    
    if(lena_512 == NULL)
    {
        // 檢查檔案是否可以開啟
        puts("Loading File Error!");
    }
    else
    {
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
        
        CvPoint TextPosition1=cvPoint(25, 270); //文字起始位置
        Color=CV_RGB(0,0,0);
        
        cvPutText(lenamat_studentID,"1 0 4 3 6 8 0 2 2",TextPosition1,&Font1,Color);
       
       /// Display lenamat_studentID
        cvNamedWindow("lena_studentID", 0);
        cvResizeWindow("lena_studentID", 512, 512);
        cvMoveWindow("lena_studentID", 530, 0);
        cvShowImage("lena_studentID", lenamat_studentID);
        printf("Display lena_studentID\n");
        
        cvWaitKey(0);
        
        cvDestroyWindow("lena_studentID");
        
        ///save the image
        FILE * name_Receive;
        // save lenamat_studentID
        char FileNameRes_studentID[] = "../../HW1/lena256_studentID.png";
//        name_Receive = fopen(FileNameRes_studentID, "wb");
//        fwrite(lenamat_studentID, sizeof(lenamat_studentID), 1, name_Receive);
        cvSaveImage(FileNameRes_studentID, lenamat_studentID);
        printf("lenamat_studentID   save as lena256_studentID.png\n");

        fclose(lena_512);
        
        delete lenai_studentID;
        
    }
    
    return 0;
}
