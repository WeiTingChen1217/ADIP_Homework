//
//  zoomingAndShrinking.cpp
//  ADIP_Homework
//
//  Created by WeiTingChen on 10/17/16.
//  Copyright © 2016 WeiTingChen. All rights reserved.
//

#include "zoomingAndShrinking.hpp"

void nearest_neighbor(cv::Mat& matSrc, cv::Mat& matDst1, cv::Mat& matDst2, double scale_x, double scale_y){
    // Reference:http://blog.csdn.net/lichengyu/article/details/8526629
    
    std::cout << "Doing nearest neighbor interpolation.\n";
    
    for (int i = 0; i < matDst1.cols; ++i)
    {
        int sx = cvFloor(i * scale_x);  // 無條件捨去
        sx = std::min(sx, matSrc.cols - 1);
        for (int j = 0; j < matDst1.rows; ++j)
        {
            int sy = cvFloor(j * scale_y);
            sy = std::min(sy, matSrc.rows - 1);
# ifdef color
            matDst1.at<cv::Vec3b>(j, i) = matSrc.at<cv::Vec3b>(sy, sx);//for Color Image
# else
            matDst1.at<uchar>(j, i) = matSrc.at<uchar>(sy, sx);//for Gray Image
# endif
        }
    }
# ifdef OpenCV_function
    cv::resize(matSrc, matDst2, matDst1.size(), 0, 0, cv::INTER_NEAREST);
# endif
    
    std::cout << "Nearest neighbor interpolation done!!.\n\n";
}

void bilinear(cv::Mat& matSrc, cv::Mat& matDst1, cv::Mat& matDst2, double scale_x, double scale_y){
    // Reference:http://blog.csdn.net/lichengyu/article/details/8526629
    
    std::cout << "Doing bilinear interpolation.\n";
    
    uchar* dataDst = matDst1.data;
    int stepDst = (int)matDst1.step;// 使用指標讀取圖檔的時候會用到 .step
    // (int)matDst1.step也就是取matDst1.step[0]的意思; 代表一列所有的數據大小; 以256*256灰階圖為例，matDst1.step[0]=matDst1.cols*matDst1.Size()=256*1=256
    uchar* dataSrc = matSrc.data;
    int stepSrc = (int)matSrc.step;
    int iWidthSrc = matSrc.cols;
    int iHiehgtSrc = matSrc.rows;
    
    double threshold = pow(2, 11); // 調整後 2^11 比較好
    
    for (int j = 0; j < matDst1.rows; ++j)
    {
        // sx, sy 記錄左上方的位置
        // fx, fy 為 alpha, beta
        float fy = (float)((j + 0.5) * scale_y - 0.5); // 使得圖像邊緣也被處理到
        int sy = cvFloor(fy);
        fy -= sy; // 取 fy 小數點
        sy = std::min(sy, iHiehgtSrc - 2); // 最多取到倒數第二個值
        sy = std::max(0, sy); // 不低於零
        
        short cbufy[2];
        cbufy[0] = cv::saturate_cast<short>((1.f - fy) * threshold); // (1-beta)
        // saturate_cast 使數值不溢出某(short)資料型態
        cbufy[1] = threshold - cbufy[0]; // beta
        
        for (int i = 0; i < matDst1.cols; ++i)
        {
            float fx = (float)((i + 0.5) * scale_x - 0.5);
            int sx = cvFloor(fx);
            fx -= sx;
            
            sx = std::min(sx, iWidthSrc - 2);
            sx = std::max(0, sx);
            
            short cbufx[2];
            cbufx[0] = cv::saturate_cast<short>((1.f - fx) * threshold);
            cbufx[1] = threshold - cbufx[0];
            
            // 開始計算
# ifdef color
            // 因為讀圖片的方式為直接向記憶體取值，故三通道與單通到的差異在於取值時要跳三格取一個值，因此*3
            for (int k = 0; k < matSrc.channels(); ++k)
            {
                *(dataDst+ j*stepDst + 3*i + k) = (
                   *(dataSrc +  sy   *stepSrc + 3* sx    + k) * cbufx[0] * cbufy[0] +
                   *(dataSrc + (sy+1)*stepSrc + 3* sx    + k) * cbufx[0] * cbufy[1] +
                   *(dataSrc +  sy   *stepSrc + 3*(sx+1) + k) * cbufx[1] * cbufy[0] +
                   *(dataSrc + (sy+1)*stepSrc + 3*(sx+1) + k) * cbufx[1] * cbufy[1]
                   ) >> 2*(int)log2(threshold);
            }
# else
            *(dataDst+ j*stepDst + i) = (
                 *(dataSrc +  sy   *stepSrc +  sx   ) * cbufx[0] * cbufy[0] +
                 *(dataSrc + (sy+1)*stepSrc +  sx   ) * cbufx[0] * cbufy[1] +
                 *(dataSrc +  sy   *stepSrc + (sx+1)) * cbufx[1] * cbufy[0] +
                 *(dataSrc + (sy+1)*stepSrc + (sx+1)) * cbufx[1] * cbufy[1]
                 ) >> 2*(int)log2(threshold); // 因為 alpha, beta 多乘了兩次 threshold 所以要除回來
# endif
        }
    }
# ifdef OpenCV_function
    cv::resize(matSrc, matDst2, matDst1.size(), 0, 0, cv::INTER_LINEAR);
# endif
    
    std::cout << "Bilinear interpolation done!!.\n\n";
}

