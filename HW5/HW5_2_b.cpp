//
//  HW5_2_b.cpp
//  ADIP_Homework
//
//  Created by WeiTingChen on 11/16/16.
//  Copyright © 2016 WeiTingChen. All rights reserved.
//

#include <iostream>
#include "OpenRawFile.hpp"
#include "FFT.hpp"

int main() {
    
    std::cout << "This is homework5_2_b.\n\n";
    
    /// Load file
    char str_input_image_1[] = "../../doc/HW5/clown_128x128.raw";
    char str_input_image_2[] = "../../doc/HW5/clown_mask_128x128.raw";
    
    int sizeSrc_width = 128;
    int sizeSrc_height = 128;
    
    CvMat *image_cvMat_1 = cvCreateMat(sizeSrc_width, sizeSrc_height, CV_8UC1);
    CvMat *image_cvMat_2 = cvCreateMat(sizeSrc_width, sizeSrc_height, CV_8UC1);
    
    OpenRAW(str_input_image_1, image_cvMat_1);
    OpenRAW(str_input_image_2, image_cvMat_2);
    
    // Chamge the format to Mat
    Mat inputImg_1 = cv::cvarrToMat(image_cvMat_1, true);
    Mat inputImg_2 = cv::cvarrToMat(image_cvMat_2, true);
    
    
    Mat magI_1 = inputImg_1.clone();
    
    Mat complexImg_1;
    
    FFT_opencv(inputImg_1, magI_1, &complexImg_1);
    
    Mat magI_2 = magI_1.clone();
    
    for(int i = 0; i < magI_1.rows; i++)
        for(int j = 0; j < magI_1.cols; j++){
            if(inputImg_2.at<uchar>(i, j) == 0)
                magI_1.at<uchar>(i, j) = inputImg_2.at<uchar>(i, j) & magI_1.at<uchar>(i, j);
        }
    
    imshow("Imput_1", inputImg_1);
    moveWindow("Imput_1", 0, 0);
    imshow("Imput_2", inputImg_2);
    moveWindow("Imput_2", 130, 0);
    
    imshow("Spectrum_1", magI_1);
    moveWindow("Spectrum_1", 0, 200);

    
    
    waitKey(0);
    
    // Save image
    imwrite("../../HW5/output/clown_128x128.jpg", inputImg_1);
    imwrite("../../HW5/output/clown_mask_128x128.jpg", inputImg_2);

    imwrite("../../HW5/output/homework5_2_b.jpg", magI_1);
    imwrite("../../HW5/output/homework5_2_b_2.jpg", magI_2);

    
    return 0;
}