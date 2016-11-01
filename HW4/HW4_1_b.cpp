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
    
    char str_input_image[] = "../../doc/HW4/lena_512.raw";
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
    // 做的次數：
    int do_number = 1;
    cout << "做的次數：";
    cin >> do_number;
    printf("次數 %d\n", do_number);
    
    int mask_Size = 3;
    Mat output_high_pass = image.clone();
    Mat output_high_pass_2 = image.clone();
    
    for (int i=0; i < do_number; i++) {
        high_pass_filter(output_high_pass, output_high_pass_2, mask_Size);
        output_high_pass = output_high_pass_2.clone();
    }
    
    //Display the original image
    namedWindow("Display Image");
    imshow("Display Image", image);
    moveWindow("Display Image", 0, 0);
    
    //Display the Low Pass Filtered Image image
    namedWindow("high Pass Filtered Image");
    imshow("high Pass Filtered Image", output_high_pass);
    moveWindow("high Pass Filtered Image", output_high_pass.cols, 0);
    
    waitKey(0);
    
    // Define save file name
    char str_save_name[30];
    char str_do_number[30];
    strcpy(str_save_name, "../../HW4/output/high_pass_");
    sprintf(str_do_number, "%d", do_number);
    strcat(str_save_name, str_do_number);
    strcat(str_save_name, ".jpg");
    puts(str_save_name);
    
    // Save image
    imwrite("../../HW4/output/lena_512.jpg", image);
    imwrite(str_save_name, output_high_pass);
    printf("Save image.\n");
    
    return 0;
}