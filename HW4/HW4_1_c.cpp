//
//  HW4_1_c.cpp
//  ADIP_Homework
//
//  Created by WeiTingChen on 11/1/16.
//  Copyright © 2016 WeiTingChen. All rights reserved.
//

#include <stdio.h>
#include <iostream>
#include "filter.hpp"
#include "OpenRawFile.hpp"

int main( )
{
    std::cout << "This is homework4_1_b.\n\n";
    
    char str1[] = "../../doc/HW4/lena_512_salt&pepper.raw";
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
    Mat output_median = image.clone();
    median_filter(image, output_median, mask_Size);
    
    //Display the original image
    namedWindow("Display Image");
    imshow("Display Image", image);
    moveWindow("Display Image", 0, 0);
    
    //Display the Low Pass Filtered Image image
    namedWindow("median Filtered Image");
    imshow("median Filtered Image", output_median);
    moveWindow("median Filtered Image", output_median.cols, 0);
    
    waitKey(0);
    
    // Save image
    imwrite("../../HW4/output/median.jpg", output_median);
    printf("Save median.jpg\n");
    
    return 0;
}
