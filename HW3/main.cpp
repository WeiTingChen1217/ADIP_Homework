//
//  main.cpp
//  HW3
//
//  Created by WeiTingChen on 10/21/16.
//  Copyright © 2016 WeiTingChen. All rights reserved.
//

#include <iostream>
#include <opencv2/opencv.hpp>

#define Figure1
#define Section1

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
    {
        histogram[i] = 0;
    }
    
    // calculate the no of pixels for each intensity values
    for(int y = 0; y < image.rows; y++)
        for(int x = 0; x < image.cols; x++)
            histogram[(int)image.at<uchar>(y,x)]++;
    
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
        line(histImage, Point(bin_w*(i), hist_h),
             Point(bin_w*(i), hist_h - hist[i]),
             Scalar(0,0,0), 1, 8, 0);
    }
    
    // display histogram
    namedWindow(name, CV_WINDOW_AUTOSIZE);
    imshow(name, histImage);
    moveWindow(name, 0, 350);
    
}

int main()
{
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
    cv::Mat cv_data(nrows, ncols, CV_8UC1, data);
    
    
    // Generate the histogram
    int histogram[16];
#ifdef Section1
    imhist_1(data, histogram, nrows, ncols);
#else
    imhist_2(cv_data, histogram);
#endif
    //print the Mat to see for yourself
    std::cout << histogram << std::endl;
    
    cv::namedWindow("Output", cv::WINDOW_AUTOSIZE);
    cv::imshow("Output", cv_data);
    cvWaitKey(0);
    cv::imwrite("../../HW3/output/2.jpg", cv_data);
    
}
