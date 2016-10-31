//
//  HW4_1_b.cpp
//  ADIP_Homework
//
//  Created by WeiTingChen on 10/31/16.
//  Copyright © 2016 WeiTingChen. All rights reserved.
//

#include <stdio.h>
#include <iostream>
#include "filter.hpp"
#include "OpenRawFile.hpp"

int main( )
{
    std::cout << "This is homework4_1_b.\n\n";
    
    char str1[] = "../../doc/HW4/lena_512.raw";
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
    Mat output_high_pass = image.clone();
    high_pass_filter(image, output_high_pass, mask_Size);
    
    //Display the original image
    namedWindow("Display Image");
    imshow("Display Image", image);
    moveWindow("Display Image", 0, 0);
    
    //Display the Low Pass Filtered Image image
    namedWindow("Low Pass Filtered Image");
    imshow("Low Pass Filtered Image", output_high_pass);
    moveWindow("Low Pass Filtered Image", output_high_pass.cols, 0);
    
    waitKey(0);
    
    // Save image
    imwrite("../../HW4/output/high_pass.jpg", output_high_pass);
    printf("Save high_pass.jpg\n");
    
    return 0;
}