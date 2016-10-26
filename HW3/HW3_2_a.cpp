//
//  main.cpp
//  HW3
//
//  Created by WeiTingChen on 10/21/16.
//  Copyright © 2016 WeiTingChen. All rights reserved.
//
//  Referance:http://www.programming-techniques.com/2013/01/histogram-equalization-using-c-image.html

#include <iostream>
#include <opencv2/opencv.hpp>

#define Figure1
//#define Section1

using std::cout;
using std::cin;
using std::endl;

using namespace cv;


void imhist_2(Mat image, int histogram[])
{
    // initialize all intensity values to 0
    for(int i = 0; i < 256; i++)
        histogram[i] = 0;
        
    // calculate the no of pixels for each intensity values
    for(int y = 0; y < image.rows; y++)
        for(int x = 0; x < image.cols; x++)
            histogram[(int)image.at<uchar>(y,x)]++;
    
}

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

void histDisplay(int histogram[], const char* name)
{
    int hist[256];
    for(int i = 0; i < 256; i++)
    {
        hist[i]=histogram[i];
    }
    // draw the histograms
    int hist_w = 512; int hist_h = 400;
    int bin_w = cvRound((double) hist_w/256);
    
    Mat histImage(hist_h, hist_w, CV_8UC1, Scalar(255, 255, 255));
    
    // find the maximum intensity element from histogram
    int max = hist[0];
    for(int i = 1; i < 256; i++){
        if(max < hist[i]){
            max = hist[i];
        }
    }
    
    // normalize the histogram between 0 and histImage.rows
    
    for(int i = 0; i < 256; i++){
        hist[i] = ((double)hist[i]/max)*histImage.rows;
    }
    
    
    // draw the intensity line for histogram
    for(int i = 0; i < 256; i++)
    {
        line(histImage,
             Point(bin_w*(i), hist_h),  // First point of the line segment.

             Point(bin_w*(i), hist_h - hist[i]), // Second point of the line segment.
             Scalar(0,0,0), 1, 8, 0);
    }
    
    // display histogram
    namedWindow(name, CV_WINDOW_AUTOSIZE);
    imshow(name, histImage);
}

int main()
{
    // insert code here...
    std::cout << "This is homework3.\n\n";
    

    for (int i=0; i<4; i++) {
            
        char str1[30];
        strcpy(str1, "../../doc/HW3/");
        
        if      (i == 0)
            strcat(str1, "lena_512.raw");
        else if (i == 1)
            strcat(str1, "lena_bright_512.raw");
        else if (i == 2)
            strcat(str1, "lena_dark_512.raw");
        else if (i == 3)
            strcat(str1, "earth_512.raw");
        puts(str1);
        
        float size = 512;
        int sizeSrc_width = size;
        int sizeSrc_height = size;
        OpenRAW(str1, sizeSrc_width, sizeSrc_height);
        // return lenamat
        
        // Load the image
        Mat image = cv::cvarrToMat(lenamat, true);
        
        // Generate the histogram
        int histogram[256];
        imhist_2(image, histogram);
        
        
        // Display the original Image
        namedWindow("Original Image");
        imshow("Original Image", image);
        moveWindow("Original Image", 0, 0);
        
        
        // Display the original Histogram
        histDisplay(histogram, "Original Histogram");
        moveWindow("Original Histogram", 0, 350);
        
        
        waitKey(0);
    }
}