void bicubic(cv::Mat& matSrc, cv::Mat& matDst1, cv::Mat& matDst2, double scale_x, double scale_y){
    // Reference:http://blog.csdn.net/lichengyu/article/details/8526629
    
    std::cout << "Doing bicubic interpolation.\n";
    
    //    int iscale_x = cv::saturate_cast<int>(scale_x);
    //    int iscale_y = cv::saturate_cast<int>(scale_y);
    
    for (int j = 0; j < matDst1.rows; ++j)
    {
        float fy = (float)((j + 0.5) * scale_y - 0.5);
        int sy = cvFloor(fy);
        fy -= sy;
        sy = std::min(sy, matSrc.rows - 3);
        sy = std::max(1, sy);
        
        const float A = -0.75f;
        
        float coeffsY[4];
        coeffsY[0] = ((A*(fy + 1) - 5*A)*(fy + 1) + 8*A)*(fy + 1) - 4*A;
        coeffsY[1] = ((A + 2)*     fy  - (A + 3))*     fy *     fy  + 1;
        coeffsY[2] = ((A + 2)*(1 - fy) - (A + 3))*(1 - fy)*(1 - fy) + 1;
        coeffsY[3] = 1.f - coeffsY[0] - coeffsY[1] - coeffsY[2];
        
        short cbufY[4];
        cbufY[0] = cv::saturate_cast<short>(coeffsY[0] * 2048);
        cbufY[1] = cv::saturate_cast<short>(coeffsY[1] * 2048);
        cbufY[2] = cv::saturate_cast<short>(coeffsY[2] * 2048);
        cbufY[3] = cv::saturate_cast<short>(coeffsY[3] * 2048);
        
        for (int i = 0; i < matDst1.cols; ++i)
        {
            float fx = (float)((i + 0.5) * scale_x - 0.5);
            int sx = cvFloor(fx);
            fx -= sx;
            
            sx = std::min(sx, matSrc.rows - 3);
            sx = std::max(1, sx);
            
            float coeffsX[4];
            coeffsX[0] = ((A*(fx + 1) - 5*A)*(fx + 1) + 8*A)*(fx + 1) - 4*A;
            coeffsX[1] = ((A + 2)*     fx  - (A + 3))*     fx *     fx  + 1;
            coeffsX[2] = ((A + 2)*(1 - fx) - (A + 3))*(1 - fx)*(1 - fx) + 1;
            coeffsX[3] = 1.f - coeffsX[0] - coeffsX[1] - coeffsX[2];
            
            short cbufX[4];
            cbufX[0] = cv::saturate_cast<short>(coeffsX[0] * 2048);
            cbufX[1] = cv::saturate_cast<short>(coeffsX[1] * 2048);
            cbufX[2] = cv::saturate_cast<short>(coeffsX[2] * 2048);
            cbufX[3] = cv::saturate_cast<short>(coeffsX[3] * 2048);
            
# ifdef color
            for (int k = 0; k < matSrc.channels(); ++k)
            {
                matDst1.at<cv::Vec3b>(j, i)[k] = abs(
                    (
                    matSrc.at<cv::Vec3b>(sy-1, sx-1)[k] * cbufX[0] * cbufY[0] +
                    matSrc.at<cv::Vec3b>(sy  , sx-1)[k] * cbufX[0] * cbufY[1] +
                    matSrc.at<cv::Vec3b>(sy+1, sx-1)[k] * cbufX[0] * cbufY[2] +
                    matSrc.at<cv::Vec3b>(sy+2, sx-1)[k] * cbufX[0] * cbufY[3] +
                    matSrc.at<cv::Vec3b>(sy-1, sx  )[k] * cbufX[1] * cbufY[0] +
                    matSrc.at<cv::Vec3b>(sy  , sx  )[k] * cbufX[1] * cbufY[1] +
                    matSrc.at<cv::Vec3b>(sy+1, sx  )[k] * cbufX[1] * cbufY[2] +
                    matSrc.at<cv::Vec3b>(sy+2, sx  )[k] * cbufX[1] * cbufY[3] +
                    matSrc.at<cv::Vec3b>(sy-1, sx+1)[k] * cbufX[2] * cbufY[0] +
                    matSrc.at<cv::Vec3b>(sy  , sx+1)[k] * cbufX[2] * cbufY[1] +
                    matSrc.at<cv::Vec3b>(sy+1, sx+1)[k] * cbufX[2] * cbufY[2] +
                    matSrc.at<cv::Vec3b>(sy+2, sx+1)[k] * cbufX[2] * cbufY[3] +
                    matSrc.at<cv::Vec3b>(sy-1, sx+2)[k] * cbufX[3] * cbufY[0] +
                    matSrc.at<cv::Vec3b>(sy  , sx+2)[k] * cbufX[3] * cbufY[1] +
                    matSrc.at<cv::Vec3b>(sy+1, sx+2)[k] * cbufX[3] * cbufY[2] +
                    matSrc.at<cv::Vec3b>(sy+2, sx+2)[k] * cbufX[3] * cbufY[3]
                    ) >> 22);
            }
# else
            matDst1.at<uchar>(j, i) = abs(
                (
                matSrc.at<uchar>(sy-1, sx-1) * cbufX[0] * cbufY[0] +
                matSrc.at<uchar>(sy  , sx-1) * cbufX[0] * cbufY[1] +
                matSrc.at<uchar>(sy+1, sx-1) * cbufX[0] * cbufY[2] +
                matSrc.at<uchar>(sy+2, sx-1) * cbufX[0] * cbufY[3] +
                matSrc.at<uchar>(sy-1, sx  ) * cbufX[1] * cbufY[0] +
                matSrc.at<uchar>(sy  , sx  ) * cbufX[1] * cbufY[1] +
                matSrc.at<uchar>(sy+1, sx  ) * cbufX[1] * cbufY[2] +
                matSrc.at<uchar>(sy+2, sx  ) * cbufX[1] * cbufY[3] +
                matSrc.at<uchar>(sy-1, sx+1) * cbufX[2] * cbufY[0] +
                matSrc.at<uchar>(sy  , sx+1) * cbufX[2] * cbufY[1] +
                matSrc.at<uchar>(sy+1, sx+1) * cbufX[2] * cbufY[2] +
                matSrc.at<uchar>(sy+2, sx+1) * cbufX[2] * cbufY[3] +
                matSrc.at<uchar>(sy-1, sx+2) * cbufX[3] * cbufY[0] +
                matSrc.at<uchar>(sy  , sx+2) * cbufX[3] * cbufY[1] +
                matSrc.at<uchar>(sy+1, sx+2) * cbufX[3] * cbufY[2] +
                matSrc.at<uchar>(sy+2, sx+2) * cbufX[3] * cbufY[3]
                ) >> 22);
# endif
        }
    }
# ifdef OpenCV_function
    cv::resize(matSrc, matDst2, matDst1.size(), 0, 0, cv::INTER_CUBIC);
# endif
    
    std::cout << "Bicubic interpolation done!!.\n\n";
}
