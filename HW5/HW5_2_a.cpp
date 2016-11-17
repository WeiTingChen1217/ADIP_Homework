//
//  main.cpp
//  HW5
//
//  Created by WeiTingChen on 11/10/16.
//  Copyright © 2016 WeiTingChen. All rights reserved.
//

#include <iostream>
#include "OpenRawFile.hpp"
#include "FFT.hpp"

int main() {

    std::cout << "This is homework5_2_a.\n\n";
    
    /// Load file
    char str_input_image_1[] = "../../doc/HW5/sine4_128x128.raw";
    char str_input_image_2[] = "../../doc/HW5/sine16_128x128.raw";

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
    Mat magI_2 = inputImg_2.clone();
    Mat complexImg_1;
    Mat complexImg_2;
    
    FFT_opencv(inputImg_1, magI_1, &complexImg_1);
    FFT_opencv(inputImg_2, magI_2, &complexImg_2);

    
    imshow("Imput_1", inputImg_1);
    moveWindow("Imput_1", 0, 0);
    imshow("Imput_2", inputImg_2);
    moveWindow("Imput_2", 130, 0);

    imshow("Spectrum_1", magI_1);
    moveWindow("Spectrum_1", 0, 200);
    imshow("Spectrum_2", magI_2);
    moveWindow("Spectrum_2", 130, 200);

//    //逆向傅立葉轉換
//    Mat ifft;  
//    idft(complexImg, ifft, DFT_REAL_OUTPUT);
//    normalize(ifft, ifft, 0, 1, CV_MINMAX);
//    
//    imshow("Reverse to origion", ifft);
//    moveWindow("Reverse to origion", 0, 400);
    
    waitKey(0);

    // Save image
    imwrite("../../HW5/output/sine4_128x128.jpg", inputImg_1);
    imwrite("../../HW5/output/sine16_128x128.jpg", inputImg_2);

    imwrite("../../HW5/output/sine4_128x128_FFT.jpg", magI_1);
    imwrite("../../HW5/output/sine16_128x128_FFT.jpg", magI_2);

//    cout << inputImg_1 << endl;

    
    return 0;
}
