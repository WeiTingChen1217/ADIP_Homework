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

void imhist_1(double image[8][8], int histogram[], const int nrows, const int ncols)
{
    
    // initialize all intensity values to 0
    for(int i = 0; i < 16; i++)
    {
        histogram[i] = 0;
    }
    
    // calculate the no of pixels for each intensity values
    for(int y = 0; y < nrows; y++)
        for(int x = 0; x < ncols; x++)
            histogram[(int)image[y][x]]++;
    
}

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

void cumhist(int histogram[], int cumhistogram[])
{
    cumhistogram[0] = histogram[0];
    
    for(int i = 1; i < 256; i++)
    {
        cumhistogram[i] = histogram[i] + cumhistogram[i-1];
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
    
#ifdef Section1
    //allocate and initialize your user-allocated memory
    const int nrows = 8;
    const int ncols = 8;
# ifdef Figure1
    double data[nrows][ncols]={
        {0, 7, 4, 11, 1, 10, 3, 4},
        {9, 0, 2 ,12 ,3, 6, 9, 2},
        {6, 7, 13, 1, 8, 13, 0, 0},
        {10, 1, 3, 14, 14, 4, 11, 2},
        {4, 12, 3, 5, 10, 7, 6, 6},
        {12, 0, 2, 9, 10, 13, 1, 5},
        {9, 5, 11, 1, 7, 8, 9, 10},
        {7, 2, 9, 10, 4, 5, 15, 0}
    };
# else
    double data[nrows][ncols]={
        { 0,  7,  5,  1,  7,  1, 7 , 7 },
        { 9,  1,  1,  9,  1,  4, 1 , 1 },
        { 5,  9,  1,  0,  4,  5, 1 , 5 },
        {12,  0,  8,  5, 11,  1, 1 , 0 },
        {12,  5,  8, 14,  1,  1, 11, 10},
        { 1, 15,  1,  1,  1, 14, 1 , 10},
        {15,  0, 10,  1,  3,  1, 13, 11},
        {10,  1,  2,  1,  1,  1, 13, 11}
    };
#endif
    //make the Mat from the data (with default stride)
    cv::Mat image(nrows, ncols, CV_8UC1, data);
    
    
    // Generate the histogram
    int histogram[16];
    imhist_1(data, histogram, nrows, ncols);
    
    cout << histogram << endl;
    
#else
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
        
        // Caluculate the size of image
        size = image.rows * image.cols;
        
        // Calculate the probability of each intensity
        float PrRk[256];
        for(int i = 0; i < 256; i++)
            PrRk[i] = (double)histogram[i] / size;
            // PrRk: PDF(機率密度函數)
            // pr(rk)=nk/(image.rows * image.cols)
        
        // Generate cumulative frequency histogram
        int cumhistogram[256];  // CDF
        cumhist(histogram, cumhistogram);
        
        // Scale the histogram
        int Sk[256];
        for(int i = 0; i < 256; i++)
            Sk[i] = cvRound((double)cumhistogram[i] * 255.0 / size);
            // Sk: CDF四捨五入後的結果
        
        
        // Generate the equlized histogram
        float PsSk[256];
        for(int i = 0; i < 256; i++)
            PsSk[i] = 0;
        
        for(int i = 0; i < 256; i++)
            PsSk[Sk[i]] = PsSk[Sk[i]] + PrRk[i];
            // PsSk: 將原本的灰階值對應到新的灰階值。Equlized
        
        int final[256];
        for(int i = 0; i < 256; i++)
            final[i] = cvRound(PsSk[i] * 10000);
            // PsSk[i] 取小數點以下第四位
        
        
        // Generate the equlized image
        Mat new_image = image.clone();
        
        for(int y = 0; y < image.rows; y++)
            for(int x = 0; x < image.cols; x++)
                new_image.at<uchar>(y,x) = saturate_cast<uchar>(Sk[image.at<uchar>(y,x)]);
        
        // Display the original Image
        namedWindow("Original Image");
        imshow("Original Image", image);
        moveWindow("Original Image", 0, 0);
        // Display equilized image
        namedWindow("Equilized Image");
        imshow("Equilized Image",new_image);
        moveWindow("Equilized Image", 512, 0);
        Mat dst;
        /// Apply Histogram Equalization
        equalizeHist( image, dst );
        namedWindow("Equilized Image_2");
        imshow("Equilized Image_2", dst);
        moveWindow("Equilized Image_2", 1000, 0);
        
        
        // Display the original Histogram
        histDisplay(histogram, "Original Histogram");
        moveWindow("Original Histogram", 0, 350);
        // Display the equilzed histogram
        histDisplay(final, "Equilized Histogram");
        moveWindow("Equilized Histogram", 512, 350);
        // Display the original Histogram_2
        imhist_2(dst, histogram);
        histDisplay(histogram, "Equilized Histogram_2");
        moveWindow("Equilized Histogram_2", 1000, 350);
        
        
        waitKey(0);
    }
#endif
    
    
}
