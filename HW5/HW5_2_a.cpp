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
    char str_input_image[] = "../../doc/HW5/blackwhite_256.raw";
    int sizeSrc_width = 256;
    int sizeSrc_height = 256;
    CvMat *image_cvMat = cvCreateMat(sizeSrc_width, sizeSrc_height, CV_8UC1);
    OpenRAW(str_input_image, image_cvMat);
    // Chamge the format to Mat
    Mat inputImg = cv::cvarrToMat(image_cvMat, true);

//    // 二值化
//    threshold(inputImg, inputImg, 150, 255, THRESH_BINARY);
    
    Mat magI = inputImg.clone();
    Mat complexImg;
    complexImg = FFT_opencv(inputImg, magI);
    
    imshow("Imput", inputImg);
    moveWindow("Imput", 0, 0);

    imshow("Spectrum", magI);
    moveWindow("Spectrum", 0, 200);

    //逆向傅立葉轉換
    Mat ifft;  
    idft(complexImg, ifft, DFT_REAL_OUTPUT);
    normalize(ifft, ifft, 0, 1, CV_MINMAX);
    
    imshow("Reverse to origion", ifft);
    moveWindow("Reverse to origion", 0, 400);
    
    waitKey(0);

    // Save image
    imwrite("../../HW5/output/blackwhite_256.jpg", magI);
    
    return 0;
}
