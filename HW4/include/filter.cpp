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
    for (int i = 0; i < src1.rows; i++)
        for (int j = 0; j < src1.cols; j++)
        {
            Scalar intensity1 = 0;
            Scalar intensity2;
// 對左上角的影像做處理
            if ((i == 0) && (j == 0)) {
                char mask[] = { 3, 2,
                                2, 1};
                char count = 0;
                for (int p = 0; p < 2; p++)
                    for (int q = 0; q < 2; q++)
                    {
                        intensity1 = src1.at<uchar>(i+p, j+q);
                        intensity2.val[0] += intensity1.val[0] * mask[count];
                        count++;
                    }
                src2.at<uchar>(i, j) = intensity2.val[0] / (a*a);
// 對左下角做處理
            }else if ((i == src1.rows-1) && (j == 0)){
                char mask[] = { 2, 1,
                                3, 2};
                char count = 0;
                for (int p = -1; p < 1; p++)
                    for (int q = 0; q < 2; q++)
                    {
                        intensity1 = src1.at<uchar>(i+p, j+q);
                        intensity2.val[0] += intensity1.val[0] * mask[count];
                        count++;
                    }
                src2.at<uchar>(i, j) = intensity2.val[0] / (a*a);

// 對右上角做處理
            }else if ((i == 0) && (j == src1.cols-1)){
                char mask[] = { 2, 3,
                                1, 2};
                char count = 0;
                for (int p = 0; p < 2; p++)
                    for (int q = -1; q < 1; q++)
                    {
                        intensity1 = src1.at<uchar>(i+p, j+q);
                        intensity2.val[0] += intensity1.val[0] * mask[count];
                        count++;
                    }
                src2.at<uchar>(i, j) = intensity2.val[0] / (a*a);
// 對右下角做處理
            }else if ((i == src1.rows-1) && (j == src1.cols-1)){
                char mask[] = { 1, 2,
                                2, 3};
                char count = 0;
                for (int p = -1; p < 1; p++)
                    for (int q = -1; q < 1; q++)
                    {
                        intensity1 = src1.at<uchar>(i+p, j+q);
                        intensity2.val[0] += intensity1.val[0] * mask[count];
                        count++;
                    }
                src2.at<uchar>(i, j) = intensity2.val[0] / (a*a);
// 對最上面做處理
            }else if (i == 0){
                char mask[] = { 2, 2, 2,
                                1, 1, 1};
                char count = 0;
                for (int p = 0; p < 2; p++)
                    for (int q = -1; q < 2; q++)
                    {
                        intensity1 = src1.at<uchar>(i+p, j+q);
                        intensity2.val[0] += intensity1.val[0] * mask[count];
                        count++;
                    }
                src2.at<uchar>(i, j) = intensity2.val[0] / (a*a);
// 對最下面做處理
            }else if (i == src1.rows-1){
                char mask[] = { 1, 1, 1,
                                2, 2, 2};
                char count = 0;
                for (int p = -1; p < 1; p++)
                    for (int q = -1; q < 2; q++)
                    {
                        intensity1 = src1.at<uchar>(i+p, j+q);
                        intensity2.val[0] += intensity1.val[0] * mask[count];
                        count++;
                    }
                src2.at<uchar>(i, j) = intensity2.val[0] / (a*a);
// 對最左邊做處理
            }else if (j == 0){
                char mask[] = { 2, 1,
                                2, 1,
                                2, 1};
                char count = 0;
                for (int p = -1; p < 2; p++)
                    for (int q = 0; q < 2; q++)
                    {
                        intensity1 = src1.at<uchar>(i+p, j+q);
                        intensity2.val[0] += intensity1.val[0] * mask[count];
                        count++;
                    }
                src2.at<uchar>(i, j) = intensity2.val[0] / (a*a);
// 對最右面做處理
            }else if (j == src1.cols-1){
                char mask[] = { 1, 2,
                                1, 2,
                                1, 2};
                char count = 0;
                for (int p = -1; p < 2; p++)
                    for (int q = -1; q < 1; q++)
                    {
                        intensity1 = src1.at<uchar>(i+p, j+q);
                        intensity2.val[0] += intensity1.val[0] * mask[count];
                        count++;
                    }
                src2.at<uchar>(i, j) = intensity2.val[0] / (a*a);
            }else{
                for (int p = -1; p < 2; p++)
                    for (int q = -1; q < 2; q++)
                    {
                        intensity1 = src1.at<uchar>(i+p, j+q);
                        intensity2.val[0] += intensity1.val[0];
                    }
                src2.at<uchar>(i, j) = intensity2.val[0] / (a*a);
            }
        }
}

