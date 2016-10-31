//
//  filter.cpp
//  ADIP_Homework
//
//  Created by WeiTingChen on 10/31/16.
//  Copyright © 2016 WeiTingChen. All rights reserved.
//

#include "filter.hpp"

void low_pass_filter(Mat src1, Mat src2, int a){
    
    //for loop for counting the number of rows and columns and displaying the pixel value at each point
    for (int i = 0; i < src1.rows-a; i++)
        for (int j = 0; j < src1.cols-a; j++)
        {
            Scalar intensity1 = 0;
            Scalar intensity2;
            if ((i == 0) || (j == 0)) {
                for (int p = 0; p <= (a-1)/2; p++)
                    for (int q = 0; q <= (a-1)/2; q++)
                    {
                        intensity1 = src1.at<uchar>(i+p, j+q);
                        intensity2.val[0] += intensity1.val[0]; // 單一通道
                    }
            }
            
            for (int p = 0; p < a; p++)
                for (int q = 0; q < a; q++)
                {
                    intensity1 = src1.at<uchar>(i+p, j+q);
                    intensity2.val[0] += intensity1.val[0]; // 單一通道
                }
            src2.at<uchar>(i+(a-1)/2, j+(a-1)/2) = intensity2.val[0] / (a*a);
        }
}
