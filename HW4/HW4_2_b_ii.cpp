//
//  HW4_2_b_ii.cpp
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
    std::cout << "This is homework4_2_b_ii.\n\n";
    
    char str_input_image[] = "../../doc/HW4/lena_512_mix.raw";
    float size = 512;
    int sizeSrc_width = size;
    int sizeSrc_height = size;
    CvMat *image_cvMat_to_Mat = cvCreateMat(sizeSrc_width, sizeSrc_height, CV_8UC1);
    OpenRAW(str_input_image, image_cvMat_to_Mat);
    
    // Load the image
    Mat image = cv::cvarrToMat(image_cvMat_to_Mat, true);
    
    //If image not found
    if (!image.data)
    {
        cout << "No image data \n";
        return -1;
    }
    
    int mask_Size = 3;
    Mat output = image.clone();
    Mat output_2 = image.clone();
    median_filter(image, output_2, mask_Size);
    low_pass_filter(output_2, output, mask_Size);
    
    //Display the original image
    namedWindow("Display Image");
    imshow("Display Image", image);
    moveWindow("Display Image", 0, 0);
    
    //Display the Low Pass Filtered Image image
    namedWindow("Filtered Image");
    imshow("Filtered Image", output);
    moveWindow("Filtered Image", output.cols, 0);
    
    waitKey(0);
    
    // Save image
    imwrite("../../HW4/output/lena_512_mix.jpg", image);
    imwrite("../../HW4/output/median_low_pass.jpg", output);
    printf("Save median_low_pass.jpg\n");
    
    return 0;
}
