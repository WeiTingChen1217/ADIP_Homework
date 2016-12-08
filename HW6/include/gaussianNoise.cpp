//
//  gaussianNoise.cpp
//  ADIP_Homework
//
//  Created by WeiTingChen on 11/20/16.
//  Copyright © 2016 WeiTingChen. All rights reserved.
//

#include "gaussianNoise.hpp"

void Gaussian_Noise(Mat gKernel)
{
    // set standard deviation to 1.0
    double sigma = 1.0;
    double r, s = 2.0 * sigma * sigma;
    
    // sum is for normalization
    double sum = 0.0;
    
    copyMakeBorder(gKernel, gKernel,
                   2, 2, 2, 2,
                   BORDER_CONSTANT, // 外推的值為常數，常在仿射變換、透視變換中使用
                   Scalar::all(0)); // 為了效率，所以對影像邊界拓展
   
    for(int i = 2; i < (gKernel.rows - 2); i++)
        for(int j = 2; j < (gKernel.cols - 2); j++){
            // generate 5x5 kernel
            for (int x = -2; x <= 2; x++)
                for(int y = -2; y <= 2; y++)
                {
                    r = sqrt(x*x + y*y);
                    gKernel.at<uchar>(i+x, j+y) = (exp(-(r*r)/s))/(M_PI * s);
                    sum += gKernel.at<uchar>(i+x, j+y);
                }
        }
    
    
    // normalize the Kernel
    for(int i = 2; i < (gKernel.rows - 2); i++)
        for(int j = 2; j < (gKernel.cols - 2); j++)
            gKernel.at<uchar>(i, j) /= sum;
    
    //縮短圖片
    Mat gKernel_2 = Mat(Size(gKernel.rows-4, gKernel.cols-4), gKernel.type(), Scalar::all(0));
    for(int i = 0; i < gKernel_2.rows; i++)
        for(int j = 0; j < gKernel_2.cols; j++)
            gKernel_2.at<uchar>(i, j) = gKernel.at<uchar>(i+2, j+2);
    
    imshow("Imput_1", gKernel);
    moveWindow("Imput_1", 0, 0);
    
    waitKey(0);
}