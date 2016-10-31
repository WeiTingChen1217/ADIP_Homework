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
#include "OpenRawFile.hpp"

int main( )
{
    std::cout << "This is homework4_1_a.\n\n";
    
    char str1[] = "../../doc/HW4/lena_512_gaussian.raw";
    float size = 512;
    int sizeSrc_width = size;
    int sizeSrc_height = size;
    CvMat *str2 = cvCreateMat(sizeSrc_width, sizeSrc_height, CV_8UC1);
    OpenRAW(str1, str2);
    
    // Load the image
    Mat image = cv::cvarrToMat(str2, true);
    
    //If image not found
    if (!image.data)
    {
        cout << "No image data \n";
        return -1;
    }
    
    int mask_Size = 3;
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