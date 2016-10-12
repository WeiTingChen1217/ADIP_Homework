//
//  main.cpp
//  HW2
//
//  Created by WeiTingChen on 10/7/16.
//  Copyright © 2016 WeiTingChen. All rights reserved.
//

#include <iostream>
#include <stdio.h>
#include <opencv2/opencv.hpp>

//#define color

CvMat *lenamat;
int sizDst_width;
int sizDst_height;

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

void showImage(cv::Mat& matSrc, cv::Mat& matDst1, cv::Mat& matDst2){
    /// Display matSrc
    namedWindow("Display_matSrc", cv::WINDOW_AUTOSIZE);
    cvMoveWindow("Display_matSrc", 0, 0);
    imshow("Display_matSrc", matSrc);
    printf("Display matSrc\n");
    /// Display matDst1
    namedWindow("Display_matDst1", cv::WINDOW_AUTOSIZE);
    cvMoveWindow("Display_matDst1", 0, 0);
    imshow("Display_matDst1", matDst1);
    printf("Display matDst1\n");
    /// Display matDst2
    namedWindow("Display_matDst2", cv::WINDOW_AUTOSIZE);
    cvMoveWindow("Display_matDst2", sizDst_width, 0);
    imshow("Display_matDst2", matDst2);
    printf("Display matDst2\n");

    cvWaitKey(0);
    
    cvDestroyWindow("Display_matSrc");
    cvDestroyWindow("Display_matDst1");
    cvDestroyWindow("Display_matDst2");
}

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
    cv::resize(matSrc, matDst2, matDst1.size(), 0, 0, cv::INTER_NEAREST);
    
    showImage(matSrc, matDst1, matDst2);

//    // Save image
//    cv::imwrite("../../HW2/nearest_1.jpg", matDst1);
//    printf("Save matDst1 as nearest_1.jpg\n");
//    cv::imwrite("../../HW2/nearest_2.jpg", matDst2);
//    printf("Save matDst2 as nearest_2.jpg\n");

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

    double threshold = pow(2, 0); // 調整後 2^11 比較好
    
    for (int j = 0; j < matDst1.rows; ++j)
    {
        // sx, sy 記錄右上方的位置
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
            
# ifdef color
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
            // 
            *(dataDst+ j*stepDst + i) = (
                *(dataSrc +  sy   *stepSrc +  sx   ) * cbufx[0] * cbufy[0] +
                *(dataSrc + (sy+1)*stepSrc +  sx   ) * cbufx[0] * cbufy[1] +
                *(dataSrc +  sy   *stepSrc + (sx+1)) * cbufx[1] * cbufy[0] +
                *(dataSrc + (sy+1)*stepSrc + (sx+1)) * cbufx[1] * cbufy[1]
                ) >> 2*(int)log2(threshold); // 因為多乘了兩次 2048(2^11) 所以要除 2^22
# endif
        }
    }
    cv::resize(matSrc, matDst2, matDst1.size(), 0, 0, cv::INTER_LINEAR);

    showImage(matSrc, matDst1, matDst2);
    
//    // Save image
//    cv::imwrite("../../HW2/linear_1.jpg", matDst1);
//    printf("Save matDst1 as linear_1.jpg\n");
//    cv::imwrite("../../HW2/linear_2.jpg", matDst2);
//    printf("Save matDst2 as linear_1.jpg\n");
    
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
        coeffsY[1] = ((A + 2)*fy - (A + 3))*fy*fy + 1;
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
            coeffsX[1] = ((A + 2)*fx - (A + 3))*fx*fx + 1;
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
                    matSrc.at<cv::Vec3b>(sy, sx-1)[k] * cbufX[0] * cbufY[1] +
                    matSrc.at<cv::Vec3b>(sy+1, sx-1)[k] * cbufX[0] * cbufY[2] +
                    matSrc.at<cv::Vec3b>(sy+2, sx-1)[k] * cbufX[0] * cbufY[3] +
                    matSrc.at<cv::Vec3b>(sy-1, sx)[k] * cbufX[1] * cbufY[0] +
                    matSrc.at<cv::Vec3b>(sy, sx)[k] * cbufX[1] * cbufY[1] +
                    matSrc.at<cv::Vec3b>(sy+1, sx)[k] * cbufX[1] * cbufY[2] +
                    matSrc.at<cv::Vec3b>(sy+2, sx)[k] * cbufX[1] * cbufY[3] +
                    matSrc.at<cv::Vec3b>(sy-1, sx+1)[k] * cbufX[2] * cbufY[0] +
                    matSrc.at<cv::Vec3b>(sy, sx+1)[k] * cbufX[2] * cbufY[1] +
                    matSrc.at<cv::Vec3b>(sy+1, sx+1)[k] * cbufX[2] * cbufY[2] +
                    matSrc.at<cv::Vec3b>(sy+2, sx+1)[k] * cbufX[2] * cbufY[3] +
                    matSrc.at<cv::Vec3b>(sy-1, sx+2)[k] * cbufX[3] * cbufY[0] +
                    matSrc.at<cv::Vec3b>(sy, sx+2)[k] * cbufX[3] * cbufY[1] +
                    matSrc.at<cv::Vec3b>(sy+1, sx+2)[k] * cbufX[3] * cbufY[2] +
                    matSrc.at<cv::Vec3b>(sy+2, sx+2)[k] * cbufX[3] * cbufY[3]
                    ) >> 22);
            }
