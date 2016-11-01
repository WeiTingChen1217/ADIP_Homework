//
//  filter.hpp
//  ADIP_Homework
//
//  Created by WeiTingChen on 10/31/16.
//  Copyright © 2016 WeiTingChen. All rights reserved.
//

#ifndef filter_hpp
#define filter_hpp

#include <stdio.h>
#include <opencv2/opencv.hpp>

using namespace cv;
using namespace std;

void low_pass_filter(Mat src1, Mat src2, int a);
void high_pass_filter(Mat src1, Mat src2, int a);
void median_filter(Mat src1, Mat src2, int a);

#endif /* filter_hpp */
