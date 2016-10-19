//
//  zoomingAndShrinking.hpp
//  ADIP_Homework
//
//  Created by WeiTingChen on 10/17/16.
//  Copyright © 2016 WeiTingChen. All rights reserved.
//


#include <stdio.h>
#include <opencv2/opencv.hpp>

//#define color
//#define OpenCV_function

void nearest_neighbor(cv::Mat& matSrc, cv::Mat& matDst1, cv::Mat& matDst2, double scale_x, double scale_y);

void bilinear(cv::Mat& matSrc, cv::Mat& matDst1, cv::Mat& matDst2, double scale_x, double scale_y);

void bicubic(cv::Mat& matSrc, cv::Mat& matDst1, cv::Mat& matDst2, double scale_x, double scale_y);