# else
            matDst1.at<uchar>(j, i) = abs(
                (
                matSrc.at<uchar>(sy-1, sx-1) * cbufX[0] * cbufY[0] +
                matSrc.at<uchar>(sy,   sx-1) * cbufX[0] * cbufY[1] +
                matSrc.at<uchar>(sy+1, sx-1) * cbufX[0] * cbufY[2] +
                matSrc.at<uchar>(sy+2, sx-1) * cbufX[0] * cbufY[3] +
                matSrc.at<uchar>(sy-1, sx  ) * cbufX[1] * cbufY[0] +
                matSrc.at<uchar>(sy,   sx  ) * cbufX[1] * cbufY[1] +
                matSrc.at<uchar>(sy+1, sx  ) * cbufX[1] * cbufY[2] +
                matSrc.at<uchar>(sy+2, sx  ) * cbufX[1] * cbufY[3] +
                matSrc.at<uchar>(sy-1, sx+1) * cbufX[2] * cbufY[0] +
                matSrc.at<uchar>(sy,   sx+1) * cbufX[2] * cbufY[1] +
                matSrc.at<uchar>(sy+1, sx+1) * cbufX[2] * cbufY[2] +
                matSrc.at<uchar>(sy+2, sx+1) * cbufX[2] * cbufY[3] +
                matSrc.at<uchar>(sy-1, sx+2) * cbufX[3] * cbufY[0] +
                matSrc.at<uchar>(sy,   sx+2) * cbufX[3] * cbufY[1] +
                matSrc.at<uchar>(sy+1, sx+2) * cbufX[3] * cbufY[2] +
                matSrc.at<uchar>(sy+2, sx+2) * cbufX[3] * cbufY[3]
                ) >> 22);
# endif
        }
    }
    
    cv::resize(matSrc, matDst2, matDst1.size(), 0, 0, 2);
    showImage(matSrc, matDst1, matDst2);

//    // Save image
//    cv::imwrite("../../HW2/bicubic_1.jpg", matDst1);
//    printf("Save matDst1 as bicubic_1.jpg\n");
//    cv::imwrite("../../HW2/bicubic_2.jpg", matDst2);
//    printf("Save matDst2 as bicubic_1.jpg\n");

    
    std::cout << "Bicubic interpolation done!!.\n\n";
}


int main(int argc, const char * argv[]) {
    // insert code here...
    std::cout << "This is homework2.\n\n";

    int size = 256;
    char FileNameOri[] = "../../doc/HW1/lena256.raw";
    int sizeSrc_width = size;
    int sizeSrc_height = size;
    OpenRAW(FileNameOri, sizeSrc_width, sizeSrc_height);
    
    size = 576;
    cv::Mat matSrc, matDst1, matDst2;
    sizDst_width = size;
    sizDst_height = size;
# ifdef color
    matSrc = cv::imread("../../doc/lena_256.jpg", 2 | 4);
# else
    //注意：当将参数copyData设为true后，则为深拷贝（复制整个图像数据）
    matSrc = cv::cvarrToMat(lenamat, true); //lenamat copy to matSrc
# endif
    matDst1 = cv::Mat(cv::Size(sizDst_width, sizDst_height), matSrc.type(), cv::Scalar::all(0));
    matDst2 = cv::Mat(matDst1.size(), matSrc.type(), cv::Scalar::all(0));
    
    double scale_x = (double)matSrc.cols / matDst1.cols;
    double scale_y = (double)matSrc.rows / matDst1.rows;
    
//    // Nearest neighbor
//    nearest_neighbor(matSrc, matDst1, matDst2, scale_x, scale_y);
    
    // Bilinear
    bilinear(matSrc, matDst1, matDst2, scale_x, scale_y);
    
//    // Bicubic
//    bicubic(matSrc, matDst1, matDst2, scale_x, scale_y);
    

    return 0;
}