// Laplacian
// 0  -1  0
// -1  4 -1
// 0  -1  0
void high_pass_filter(Mat src1, Mat src2, int a){
    
    //for loop for counting the number of rows and columns and displaying the pixel value at each point
    for (int i = 0; i < src1.rows; i++)
        for (int j = 0; j < src1.cols; j++)
        {
            Scalar intensity1 = 0;
            Scalar intensity2;
// 對左上角的影像做處理
            if ((i == 0) && (j == 0)) {
                char mask[] = {  2, -1,
                                -1,  0};
                char count = 0;
                for (int p = 0; p < 2; p++)
                    for (int q = 0; q < 2; q++)
                    {
                        intensity1 = src1.at<uchar>(i+p, j+q);
                        intensity2.val[0] += intensity1.val[0] * mask[count];
                        count++;
                    }
// 對左下角做處理
            }else if ((i == src1.rows-1) && (j == 0)){
                char mask[] = { -1,  0,
                                 2, -1};
                char count = 0;
                for (int p = -1; p < 1; p++)
                    for (int q = 0; q < 2; q++)
                    {
                        intensity1 = src1.at<uchar>(i+p, j+q);
                        intensity2.val[0] += intensity1.val[0] * mask[count];
                        count++;
                    }
// 對右上角做處理
            }else if ((i == 0) && (j == src1.cols-1)){
                char mask[] = { -1,  2,
                                 0, -1};
                char count = 0;
                for (int p = 0; p < 2; p++)
                    for (int q = -1; q < 1; q++)
                    {
                        intensity1 = src1.at<uchar>(i+p, j+q);
                        intensity2.val[0] += intensity1.val[0] * mask[count];
                        count++;
                    }
// 對右下角做處理
            }else if ((i == src1.rows-1) && (j == src1.cols-1)){
                char mask[] = {  0, -1,
                                -1,  2};
                char count = 0;
                for (int p = -1; p < 1; p++)
                    for (int q = -1; q < 1; q++)
                    {
                        intensity1 = src1.at<uchar>(i+p, j+q);
                        intensity2.val[0] += intensity1.val[0] * mask[count];
                        count++;
                    }
// 對最上面做處理
            }else if (i == 0){
                char mask[] = { -1,  3, -1,
                                 0, -1,  0};
                char count = 0;
                for (int p = 0; p < 2; p++)
                    for (int q = -1; q < 2; q++)
                    {
                        intensity1 = src1.at<uchar>(i+p, j+q);
                        intensity2.val[0] += intensity1.val[0] * mask[count];
                        count++;
                    }
// 對最下面做處理
            }else if (i == src1.rows-1){
                char mask[] = {  0, -1,  0,
                                -1,  3, -1};
                char count = 0;
                for (int p = -1; p < 1; p++)
                    for (int q = -1; q < 2; q++)
                    {
                        intensity1 = src1.at<uchar>(i+p, j+q);
                        intensity2.val[0] += intensity1.val[0] * mask[count];
                        count++;
                    }
// 對最左邊做處理
            }else if (j == 0){
                char mask[] = { -1,  0,
                                 3, -1,
                                -1,  0};
                char count = 0;
                for (int p = -1; p < 2; p++)
                    for (int q = 0; q < 2; q++)
                    {
                        intensity1 = src1.at<uchar>(i+p, j+q);
                        intensity2.val[0] += intensity1.val[0] * mask[count];
                        count++;
                    }
// 對最右面做處理
            }else if (j == src1.cols-1){
                char mask[] = {  0, -1,
                                -1,  3,
                                 0, -1};
                char count = 0;
                for (int p = -1; p < 2; p++)
                    for (int q = -1; q < 1; q++)
                    {
                        intensity1 = src1.at<uchar>(i+p, j+q);
                        intensity2.val[0] += intensity1.val[0] * mask[count];
                        count++;
                    }
            }else{
                for (int p = -1; p < 2; p++)
                    for (int q = -1; q < 2; q++)
                    {
                        intensity1 = src1.at<uchar>(i+p, j+q);
                        intensity2.val[0] += intensity1.val[0];
                    }
            }
            src2.at<uchar>(i, j) = intensity2.val[0] / (a*a);

        }
}
