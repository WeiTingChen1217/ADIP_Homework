//
//  HW4_1_a.cpp
//
//  Created by WeiTingChen on 10/26/16.
//  Copyright © 2016 WeiTingChen. All rights reserved.
//
// Reference: http://webcache.googleusercontent.com/search?q=cache:LVlKsnGY2QgJ:opencv-hub.blogspot.com/2016/02/opencv-code-for-low-pass-averaging-filter.html+&cd=8&hl=zh-TW&ct=clnk&gl=tw&client=safari
// Opencv C++ Tutorial on Low Pass Averaging Filter
#include <iostream>
#include "filter.hpp"

//using namespace cv;
//using namespace std;
//
//void low_pass_filter(Mat src1, int a){
//    // create a empty size
//    Mat src2 = src1.clone();
//    
//    //for loop for counting the number of rows and columns and displaying the pixel value at each point
//    for (int i = 0; i < src1.rows-a; i++)
//        for (int j = 0; j < src1.cols-a; j++)
//        {
//            Scalar intensity1 = 0;
//            Scalar intensity2;
//            for (int p = 0; p < a; p++)
//                for (int q = 0; q < a; q++)
//                {
//                    intensity1 = src1.at<uchar>(i+p, j+q);
//                    intensity2.val[0] += intensity1.val[0]; // 單一通道
//                }
//            src2.at<uchar>(i+(a-1)/2, j+(a-1)/2) = intensity2.val[0] / (a*a);
//        }
//    
//    
//    //Display the original image
//    namedWindow("Display Image");
//    imshow("Display Image", src1);
//    moveWindow("Display Image", 0, 0);
//    
//    
//    //Display the Low Pass Filtered Image image
//    namedWindow("Low Pass Filtered Image");
//    imshow("Low Pass Filtered Image", src2);
//    moveWindow("Low Pass Filtered Image", src2.cols, 0);
//    waitKey(0);
//}

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

int main( )
{
    std::cout << "This is homework4_1_a.\n\n";
    
    Mat src1,src2;

    char str1[] = "../../doc/HW4/lena_512_gaussian.raw";
    float size = 512;
    int sizeSrc_width = size;
    int sizeSrc_height = size;
    OpenRAW(str1, sizeSrc_width, sizeSrc_height);
    
    // Load the image
    Mat image = cv::cvarrToMat(lenamat, true);
    
    
    
    //If image not found
    if (!image.data)
    {
        cout << "No image data \n";
        return -1;
    }
    
    //Take the mask size
    //It should be less than that of size of the image
    int mask_Size = 3;
//    cout << "Enter the mask Size = ";
//    cin  >> mask_Size;
    
    Mat output_low_pass = image.clone();
    low_pass_filter(image, output_low_pass, mask_Size);
    
    //Display the original image
    namedWindow("Display Image");
    imshow("Display Image", image);
    moveWindow("Display Image", 0, 0);
    
    
    //Display the Low Pass Filtered Image image
    namedWindow("Low Pass Filtered Image");
    imshow("Low Pass Filtered Image", output_low_pass);
    moveWindow("Low Pass Filtered Image", output_low_pass.cols, 0);
    
    waitKey(0);
    
    return 0;
}